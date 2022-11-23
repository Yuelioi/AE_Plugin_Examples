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

/*
	Skeleton.h
*/

#pragma once

#ifndef SKELETON_H
#define SKELETON_H

typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned short		u_int16;
typedef unsigned long		u_long;
typedef short int			int16;
#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096

#define PF_DEEP_COLOR_AWARE 1	// make sure we get 16bpc pixels; 
								// AE_Effect.h checks for this.

#include "AEConfig.h"

#ifdef AE_OS_WIN
	typedef unsigned short PixelType;
	#include <Windows.h>
#endif

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "AE_EffectCBSuites.h"
#include "String_Utils.h"
#include "AE_GeneralPlug.h"
#include "AEFX_ChannelDepthTpl.h"
#include "AEGP_SuiteHandler.h"

#include "Skeleton_Strings.h"

/* Versioning information */

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1


/* Parameter defaults */

#define	SKELETON_GAIN_MIN		0
#define	SKELETON_GAIN_MAX		100
#define	SKELETON_GAIN_DFLT		10

enum {
	SKELETON_INPUT = 0,
	SKELETON_ANGLE,
	SKELETON_BUTTON,
	SKELETON_CHECKBOX,
	SKELETON_COLOR,
	SKELETON_SLIDER,
	SKELETON_LAYER,
	SKELETON_POINT,
	SKELETON_POINT3D,
	SKELETON_TOPIC_START,
	SKELETON_TOPIC_SLIDER,
	SKELETON_TOPIC_END,
	SKELETON_POPUP,
	SKELETON_NUM_PARAMS
};

enum {
	ANGLE_DISK_ID = 1,
	BUTTON_DISK_ID,
	CHECKBOX_DISK_ID,
	COLOR_DISK_ID,
	SLIDER_DISK_ID,
	LAYER_DISK_ID,
	POINT_DISK_ID,
	POINT3D_DISK_ID,
	TOPIC_START_DISK_ID,
	TOPIC_SLIDER_DISK_ID,
	TOPIC_END_DISK_ID,
	POPUP_DISK_ID
};

typedef struct GainInfo{
	PF_FpLong	gainF;
} GainInfo, *GainInfoP, **GainInfoH;


extern "C" {

	DllExport
	PF_Err
	EffectMain(
		PF_Cmd			cmd,
		PF_InData		*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output,
		void			*extra);

}

#endif // SKELETON_H