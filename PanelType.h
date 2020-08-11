/*****************************************************************************
 * FILE NAME    : PanelType.h
 * DATE         : April 15 2020
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
#ifndef _paneltype_h_
#define _paneltype_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <json.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "String.h"
#include "JSONIF.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Type : PanelType
 *****************************************************************************/
struct _PanelType
{
  string                                name;
  string                                listNumber;
  string				usedin;
  int                                   maxPositions;
  struct _PanelType*                    next;
  bool					usesSMDUH2;
};
typedef struct _PanelType PanelType;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
string
PanelTypeToJSONString
(PanelType* InType);

string
PanelTypesToJSONString
(PanelType* InType);

void
PanelTypesDestroy
(PanelType* InHead);

PanelType*
PanelTypeFromJSONString
(json_value* InValue);

PanelType*
PanelTypeFindByName
(PanelType* InHead, string InName);

PanelType*
PanelTypeFindByListNumber
(PanelType* InHead, string InListNumber);

void
PanelTypeAppend
(PanelType* InHead, PanelType* InPanelType);

void
PanelTypeDestroy
(PanelType* InPanelType);

PanelType*
PanelTypeCreate
();

StringList*
PanelTypeGetNamesList
(PanelType* InType);

#endif /* _paneltype_h_*/

