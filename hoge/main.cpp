#include <windows.h>

//グローバル変数
HINSTANCE hInst;	//インスタンス
WCHAR szTitle[] = L"テストアプリ";		//タイトル
WCHAR szWindowClass[] = L"MyTestApp";	//ウィンドウクラス名

//関数プロトタイプ宣言
ATOM MyRegisterClass(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

//ウィンドウプロシージャのプロトタイプ宣言
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow)
{
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow))
	{
		return 0;
	}

	//必要ないので削除
	//MessageBox(NULL, L"ウィンドウアプリのテスト", L"タイトル", MB_OK);

	//MSG構造体の変数を宣言
	MSG msg;
	BOOL ret;

	//メッセージループ
	while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (ret == -1)
		{
			//GetMessage関数の実行失敗
			return -1;
		}
		//if (msg.message == WM_LBUTTONUP)
		//	break;
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//ウィンドウクラスの登録
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

//ウィンドウの作成
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	//グローバル変数にインスタンスハンドルを格納する
	hInst = hInstance;

	HWND hWnd = CreateWindowW(
		szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd) {
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	return TRUE;
}

//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;

	static int l, r, c;
	WCHAR buf[BUFFERSIZE];
	static const WCHAR* format = L"クリック回数\n"
		L"L: %d\n"
		L"R: %d\n"
		L"C: %d\n";

	switch (message)
	{
	case WM_PAINT: //ウィンドウの描画
		StringCchPrintf(buf, BUFFERSIZE, format, l, r, c);

		GetClientRect(hWnd, &rt);
		hdc = BeginPaint(hWnd, &ps);

		DrawText(hdc, buf, -1, &rt, DT_WORDBREAK);

		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONUP: //マウス左クリック
		l++;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_RBUTTONUP: //マウス右クリック
		r++;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_MBUTTONUP: //マウス中クリック
		c++;
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY: //ウィンドウの破棄
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//int main()
//{
//	ifstream ifs("C:\\Users\\NaokiMurakami\\3D Objects\\walkman.ply", ios::binary);
//	if (!ifs.is_open())
//	{
//		printf("Not Found File\n");
//		return 0;
//	}
//
//	string headerStr;
//	unsigned int vertNum = 0;
//	unsigned int indiceNum = 0;
//	while (headerStr != "end_header" && !ifs.eof())
//	{
//		ifs >> headerStr;
//		cout << headerStr << " ";
//		if (headerStr == "comment")
//		{
//			getline(ifs, headerStr);
//			cout << headerStr << endl;
//		}
//		else if (headerStr == "element")
//		{
//			string elementInfo;
//			ifs >> elementInfo;
//			cout << elementInfo << " ";
//			if (elementInfo == "vertex")
//			{
//				ifs >> vertNum;
//				cout << vertNum << endl;
//			}
//			else if (elementInfo == "face")
//			{
//				ifs >> indiceNum;
//				cout << indiceNum << endl;
//			}
//		}
//		//本当はpropertyに応じてレイアウトとかを決定したい
//		else if (headerStr == "property")
//		{
//
//		}
//	}
//	printf("vertex: %u\n", vertNum);
//	printf("face: %u\n", indiceNum);
//
//	ifs.close();
//	return 0;
//}