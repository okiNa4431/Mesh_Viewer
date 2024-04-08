#include "Application.h"

//�E�B���h�E�T�C�Y
const unsigned int window_width = 1280;
const unsigned int window_height = 720;

//�E�B���h�E�̃R�[���o�b�N�֐�
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
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
	_dx12.reset(new Dx12Wrapper(_hwnd));
	return true;
}

void Application::Run()
{
	ShowWindow(_hwnd, SW_SHOW);
	MSG msg = {};
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//�����A�v���P�[�V�������I�����Ď���message��WM_QUIT�ɂȂ�
		if (msg.message == WM_QUIT) {
			break;
		}

		_dx12->BeginDraw();//�[�x�ƃ����_�[�^�[�Q�b�g�ƃr���[�|�[�g�A�V�U�[��`

		_dx12->EndDraw();//�R�}���h�L���[�̃N���[�Y���t�F���X���

		//�t���b�v
		_dx12->Swapchain()->Present(1, 0);
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