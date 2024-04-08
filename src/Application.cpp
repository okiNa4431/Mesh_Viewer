#include "Application.h"

//ウィンドウサイズ
const unsigned int window_width = 1280;
const unsigned int window_height = 720;

//ウィンドウのコールバック関数
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//ウィンドウクラスの設定と生成
void Application::CreateGameWindow(HWND& hwnd, WNDCLASSEX& w)
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProcedure;
	w.lpszClassName = TEXT("DirectXTest");
	w.hInstance = GetModuleHandle(nullptr);
	RegisterClassEx(&w);//ウィンドウクラスの登録(生成する時に使うため)

	RECT wrc = { 0, 0, window_width, window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	//ウィンドウの生成
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
	//ウィンドウ関連
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
		//もうアプリケーションが終わるって時にmessageがWM_QUITになる
		if (msg.message == WM_QUIT) {
			break;
		}

		_dx12->BeginDraw();//深度とレンダーターゲットとビューポート、シザー矩形

		_dx12->EndDraw();//コマンドキューのクローズやらフェンスやら

		//フリップ
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