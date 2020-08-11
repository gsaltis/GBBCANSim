/*****************************************************************************
 * FILE NAME    : Panel.h
 * DATE         : March 27 2020
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
#ifndef _panel_h_
#define _panel_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sqlite3.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "FuseBreaker.h"
#include "String.h"
#include "CanMsg.h"
#include "PanelType.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Type : Panel
 *****************************************************************************/
typedef struct _Panel
{
  uint32_t                              index;
  PanelType*				panelType;
  struct _Bay*                          bay;
  CanDevice*				smduh2;
} Panel;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
string
PanelValuesToJSONString
(Panel* InPanel);

void
PanelValuesFromJSONString
(Panel* InPanel, json_value* InRegisters);

string
PanelToJSONString
(Panel* InPanel);

Panel*
PanelFromJSONString
(json_value* InValue);

bool
PanelRemoveFuseBreakerByIndex
(Panel* InPanel,uint32_t InFuseBreakerIndex);

FuseBreaker*
PanelFindFuseBreaker
(Panel* InPanel,uint32_t InFuseBreakerIndex);

void
PanelAddFuseBreaker
(Panel* InPanel,FuseBreaker* InFuseBreaker);

Panel*
PanelCreate
(uint32_t InIndex, PanelType* InPanelType);

void
PanelDestroy
(Panel* InPanel);

bool
PanelFuseBreakerSlotOccupied
(Panel* InPanel, int InFuseBreakerIndex);

FuseBreaker*
PanelFirstFuseBreaker
(Panel* InPanel);

FuseBreaker*
PanelNextFuseBreaker
(Panel* InPanel, FuseBreaker* InFuseBreaker);

void
PanelDisplay
(Panel* InPanel, int InIndent, bool InAllInformation);

string
PanelGetFuseBreakerList
(Panel* InPanel);

void
PanelSetEmpty
(Panel* InPanel);

string
PanelRegValuesToJSONString
(Panel* InPanel);

CanReg*
PanelFindCANRegister
(Panel* InPanel, int InValueType);

void
PanelSaveCANRegister
(Panel* InPanel, int InValueType, ufloatbit32_t* InValue, sqlite3* InDatabase);

void
PanelSaveValuesSQL
(Panel* InPanel);

void
PanelSaveRegisterValueSQL
(Panel* InPanel, CanReg* InReg);

#endif /* _panel_h_*/
