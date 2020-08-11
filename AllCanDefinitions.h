/*******************************************************************************
 * FILE 	: AllCanDefinitions.h
 * DATE		: April 26 2018
 * PROJECT	:
 * COPYRIGHT	: Copyright (c) 2018 Vertiv Company
 *******************************************************************************/
#ifndef _allcandefinitions_h_
#define _allcandefinitions_h_

/*******************************************************************************
 * Local Headers
 *******************************************************************************/
#include "CanMsg.h"

/*******************************************************************************
 * Exported Data
 *******************************************************************************/
extern CanRegDef RegDefList_SMDUE[];
extern CanMsgDef MsgDefList_SMDUE[];
extern CanRegDef RegDefList_SMDUH2[];
extern CanMsgDef MsgDefList_SMDUH2[];
extern CanRegDef RegDefList_SMTEMP[];
extern CanMsgDef MsgDefList_SMTEMP[];
extern CanRegDef RegDefList_Rect[];
extern CanMsgDef MsgDefList_Rect[];
extern CanRegDef RegDefList_Rect12[];
extern CanMsgDef MsgDefList_Rect12[]; 
extern CanRegDef RegDefList_BBU[];
extern CanMsgDef MsgDefList_BBU[];
// extern CanDeviceDef CanDeviceDefList[];

extern int NumRegDefList_SMDUE;
extern int NumMsgDefList_SMDUE;
extern int NumRegDefList_SMDUH2;
extern int NumMsgDefList_SMDUH2;
extern int NumRegDefList_SMTEMP;
extern int NumMsgDefList_SMTEMP;
extern int NumRegDefList_Rect;
extern int NumMsgDefList_Rect;
extern int NumRegDefList_Rect12;
extern int NumMsgDefList_Rect12;
extern int NumRegDefList_BBU;
extern int NumMsgDefList_BBU;
extern int NumDeviceDefs;


/*******************************************************************************
 * Exported Functions
 *******************************************************************************/
void
CanDefinitionsInitialize();

#endif
