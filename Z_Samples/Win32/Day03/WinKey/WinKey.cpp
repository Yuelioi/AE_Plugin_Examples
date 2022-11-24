#include <windows.h>
#include<stdio.h>

HANDLE g_hOutput;


void OnKeyDown(HWND hWnd, WPARAM wParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "OnKeyDow:%d\n", wParam);
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
}

void OnKeyUp(HWND hWnd, WPARAM wParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "OnKeyUp:%d\n", wParam);
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
}

void OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "WM_CHAR:%d | %d\n", wParam, lParam);
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
}

void OnPaint(HWND hWnd) {
	const char* pszText = "Paint\n";
	WriteConsole(g_hOutput, pszText, strlen(pszText), NULL, NULL);

	PAINTSTRUCT ps = { 0 };
	HDC hdc = BeginPaint(hWnd, &ps);
	TextOut(hdc, 100, 100, "hello", 5);
	EndPaint(hWnd, &ps);
	// 绘图代码, 需要放在处理WM_PAINT消息时调用

}

// 窗口处理函数(自定义 处理消息)
LRESULT CALLBACK WndProd(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam) {
	switch (msgID) {

	case WM_PAINT:
		OnPaint(hWnd);
		break;

	case WM_KEYDOWN:
		OnKeyDown(hWnd, wParam);
		break;
	case WM_KEYUP:
		OnKeyUp(hWnd, wParam);
		break;
	case WM_CHAR:
		OnChar(hWnd, wParam, 12);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}



	return DefWindowProc(hWnd, msgID, wParam, lParam);
}

// 入口函数
int CALLBACK WinMain(HINSTANCE hIns, HINSTANCE hPreIns, LPSTR IpCmdLine, int nCmdShow) {

	AllocConsole();
	g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);


	// 注册窗口类
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

	// 在内存中创建窗口
	HWND hWnd = CreateWindow( // 返回句柄
		"Main",
		"window",
		WS_OVERLAPPEDWINDOW, // 风格
		100, 100, 500, 500,
		NULL, // 父窗口
		NULL, // 菜单
		hIns,
		NULL
	);

	// 显示窗口
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// 消息循环
	MSG nMsg = {};
	while (GetMessage(&nMsg, NULL, 0, 0)) {
		TranslateMessage(&nMsg); // 翻译消息
		DispatchMessage(&nMsg); // 派发消息(谁处理消息 就派发给谁 -> 窗口处理函数)
	}
	return 0;
}