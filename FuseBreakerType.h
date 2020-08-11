/*****************************************************************************
 * FILE NAME    : FuseBreakerType.h
 * DATE         : April 13 2020
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
#ifndef _fusebreakertype_h_
#define _fusebreakertype_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "String.h"
#include "json.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Type : FuseBreakerType
 *****************************************************************************/
struct _FuseBreakerType
{
  string                                name;
  int                                   id;
  int                                   serialNumber;
  string                                type;
  string				fuseBreaker;
  int                                   rating;
  int                                   requiredPositions;
  StringList*                           usedInBays;
  struct _FuseBreakerType*              next;
};
typedef struct _FuseBreakerType FuseBreakerType;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
FuseBreakerTypeListDestroy
(FuseBreakerType* InType);

FuseBreakerType*
FuseBreakerTypeFindByName
(FuseBreakerType* InType, string InName);

FuseBreakerType*
FuseBreakerTypeFindBySerialNumber
(FuseBreakerType* InType, int InSerialNumber);

FuseBreakerType*
FuseBreakerTypeFindByID
(FuseBreakerType* InType, int InID);

void
FuseBreakerTypeAppend
(FuseBreakerType* InHead, FuseBreakerType* InType);

FuseBreakerType*
FuseBreakerTypeFromJSONString
(json_value* InValue);

void
FuseBreakerTypeDestroy
(FuseBreakerType* InType);

FuseBreakerType*
FuseBreakerTypeCreate
();

StringList*
FuseBreakerTypeGetFusesForPanel
(FuseBreakerType* InHead, string InPanelList);

StringList*
FuseBreakerTypeGetBreakersForPanel
(FuseBreakerType* InHead, string InPanelList);

StringList*
FuseBreakerTypeGetTypeForPanel
(FuseBreakerType* InHead, string InType, string InPanelList);

#endif /* _fusebreakertype_h_*/
