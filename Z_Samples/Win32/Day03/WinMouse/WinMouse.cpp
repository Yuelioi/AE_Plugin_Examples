#include <windows.h>
#include <stdio.h>

HANDLE g_hOutput; // 句柄 控制台输出用

void OnMouseWheel(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "滚轮 其他按键状态:%d 滚轮移动:%d x:%d y:%d\n", LOWORD(wParam),HIWORD(wParam), LOWORD(lParam), HIWORD(lParam)); // 120
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
}

void OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	char szText[256] = {0};
	sprintf_s(szText,"左键按下 其他按键状态:%d x:%d y:%d\n",wParam, LOWORD( lParam),HIWORD(lParam));
	WriteConsole(g_hOutput,szText, strlen(szText),NULL,NULL); // 左键按下 其他按键状态:9 x:343 y:244  (这个9 包括按下左键本身, 原理是其他按键+左键(1)) 可以用抬起测试具体按键数值 
}

void OnDBLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "左键双击按下 其他按键状态:%d x:%d y:%d\n", wParam, LOWORD(lParam), HIWORD(lParam));
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL); // Down 其他按键状态:9 x:343 y:244 (这个9 包括按下左键本身, 原理是其他按键+左键(1)) 可以用抬起测试具体按键数值 
}

// 窗口处理函数(自定义 处理消息)
LRESULT CALLBACK WndProd(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam) {
	switch (msgID) {

	case WM_MOUSEWHEEL:
		OnMouseWheel(hWnd, wParam, lParam);
		break;

	case WM_LBUTTONDBLCLK:
		OnDBLButtonDown(hWnd, wParam, lParam);
		break;

	case WM_LBUTTONDOWN:
		OnLButtonDown(hWnd, wParam, lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msgID, wParam, lParam);
}

// 入口函数
int CALLBACK WinMain(HINSTANCE hIns, HINSTANCE hPreIns, LPSTR IpCmdLine, int nCmdShow) {

	// 允许显示控制台
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
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // 窗口水平/垂直变化时 重绘窗口
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