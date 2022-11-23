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

/*	Skeleton.cpp

	This is a compiling husk of a project. Fill it in with interesting
	pixel processing code.

	Revision History

	Version		Change													Engineer	Date
	=======		======													========	======
	1.0			(seemed like a good idea at the time)					bbb			6/1/2002

	1.0			Okay, I'm leaving the version at 1.0,					bbb			2/15/2006
				for obvious reasons; you're going to
				copy these files directly! This is the
				first XCode version, though.

	1.0			Let's simplify this barebones sample					zal			11/11/2010

	1.0			Added new entry point									zal			9/18/2017

*/

#include "Skeleton.h"

static PF_Err
About(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);

	suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
		"%s v%d.%d\r%s",
		STR(StrID_Name),
		MAJOR_VERSION,
		MINOR_VERSION,
		STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err
GlobalSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	out_data->my_version = PF_VERSION(MAJOR_VERSION,
		MINOR_VERSION,
		BUG_VERSION,
		STAGE_VERSION,
		BUILD_VERSION);

	out_data->out_flags = PF_OutFlag_DEEP_COLOR_AWARE | PF_OutFlag_I_DO_DIALOG;	// just 16bpc, not 32bpc

	out_data->out_flags2 = PF_OutFlag2_SUPPORTS_THREADED_RENDERING;

	return PF_Err_NONE;
}

static PF_Err
ParamsSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	PF_Err		err = PF_Err_NONE;
	PF_ParamDef	def;

	AEFX_CLR_STRUCT(def);

	PF_ADD_ANGLE(
		"Angle Name",	// 控件名称
		180,			// 默认角度
		ANGLE_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_BUTTON(
		"A Button",		// 控件名称
		"Click",		// 按钮名称
		NULL,			// PUI_FLAGS
		NULL,			// PARAM_FLAGS
		BUTTON_DISK_ID);



	AEFX_CLR_STRUCT(def);

	// PF_ADD_CHECKBOXX 同下,只不过复选框名称为空
	PF_ADD_CHECKBOX(
		"Left",			// 控件名称
		"Right",		// 复选框名称
		TRUE,			// 默认值
		NULL,			// Flags
		CHECKBOX_DISK_ID);


	AEFX_CLR_STRUCT(def);

	PF_ADD_COLOR(
		"Colour Name",	// 控件名称
		255, 0, 0,		// 默认RGB
		COLOR_DISK_ID);


	// PF_ADD_SLIDER 同下 只不过范围是整数

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(
		"Slider Name",			//  控件名称
		0, 100.0,				// 限制范围
		0, 100.0,				// 滑块范围
		50.0,					// 默认值
		PF_Precision_INTEGER,	// 单位
		NULL,					// DISPLAY FLAG
		NULL,					// FLAGS
		SLIDER_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_LAYER(
		"Layer Name",	// 控件名称
		1,				// 默认值
		LAYER_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_POINT(
		"Point Name",	// 控件名称
		50, 50,			// 默认值(当前图层宽高百分比)
		NULL,			// 限制src大小边界 
		POINT_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_POINT_3D(
		"Point 3D Name", // 控件名称
		50, 50, 0,		// 默认值(当前图层宽高深百分比)
		POINT3D_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_TOPIC(
		"Topic Name",
		TOPIC_START_DISK_ID);

	PF_ADD_FLOAT_SLIDERX(
		"Topic Slider", 0, 100, 0, 100, 50, PF_Precision_INTEGER, NULL, NULL, TOPIC_SLIDER_DISK_ID);

	PF_END_TOPIC(TOPIC_END_DISK_ID);


	AEFX_CLR_STRUCT(def);

	PF_ADD_POPUP(
		"Popup",
		3,
		1,
		"A|B|(-|C",
		POPUP_DISK_ID);


	out_data->num_params = SKELETON_NUM_PARAMS;

	return err;
}

static PF_Err
MySimpleGainFunc16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err		err = PF_Err_NONE;

	GainInfo* giP = reinterpret_cast<GainInfo*>(refcon);
	PF_FpLong	tempF = 0;

	if (giP) {
		tempF = giP->gainF * PF_MAX_CHAN16 / 100.0;
		if (tempF > PF_MAX_CHAN16) {
			tempF = PF_MAX_CHAN16;
		};

		outP->alpha = inP->alpha;
		outP->red = MIN((inP->red + (A_u_char)tempF), PF_MAX_CHAN16);
		outP->green = MIN((inP->green + (A_u_char)tempF), PF_MAX_CHAN16);
		outP->blue = MIN((inP->blue + (A_u_char)tempF), PF_MAX_CHAN16);
	}

	return err;
}

static PF_Err
MySimpleGainFunc8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err		err = PF_Err_NONE;

	GainInfo* giP = reinterpret_cast<GainInfo*>(refcon);
	PF_FpLong	tempF = 0;

	if (giP) {
		tempF = giP->gainF * PF_MAX_CHAN8 / 100.0;
		if (tempF > PF_MAX_CHAN8) {
			tempF = PF_MAX_CHAN8;
		};

		outP->alpha = inP->alpha;
		outP->red = MIN((inP->red + (A_u_char)tempF), PF_MAX_CHAN8);
		outP->green = MIN((inP->green + (A_u_char)tempF), PF_MAX_CHAN8);
		outP->blue = MIN((inP->blue + (A_u_char)tempF), PF_MAX_CHAN8);
	}

	return err;
}

static PF_Err
Render(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	PF_Err				err = PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);

	/*	Put interesting code here. */
	GainInfo			giP;
	AEFX_CLR_STRUCT(giP);
	A_long				linesL = 0;

	linesL = output->extent_hint.bottom - output->extent_hint.top;


	if (PF_WORLD_IS_DEEP(output)) {
		ERR(suites.Iterate16Suite1()->iterate(in_data,
			0,								// progress base
			linesL,							// progress final
			&params[SKELETON_INPUT]->u.ld,	// src 
			NULL,							// area - null for all pixels
			(void*)&giP,					// refcon - your custom data pointer
			MySimpleGainFunc16,				// pixel function pointer
			output));
	}
	else {
		ERR(suites.Iterate8Suite1()->iterate(in_data,
			0,								// progress base
			linesL,							// progress final
			&params[SKELETON_INPUT]->u.ld,	// src 
			NULL,							// area - null for all pixels
			(void*)&giP,					// refcon - your custom data pointer
			MySimpleGainFunc8,				// pixel function pointer
			output));
	}

	return err;
}


extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite * inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		"Skeleton", // Name
		"ADBE Skeleton", // Match Name
		"Sample Plug-ins", // Category
		AE_RESERVED_INFO); // Reserved Info

	return result;
}


PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output,
	void* extra)
{
	PF_Err		err = PF_Err_NONE;



	SHORT keyDown = ((GetKeyState(VK_LCONTROL) & 0x8000) | (GetKeyState(VK_RCONTROL) & 0x8000)) && !(GetKeyState(VK_RMENU) & 0x8000);

	if (keyDown != 0)

		"Control Key pressed!"; 

	try {
		switch (cmd) {
		case PF_Cmd_ABOUT:

			err = About(in_data,
				out_data,
				params,
				output);
			break;

		case PF_Cmd_GLOBAL_SETUP:

			err = GlobalSetup(in_data,
				out_data,
				params,
				output);
			break;

		case PF_Cmd_PARAMS_SETUP:

			err = ParamsSetup(in_data,
				out_data,
				params,
				output);
			break;

		case PF_Cmd_RENDER:

			err = Render(in_data,
				out_data,
				params,
				output);
			break;

		case PF_Cmd_DO_DIALOG:
			int a = 1; // 任意你的代码
			break;
		}
	}
	catch (PF_Err& thrown_err) {
		err = thrown_err;
	}
	return err;
}

