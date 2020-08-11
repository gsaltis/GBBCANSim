/*****************************************************************************
 * FILE NAME    : BayType.h
 * DATE         : April 15 2020
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
#ifndef _baytype_h_
#define _baytype_h_

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

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Type : BayType
 *****************************************************************************/
struct _BayType
{
  string                                name;
  string                                listNumber;
  int                                   panelCount;
  struct _BayType*                      next;
};
typedef struct _BayType BayType;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
string
BayTypeToJSONString
(BayType* InBayType);

string
BayTypesToJSONString
(BayType* InBayType);

BayType*
BayTypeFromJSONString
(json_value* InValue);

void
BayTypesDestroy
(BayType* InType);

void
BayTypeAppend
(BayType* InHead, BayType* InType);

BayType*
BayTypeFindByName
(BayType* InType, string InName);

void
BarTypeDestroy
(BayType* InType);

BayType*
BayTypeCreate
();

StringList*
BayTypeGetNamesList
(BayType* InType);

#endif /* _baytype_h_*/
