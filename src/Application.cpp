#include "Application.h"
#include <windowsx.h>
#include <filesystem>
#include <iostream>
#include <regex>

#pragma comment(lib, "DirectXTK12.lib")
#pragma comment(lib, "dxguid.lib")

using namespace std::chrono;

//�E�B���h�E�T�C�Y
const unsigned int window_width = 1280;
const unsigned int window_height = 720;
ComPtr<ID3D12DescriptorHeap> _heapForSpriteFont;

//�J�n����
system_clock::time_point _frameStart;
int _fps = 0;
int _nowMeasureFps = 0;

//���͒l�֘A
int _wheel;
bool _downMButton = false;
bool _downLButton = false;

///�f�o�b�O���C���[��L���ɂ���
void EnableDebugLayer() {
	ID3D12Debug* debugLayer = nullptr;
	auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
	debugLayer->EnableDebugLayer();
	debugLayer->Release();
}

//�E�B���h�E�̃R�[���o�b�N�֐�
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_MOUSEWHEEL:
		{
			_wheel = GET_WHEEL_DELTA_WPARAM(wparam);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			_downMButton = true;
			break;
		}
		case WM_MBUTTONUP:
		{
			_downMButton = false;
			break;
		}
		case WM_LBUTTONDOWN:
		{
			_downLButton = true;
			break;
		}
		case WM_LBUTTONUP:
		{
			_downLButton = false;
			break;
		}
		/*case WM_INPUT:
		{
			UINT size = 0;
			GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
			BYTE* buffer = new BYTE[size];
			if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER)) != -1) {
				RAWINPUT* rawInput = (RAWINPUT*)buffer;
				if (rawInput->header.dwType == RIM_TYPEMOUSE) {
					wheel = rawInput->data.mouse.usButtonData;
				}
			}
			delete[] buffer;
			break;
		}*/
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
			break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//FPS�擾
int getFPS()
{
	auto frameEnd = system_clock::now();
	auto frameDuration = duration_cast<microseconds>(frameEnd - _frameStart).count();
	_nowMeasureFps++;

	if (frameDuration >= 1'000'000)
	{
		_frameStart = frameEnd;
		frameDuration = 0;
		_fps = _nowMeasureFps;
		_nowMeasureFps = 0;
	}

	return _fps;
}

//�E�B���h�E�N���X�̐ݒ�Ɛ���
void Application::CreateGameWindow(HWND& hwnd, WNDCLASSEX& w)
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProcedure;
	w.lpszClassName = TEXT("DirectXTest");
	w.hInstance = GetModuleHandle(nullptr);
	RegisterClassEx(&w);//�E�B���h�E�N���X�̓o�^(�������鎞�Ɏg������)

	RECT wrc = { 0, 0, window_width, window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	//�E�B���h�E�̐���
	hwnd = CreateWindow(w.lpszClassName,
		TEXT("DX12test"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr
	);
}

SIZE Application::GetWindowSize()const
{
	SIZE ret;
	ret.cx = window_width;
	ret.cy = window_height;
	return ret;
}

Application& Application::Instance()
{
	static Application instance;
	return instance;
}

bool Application::Init()
{
	//�E�B���h�E�֘A
	CreateGameWindow(_hwnd, _windowClass);

#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	EnableDebugLayer();
#endif

	//Wrapper������
	_dx12.reset(new Dx12Wrapper(_hwnd));

	//�����_���[������
	_renderer.reset(new renderer(_dx12));

	//���b�V���ǂݍ���
	_renderer->AddMesh("C:\\Users\\NaokiMurakami\\3D Objects\\lowpolycat\\cat_obj.obj");

	//GraphicsMemory������
	_geometry = new DirectX::GraphicsMemory(_dx12->Device());

	//SpriteBatch������
	DirectX::ResourceUploadBatch resUploadBatch(_dx12->Device());
	resUploadBatch.Begin();
	DirectX::RenderTargetState rtState(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	DirectX::SpriteBatchPipelineStateDescription pd(rtState);
	_spriteBatch = new DirectX::SpriteBatch(_dx12->Device(), resUploadBatch, pd);

	//SpriteFont������
	_heapForSpriteFont = _dx12->CreateDesHeapForSpriteFont();
	_spriteFont = new DirectX::SpriteFont(_dx12->Device(), resUploadBatch, L"C:\\Users\\NaokiMurakami\\source\\repos\\MyMeshViewer\\font\\fonttest.spritefont", _heapForSpriteFont->GetCPUDescriptorHandleForHeapStart(), _heapForSpriteFont->GetGPUDescriptorHandleForHeapStart());
	
	auto future = resUploadBatch.End(_dx12->CmdQue().Get());
	_dx12->WaitForCommandQueue();
	future.wait();
	_spriteBatch->SetViewport(_dx12->GetViewPort());

	return true;
}

void Application::Run()
{
	ShowWindow(_hwnd, SW_SHOW);
	_frameStart = system_clock::now();//FPS����̂��߂̎��Ԏ擾
	MSG msg = {};
	while (true)
	{
		_wheel = 0;//�}�E�X�֑���p�̃X�N���[���ʂ�������

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//�����A�v���P�[�V�������I�����Ď���message��WM_QUIT�ɂȂ�
		if (msg.message == WM_QUIT) {
			break;
		}

		_dx12->BeginDraw();//�[�x�ƃ����_�[�^�[�Q�b�g�ƃr���[�|�[�g�A�V�U�[��`
		_renderer->SetPipelineAndSignature();//�p�C�v���C���ƃV�O�l�`���̃Z�b�g
		_renderer->setMatData();//���W�ϊ��p�̍s����Z�b�g
		_renderer->Draw();//renderer�̕ێ�����mesh��Draw()���ĂԁB���_�C���f�b�N�X�r���[�ƃg�|���W�[��ݒ肵����ɕ`�悷��B
		_renderer->Update(_wheel, _downMButton, _downLButton);//���W�ϊ��̒l�X�V��

		//��������(debug�p)
		_dx12->CommandList()->SetDescriptorHeaps(1, _heapForSpriteFont.GetAddressOf());
		_spriteBatch->Begin(_dx12->CommandList().Get());
		int fps = getFPS();
		_spriteFont->DrawString(_spriteBatch, ((wstring)L"FPS: " + std::to_wstring(fps)).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::Black, 0.0f, XMFLOAT2(0, 0), 0.5f);
		_spriteFont->DrawString(_spriteBatch, ((wstring)L"phi: " + std::to_wstring((_renderer->_phi + _renderer->_changePhi) * 180.0 / XM_PI) + L", theta: " + std::to_wstring((_renderer->_theta + _renderer->_changeTheta) * 180.0 / XM_PI)).c_str(), DirectX::XMFLOAT2(0, 20), DirectX::Colors::Black, 0.0f, XMFLOAT2(0, 0), 0.5f);
		_spriteBatch->End();

		_dx12->EndDraw();//�R�}���h�L���[�̃N���[�Y���t�F���X���

		//�t���b�v
		_dx12->Swapchain()->Present(1, 0);

		_geometry->Commit(_dx12->CmdQue().Get());
	}
}

void Application::Terminate()
{
	UnregisterClass(_windowClass.lpszClassName, _windowClass.hInstance);
}

Application::Application()
{
}
Application::~Application()
{
}