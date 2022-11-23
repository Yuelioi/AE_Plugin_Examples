#include <windows.h>
// 窗口处理函数(自定义 处理消息)
LRESULT CALLBACK WndProd(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM IParam) {
	return DefWindowProc(hWnd, msgID, wParam, IParam);
}

// 入口函数
int CALLBACK WinMain(HINSTANCE hIns, HINSTANCE hPreIns, LPSTR IpCmdLine, int nCmdShow) {
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