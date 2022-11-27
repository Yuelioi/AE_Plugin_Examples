

#include "PanelatorUI_Plat.h"
#include <stdio.h>

const char OSWndObjectProperty[] = "PanelatorUI_PlatPtr";

const int BtnID = 444;

LRESULT CALLBACK WndProd(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM IParam) {
	switch (msgID) {
	case WM_DESTROY:
		PostQuitMessage(0);
		//hWnd.width = 0;
		break;
	}

	if (GetAsyncKeyState(VK_ESCAPE)) {
		hWnd->unused;
		//PostQuitMessage(0);
	}
	return DefWindowProc(hWnd, msgID, wParam, IParam);
}

PanelatorUI_Plat::PanelatorUI_Plat(SPBasicSuite* spbP, AEGP_PanelH panelH,
	AEGP_PlatformViewRef platformWindowRef,
	AEGP_PanelFunctions1* outFunctionTable)
	: PanelatorUI(spbP, panelH, platformWindowRef, outFunctionTable),
	i_prevWindowProc(NULL)
{

	//// hook the main window
	//i_prevWindowProc = (WindowProc)GetWindowLongPtr(platformWindowRef, GWLP_WNDPROC);
	//// blasting the client's wndproc, it's required that they provide an adapter.

	//SetWindowLongPtrA(platformWindowRef, GWLP_WNDPROC, (LONG_PTR)PanelatorUI_Plat::StaticOSWindowWndProc);
	//::SetProp(platformWindowRef, OSWndObjectProperty, (HANDLE)this);


	POINT pt = { 0,0 };

	GetCursorPos(&pt);


	HINSTANCE hIns;
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
		WS_POPUP, // 风格
		100, 100, 100, 100,
		NULL, // 父窗口
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



	ShowWindow(hWnd, SW_SHOW);
	ShowWindow(hWnd1, SW_SHOW);
	ShowWindow(hWnd2, SW_SHOW);
	ShowWindow(hWnd3, SW_SHOW);
	ShowWindow(hWnd4, SW_SHOW);
	UpdateWindow(hWnd);

	//	MSG nMsg = {};
	//	while (GetMessage(&nMsg, NULL, 0, 0)) {
	//		TranslateMessage(&nMsg); // 翻译消息
	//		DispatchMessage(&nMsg); // 派发消息(谁处理消息 就派发给谁 -> 窗口处理函数)
	//};




}

LRESULT CALLBACK	PanelatorUI_Plat::StaticOSWindowWndProc(HWND	hWnd,
	UINT	message,
	WPARAM	wParam,
	LPARAM	lParam)
{
	// suck out our window ptr
	PanelatorUI_Plat* platPtr = reinterpret_cast<PanelatorUI_Plat*>(::GetProp(hWnd, OSWndObjectProperty));
	if (platPtr) {
		return platPtr->OSWindowWndProc(hWnd, message, wParam, lParam);
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

HBRUSH CreateBrushFromSelector(PFAppSuite4* appSuiteP, PF_App_ColorType sel)
{
	PF_App_Color	appColor = { 0 };
	appSuiteP->PF_AppGetColor(sel, &appColor);
	return CreateSolidBrush(RGB(appColor.red / 255, appColor.green / 255, appColor.blue / 255));
}

LRESULT PanelatorUI_Plat::OSWindowWndProc(HWND	hWnd,
	UINT	message,
	WPARAM	wParam,
	LPARAM	lParam)
{
	LRESULT result = 0;
	// do I want to do something
	bool handledB = false;
	switch (message) {

	case WM_SIZING:
		InvalidateAll();
		break;


	}


	if (i_prevWindowProc && !handledB) {
		result = CallWindowProc(i_prevWindowProc, hWnd,
			message,
			wParam,
			lParam);
	}
	else {
		result = DefWindowProc(hWnd, message, wParam, lParam);
	}
	return result;

}


void PanelatorUI_Plat::InvalidateAll()
{
	RECT clientArea;
	GetClientRect(i_refH, &clientArea);

	InvalidateRect(i_refH, &clientArea, FALSE);
}
