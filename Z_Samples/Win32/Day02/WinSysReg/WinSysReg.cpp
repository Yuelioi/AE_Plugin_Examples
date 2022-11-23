#include <windows.h>
#include <stdio.h>

HANDLE g_hOutput = 0; // ���ܱ�׼������

void OnCreate(HWND hWnd, LPARAM lParam) {
	CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
	char* pszText = (char*)pcs->lpszName; // window
	MessageBox(hWnd, "����", pszText, MB_YESNO);
}

void OnSize(HWND hWnd, LPARAM lParam) {
	short nWidth = LOWORD(lParam);
	short nHight = LOWORD(lParam);
	char szText[256] = { 0 };
	sprintf_s(szText, "��%d ��%d\n", nWidth, nHight);
	WriteConsole(g_hOutput,szText,strlen(szText),NULL,NULL );

}

LRESULT CALLBACK WndProd(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam) {
	switch (msgID) {
	case WM_SIZE:
		OnSize(hWnd, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
		OnCreate(hWnd, lParam);

		break;

	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			int nRet = MessageBox(hWnd, "�Ƿ�رմ���", "��ʾ�رմ���", MB_YESNO);
			if (nRet == IDYES) {
				// ʲô����д �Զ�����
			}
			else {
				return 0;
			}
		}
		break;
	}
	if (GetAsyncKeyState(VK_ESCAPE)) {
		PostQuitMessage(0);
	}
	return DefWindowProc(hWnd, msgID, wParam, lParam);
}

// ��ں���
int CALLBACK WinMain(HINSTANCE hIns, HINSTANCE hPreIns, LPSTR IpCmdLine, int nCmdShow) {

	POINT pt = { 0,0 };

	GetCursorPos(&pt);

	AllocConsole(); // ����dos����
	g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	WNDCLASS wc = { 0 };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = NULL;// Ĭ�������ʽ
	wc.hIcon = NULL;// Icon
	wc.hInstance = hIns;
	wc.lpfnWndProc = WndProd; //����������
	wc.lpszClassName = "Main";
	wc.lpszMenuName = NULL; // ��Ҫ�˵�
	wc.style = CS_HREDRAW | CS_VREDRAW; // ����ˮƽ/��ֱ�仯ʱ �ػ洰��


	RegisterClass(&wc); // �����ϸ�ֵд�����ϵͳ	
	HWND hWnd = CreateWindow( // ���ؾ��
		"Main",
		"window",
		WS_OVERLAPPEDWINDOW, // ���
		100, 100, 400, 400,
		NULL, // ������
		NULL, // �˵�
		hIns,
		NULL
	);

	// �Ӵ���


	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = NULL;// Ĭ�������ʽ
	wc.hIcon = NULL;// Icon
	wc.hInstance = hIns;
	wc.lpfnWndProc = DefWindowProc; // Window Ĭ�ϴ�����
	wc.lpszClassName = "Child";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW; // ����ˮƽ/��ֱ�仯ʱ �ػ洰��


	RegisterClass(&wc); // �����ϸ�ֵд�����ϵͳ

	HWND hChd1 = CreateWindow(
		"Child",
		"c1",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, // ���
		pt.x, pt.y, 100, 30,
		hWnd, // ������
		NULL, // �˵�
		hIns,
		NULL
	);
	HWND hChd2 = CreateWindow(
		"Child",
		"c2",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, // ���
		pt.x, pt.y, 100, 30,
		hWnd, // ������
		NULL, // �˵�
		hIns,
		NULL
	);



	// ����ϵͳ������ ����ע��
	HWND hWnd1 = CreateWindow(
		"Button",
		"BTN1",
		WS_POPUP, // ���
		pt.x, pt.y, 100, 30,
		hWnd, // ������
		NULL, // �˵�
		hIns,
		NULL
	);
	HWND hWnd2 = CreateWindow(
		"Button",
		"BTN2",
		WS_POPUP, // ���
		pt.x + 100, pt.y + 100, 100, 30,
		hWnd, // ������
		NULL, // �˵�
		hIns,
		NULL
	);
	HWND hWnd3 = CreateWindow(
		"Button",
		"BTN2",
		WS_POPUP, // ���
		pt.x - 100, pt.y + 100, 100, 30,
		hWnd, // ������
		NULL, // �˵�
		hIns,
		NULL
	);
	HWND hWnd4 = CreateWindow(
		"Button",
		"BTN2",
		WS_POPUP, // ���
		pt.x, pt.y + 200, 100, 30,
		hWnd, // ������
		NULL, // �˵�
		hIns,
		NULL
	);

	// ��ʾ����
	ShowWindow(hWnd, SW_SHOW);
	ShowWindow(hWnd1, SW_SHOW);
	ShowWindow(hWnd2, SW_SHOW);
	ShowWindow(hWnd3, SW_SHOW);
	ShowWindow(hWnd4, SW_SHOW);
	UpdateWindow(hWnd);

	// ��Ϣѭ��
	MSG nMsg = {};
	while (GetMessage(&nMsg, NULL, 0, 0)) {
		TranslateMessage(&nMsg); // ������Ϣ
		DispatchMessage(&nMsg); // �ɷ���Ϣ(˭������Ϣ ���ɷ���˭ -> ���ڴ�����)
	}
	return 0;
}