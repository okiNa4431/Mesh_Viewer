#include "Application.h"

//ウィンドウサイズ
const unsigned int window_width = 1280;
const unsigned int window_height = 720;

///デバッグレイヤーを有効にする
void EnableDebugLayer() {
	ID3D12Debug* debugLayer = nullptr;
	auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
	debugLayer->EnableDebugLayer();
	debugLayer->Release();

}

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

#ifdef _DEBUG
	//デバッグレイヤーをオンに
	EnableDebugLayer();
#endif

	//Wrapper初期化
	_dx12.reset(new Dx12Wrapper(_hwnd));

	//レンダラー初期化
	_renderer.reset(new renderer(_dx12));

	//メッシュ読み込み
	_renderer->AddMesh("C:\\Users\\NaokiMurakami\\3D Objects\\walkman.ply");
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
		_renderer->SetPipelineAndSignature();//パイプラインとシグネチャのセット
		_renderer->setMatData();//座標変換用の行列をセット
		_renderer->Draw();//rendererの保持するmeshのDraw()を呼ぶ。頂点インデックスビューとトポロジーを設定した後に描画する。
		_renderer->Update();//座標変換の値更新等
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