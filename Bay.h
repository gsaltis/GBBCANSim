/*****************************************************************************
 * FILE NAME    : Bay.h
 * DATE         : March 27 2020
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
#ifndef _bay_h_
#define _bay_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "String.h"
#include "Panel.h"
#include "json.h"
#include "BayType.h"
#include "main.h"
#include "CanMsg.h"
#include "PanelConnection.h"
#include "Rectifier.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/
#define BAY_MAX_INDEX                   8

/*****************************************************************************!
 * Exported Type : Bay
 *****************************************************************************/
typedef struct _Bay
{
  string                                name;
  int                                   index;
  string                                ipaddress;
  uint32_t                              portNumber;
  BayType*                              type;
  Panel**                               panels;
  string 				listNumber;
  int                                   panelCount;
  CanDevice*				canDevice;
  PanelConnection*			panelConnections;
  CanReg**				canRegisters;
  Rectifier*				rectifiers;
} Bay;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
BaysValueFromJSONString
(json_value* InValue);

void
BayValueFromJSONString
(Bay* InBay, json_value* InValue);

CanReg*
BayFindCANRegister
(Bay* InBay, int InValueType);

string
BaysValueToJSONString
();

string
BayValueToJSONString
(Bay* InBay);

bool
BayRemovePanelByIndex
(Bay* InBay, int InPanelIndex);

bool
BaysRemoveBayByIndex
(int InIndex);

bool
BayUpdateFromJSONString
(Bay* InBay, json_value* InValue);

bool
BayPanelSlotOccupied
(Bay* InBay, int InPanelIndex);

bool
BaysFromJSONString
(json_value* InValue);

Bay*
BayFromJSONString
(json_value* InValue);

string
BaysToJSONString
();

string
BayToJSONString
(Bay* InBay);

string
BayTypeToString
(BayType* InBayType);

void
BaysInitialize
(string InFilename);

Bay*
FindBayByIndex
(int InIndex);

void
BayNameSet
(Bay* InBay, string InName);

Panel*
BayFindPanelByIndex
(Bay* InBay,int InPanelIndex);

void
BayGetIPInfo
(Bay* InBay,string* InIPAddress, uint32_t* InPort);

void
BayAddPanel
(Bay* InBay,Panel* InPanel);

void
BaySetIPInfo
(Bay* InBay,string InIPAddress, uint32_t InPort);

void
BaySetName
(Bay* InBay,string InName);

void
BaySet
(Bay* InBay, string InName, string InIPAddress, uint32_t InPort);

void
BayDestroy
(Bay* InBay);

Bay*
BayCreate
(int InIndex, BayType* InBayType);

bool
BaysSave
(string InFilename);

bool
BaysSaveValues
(string InFilename);

Bay*
BaysFirstBay
();

Bay*
BaysNextBay
(Bay* InBay);

Panel*
BayFirstPanel
(Bay* InBay);

Panel*
BayNextPanel
(Bay* InBay, Panel* InPanel);

void
BayDisplay
(Bay* InBay, int InIndent, bool InAllInformation);

string
BaysGetIndexList
();

string
BayGetPanelIndexList
(Bay* InBay, bool InIncludePlainPanels);

PanelConnection*
BayFindPanelConnectionByIndices
(Bay* InBay, int InFromIndex, int InToIndex);

bool
BayVerifyNewPanelConnection
(Bay* InBay, int InFromIndex, int InToIndex);

void
BayAppendPanelConnection
(Bay* InBay, PanelConnection* InPanelConnection);

bool
BayRemovePanelConnection
(Bay* InBay, PanelConnection* InPanelConnection);

bool
BayRemovePanelConnectionsByIndex
(Bay* InBay, int InPanelIndex);

void
BayCreateEmptyPanels
(Bay* InBay);

string
BayRegValuesToJSONString
(Bay* InBay, bool InSavePanelRegs);

void
BaysSaveValuesSQL
();

void
BaySaveValuesSQL
(Bay* InBay);

void
BayAddRectifier
(Bay* InBay, Rectifier* InRectifier);

uint16_t
BayGetNextRectifierIndex
(Bay* InBay);

Rectifier*
BayFindRectifierByIndex
(Bay* InBay, uint16_t InIndex);

#endif /* _bay_h_*/
