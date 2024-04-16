#include <windows.h>

//�O���[�o���ϐ�
HINSTANCE hInst;	//�C���X�^���X
WCHAR szTitle[] = L"�e�X�g�A�v��";		//�^�C�g��
WCHAR szWindowClass[] = L"MyTestApp";	//�E�B���h�E�N���X��

//�֐��v���g�^�C�v�錾
ATOM MyRegisterClass(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

//�E�B���h�E�v���V�[�W���̃v���g�^�C�v�錾
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

	//�K�v�Ȃ��̂ō폜
	//MessageBox(NULL, L"�E�B���h�E�A�v���̃e�X�g", L"�^�C�g��", MB_OK);

	//MSG�\���̂̕ϐ���錾
	MSG msg;
	BOOL ret;

	//���b�Z�[�W���[�v
	while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (ret == -1)
		{
			//GetMessage�֐��̎��s���s
			return -1;
		}
		//if (msg.message == WM_LBUTTONUP)
		//	break;
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//�E�B���h�E�N���X�̓o�^
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

//�E�B���h�E�̍쐬
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	//�O���[�o���ϐ��ɃC���X�^���X�n���h�����i�[����
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

//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;

	static int l, r, c;
	WCHAR buf[BUFFERSIZE];
	static const WCHAR* format = L"�N���b�N��\n"
		L"L: %d\n"
		L"R: %d\n"
		L"C: %d\n";

	switch (message)
	{
	case WM_PAINT: //�E�B���h�E�̕`��
		StringCchPrintf(buf, BUFFERSIZE, format, l, r, c);

		GetClientRect(hWnd, &rt);
		hdc = BeginPaint(hWnd, &ps);

		DrawText(hdc, buf, -1, &rt, DT_WORDBREAK);

		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONUP: //�}�E�X���N���b�N
		l++;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_RBUTTONUP: //�}�E�X�E�N���b�N
		r++;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_MBUTTONUP: //�}�E�X���N���b�N
		c++;
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY: //�E�B���h�E�̔j��
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
//		//�{����property�ɉ����ă��C�A�E�g�Ƃ������肵����
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