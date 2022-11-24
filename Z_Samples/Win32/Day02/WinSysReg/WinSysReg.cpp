#include <windows.h>
#include <stdio.h>

HANDLE g_hOutput = 0; // 接受标准输出句柄

#define WM_MYMESSAGE WM_USER+10 // (小于31743的值)

void OnCreate(HWND hWnd, LPARAM lParam) {
	CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
	char* pszText = (char*)pcs->lpszName; // window
	MessageBox(hWnd, "窗口", pszText, MB_YESNO);


	// 自定义消息
	PostMessage(hWnd, WM_MYMESSAGE, 1, 2);
}

void OnMyMes(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "%d,%d", wParam, lParam);
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
	MessageBox(hWnd,  szText, "自定义消息窗口", MB_YESNO);
}

void OnSize(HWND hWnd, LPARAM lParam) {
	short nWidth = LOWORD(lParam);
	short nHight = LOWORD(lParam);
	char szText[256] = { 0 };
	sprintf_s(szText, "宽%d 高%d\n", nWidth, nHight);
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
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

	case WM_MYMESSAGE:
		OnMyMes(hWnd, wParam, lParam);
		break;

	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			int nRet = MessageBox(hWnd, "是否关闭窗口", "提示关闭窗口", MB_YESNO);
			if (nRet == IDYES) {
				// 什么都不写 自动跳过
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

// 入口函数
int CALLBACK WinMain(HINSTANCE hIns, HINSTANCE hPreIns, LPSTR IpCmdLine, int nCmdShow) {

	POINT pt = { 0,0 };

	GetCursorPos(&pt);

	AllocConsole(); // 增加dos界面
	g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	WNDCLASS wc = { 0 };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = NULL;// 默认鼠标样式
	wc.hIcon = NULL;// Icon
	wc.hInstance = hIns;
	wc.lpfnWndProc = WndProd; //处理函数名称
	wc.lpszClassName = "Main";
	wc.lpszMenuName = NULL; // 不要菜单
	wc.style = CS_HREDRAW | CS_VREDRAW; // 窗口水平/垂直变化时 重绘窗口


	RegisterClass(&wc); // 将以上赋值写入操作系统	
	HWND hWnd = CreateWindow( // 返回句柄
		"Main",
		"window",
		WS_OVERLAPPEDWINDOW, // 风格
		100, 100, 400, 400,
		NULL, // 父窗口
		NULL, // 菜单
		hIns,
		NULL
	);

	// 子窗口


	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = NULL;// 默认鼠标样式
	wc.hIcon = NULL;// Icon
	wc.hInstance = hIns;
	wc.lpfnWndProc = DefWindowProc; // Window 默认处理函数
	wc.lpszClassName = "Child";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW; // 窗口水平/垂直变化时 重绘窗口


	RegisterClass(&wc); // 将以上赋值写入操作系统

	HWND hChd1 = CreateWindow(
		"Child",
		"c1",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, // 风格
		pt.x, pt.y, 100, 30,
		hWnd, // 父窗口
		NULL, // 菜单
		hIns,
		NULL
	);
	HWND hChd2 = CreateWindow(
		"Child",
		"c2",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, // 风格
		pt.x, pt.y, 100, 30,
		hWnd, // 父窗口
		NULL, // 菜单
		hIns,
		NULL
	);



	// 创建系统窗口类 无需注册
	HWND hWnd1 = CreateWindow(
		"Button",
		"BTN1",
		WS_POPUP, // 风格
		pt.x, pt.y, 100, 30,
		hWnd, // 父窗口
		NULL, // 菜单
		hIns,
		NULL
	);
	HWND hWnd2 = CreateWindow(
		"Button",
		"BTN2",
		WS_POPUP, // 风格
		pt.x + 100, pt.y + 100, 100, 30,
		hWnd, // 父窗口
		NULL, // 菜单
		hIns,
		NULL
	);
	HWND hWnd3 = CreateWindow(
		"Button",
		"BTN2",
		WS_POPUP, // 风格
		pt.x - 100, pt.y + 100, 100, 30,
		hWnd, // 父窗口
		NULL, // 菜单
		hIns,
		NULL
	);
	HWND hWnd4 = CreateWindow(
		"Button",
		"BTN2",
		WS_POPUP, // 风格
		pt.x, pt.y + 200, 100, 30,
		hWnd, // 父窗口
		NULL, // 菜单
		hIns,
		NULL
	);

	// 显示窗口
	ShowWindow(hWnd, SW_SHOW);
	ShowWindow(hWnd1, SW_SHOW);
	ShowWindow(hWnd2, SW_SHOW);
	ShowWindow(hWnd3, SW_SHOW);
	ShowWindow(hWnd4, SW_SHOW);
	UpdateWindow(hWnd);

	// 消息循环
	MSG nMsg = {};
	while (1) {
		if (PeekMessage(&nMsg, NULL, 0, 0, PM_NOREMOVE)) {
			if (GetMessage(&nMsg, NULL, 0, 0)) {
				TranslateMessage(&nMsg);
				DispatchMessage(&nMsg);
			}
			else {
				return 0; // 退出程序
			}
		}
		else {
			// 没消息 空闲处理
			WriteConsole(g_hOutput, "Onldle", strlen("Onldle"), NULL, NULL);
		}
	}
	return 0;
}