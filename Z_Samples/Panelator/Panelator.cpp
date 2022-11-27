/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

#include <new>
#include "Panelator.h"
#include <windows.h>
#include <stdio.h>

HANDLE g_hOutput; // 句柄 控制台输出用

template <>
const A_char* SuiteTraits<AEGP_PanelSuite1>::i_name = kAEGPPanelSuite;
template <>
const int32_t SuiteTraits<AEGP_PanelSuite1>::i_version = kAEGPPanelSuiteVersion1;

class Panelator
{
public:
	SPBasicSuite* i_pica_basicP;
	AEGP_PluginID					i_pluginID;

	AEGP_SuiteHandler				i_sp;
	AEGP_Command					i_command;
	SuiteHelper<AEGP_PanelSuite1>	i_ps;
	const A_u_char* i_match_nameZ;

	/// STATIC BINDERS
	static SPAPI A_Err
		S_CommandHook(
			AEGP_GlobalRefcon	plugin_refconP,			/* >> */
			AEGP_CommandRefcon	refconP,				/* >> */
			AEGP_Command		command,				/* >> */
			AEGP_HookPriority	hook_priority,			/* >> currently always AEGP_HP_BeforeAE */
			A_Boolean			already_handledB,		/* >> */
			A_Boolean* handledPB)		/* << whether you handled */
	{
		PT_XTE_START{
			reinterpret_cast<Panelator*>(refconP)->CommandHook(command, hook_priority, already_handledB, handledPB);
		} PT_XTE_CATCH_RETURN_ERR;
	}

	static A_Err
		S_UpdateMenuHook(
			AEGP_GlobalRefcon		plugin_refconP,		/* >> */
			AEGP_UpdateMenuRefcon	refconP,			/* >> */
			AEGP_WindowType			active_window)		/* >> */
	{
		PT_XTE_START{
			reinterpret_cast<Panelator*>(plugin_refconP)->UpdateMenuHook(active_window);
		} PT_XTE_CATCH_RETURN_ERR;
	}

	static A_Err
		S_CreatePanelHook(
			AEGP_GlobalRefcon		plugin_refconP,
			AEGP_CreatePanelRefcon	refconP,
			AEGP_PlatformViewRef	container,
			AEGP_PanelH				panelH,
			AEGP_PanelFunctions1* outFunctionTable,
			AEGP_PanelRefcon* outRefcon)
	{
		PT_XTE_START{
			 reinterpret_cast<Panelator*>(plugin_refconP)->CreatePanelHook(container, panelH, outFunctionTable, outRefcon);
		} PT_XTE_CATCH_RETURN_ERR;
	}

	Panelator(
		SPBasicSuite* pica_basicP,
		AEGP_PluginID	pluginID) :
		i_pica_basicP(pica_basicP),
		i_pluginID(pluginID),
		i_sp(pica_basicP),
		i_ps(pica_basicP),
		i_match_nameZ((A_u_char*)STR(StrID_Name))
	{
		PT_ETX(i_sp.CommandSuite1()->AEGP_GetUniqueCommand(&i_command));
		PT_ETX(i_sp.CommandSuite1()->AEGP_InsertMenuCommand(i_command, STR(StrID_Name), AEGP_Menu_WINDOW, AEGP_MENU_INSERT_SORTED));

		PT_ETX(i_sp.RegisterSuite5()->AEGP_RegisterCommandHook(i_pluginID,
			AEGP_HP_BeforeAE,
			i_command,
			&Panelator::S_CommandHook,
			(AEGP_CommandRefcon)(this)));
		PT_ETX(i_sp.RegisterSuite5()->AEGP_RegisterUpdateMenuHook(i_pluginID,
			&Panelator::S_UpdateMenuHook,
			NULL));

		//PT_ETX(i_ps->AEGP_RegisterCreatePanelHook(i_pluginID, i_match_nameZ,
		//	&Panelator::S_CreatePanelHook,
		//	(AEGP_CreatePanelRefcon)this,
		//	true));
	}

	void CommandHook(
		AEGP_Command		command,				/* >> */
		AEGP_HookPriority	hook_priority,			/* >> currently always AEGP_HP_BeforeAE */
		A_Boolean			already_handledB,		/* >> */
		A_Boolean* handledPB)				/* << whether you handled */
	{
		if (command == i_command) {
			PT_ETX(i_ps->AEGP_ToggleVisibility(i_match_nameZ));
		}
	}

	void UpdateMenuHook(
		AEGP_WindowType			active_window)		/* >> */
	{
		PT_ETX(i_sp.CommandSuite1()->AEGP_EnableCommand(i_command));

		A_Boolean	out_thumb_is_shownB = FALSE, out_panel_is_frontmostB = FALSE;

		PT_ETX(i_ps->AEGP_IsShown(i_match_nameZ, &out_thumb_is_shownB, &out_panel_is_frontmostB));
		PT_ETX(i_sp.CommandSuite1()->AEGP_CheckMarkMenuCommand(i_command, out_thumb_is_shownB && out_panel_is_frontmostB));
	}

	void CreatePanelHook(
		AEGP_PlatformViewRef container,
		AEGP_PanelH		panelH,
		AEGP_PanelFunctions1* outFunctionTable,
		AEGP_PanelRefcon* outRefcon)
	{
		*outRefcon = reinterpret_cast<AEGP_PanelRefcon>(new PanelatorUI_Plat(i_pica_basicP, panelH, container, outFunctionTable));
	}
};


A_Err EntryPointFunc1(
	struct SPBasicSuite* pica_basicP,			/* >> */
	A_long				 	major_versionL,			/* >> */
	A_long					minor_versionL,			/* >> */
	AEGP_PluginID			aegp_plugin_id,			/* >> */
	AEGP_GlobalRefcon* global_refconP)		/* << */
{

	
	PT_XTE_START{
		*global_refconP = (AEGP_GlobalRefcon) new Panelator(pica_basicP, aegp_plugin_id);
	} PT_XTE_CATCH_RETURN_ERR;

	

}


void OnMouseWheel(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "滚轮 其他按键状态:%d 滚轮移动:%d x:%d y:%d\n", LOWORD(wParam), HIWORD(wParam), LOWORD(lParam), HIWORD(lParam)); // 120
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL);
}

void OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	char szText[256] = { 0 };
	sprintf_s(szText, "左键按下 其他按键状态:%d x:%d y:%d\n", wParam, LOWORD(lParam), HIWORD(lParam));
	WriteConsole(g_hOutput, szText, strlen(szText), NULL, NULL); // 左键按下 其他按键状态:9 x:343 y:244  (这个9 包括按下左键本身, 原理是其他按键+左键(1)) 可以用抬起测试具体按键数值 
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
int CALLBACK EntryPointFunc(HINSTANCE hIns, HINSTANCE hPreIns, LPSTR IpCmdLine, int nCmdShow) {

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