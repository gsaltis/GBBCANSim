/*****************************************************************************
 * FILE NAME    : FuseBreakerType.c
 * DATE         : April 13 2020
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/

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
#include "MemoryManager.h"
#include "String.h"
#include "ascii.h"
#include "FuseBreakerType.h"
#include "JSONIF.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/

/*****************************************************************************!
 * Function : FuseBreakerTypeCreate
 *****************************************************************************/
FuseBreakerType*
FuseBreakerTypeCreate
()
{
  int                                   n;
  FuseBreakerType*                      type;

  n = sizeof(FuseBreakerType);
  type = (FuseBreakerType*)GetMemory(n);
  memset(type, NUL, n);
  return type;
}

/*****************************************************************************!
 * Function : FuseBreakerTypeDestroy
 *****************************************************************************/
void
FuseBreakerTypeDestroy
(FuseBreakerType* InType)
{
  if ( NULL == InType ) {
    return;
  }
  if ( InType->name ) {
    FreeMemory(InType->name);
  }

  FreeMemory(InType);
}

/*****************************************************************************!
 * Function : FuseBreakerTypeFromJSONString
 *****************************************************************************/
FuseBreakerType*
FuseBreakerTypeFromJSONString
(json_value* InValue)
{
  FuseBreakerType*                      type;
  if ( NULL == InValue ) {
    return NULL;
  }

  type = FuseBreakerTypeCreate();

  type->name = JSONIFGetString(InValue, "name");
  type->fuseBreaker = JSONIFGetString(InValue, "fusebreaker");
  type->id = JSONIFGetInt(InValue, "id");
  type->type = JSONIFGetString(InValue, "type");
  type->rating = JSONIFGetInt(InValue, "rating");
  type->serialNumber = JSONIFGetInt(InValue, "serialnumber");
  type->requiredPositions = JSONIFGetInt(InValue, "requiredpositions");
  type->usedInBays = JSONIFGetStringList(InValue, "usedin");
  return type;
}


/*****************************************************************************!
 * Function : FuseBreakerTypeAppend
 *****************************************************************************/
void
FuseBreakerTypeAppend
(FuseBreakerType* InHead, FuseBreakerType* InType)
{
  FuseBreakerType*                      type;
  if ( NULL == InHead || NULL == InType ) {
    return;
  }
  type = InHead;
  while (type->next) {
    type = type->next;
  }
  type->next = InType;
}

/*****************************************************************************!
 * Function : FuseBreakerTypeFindByID
 *****************************************************************************/
FuseBreakerType*
FuseBreakerTypeFindByID
(FuseBreakerType* InType, int InID)
{
  FuseBreakerType*                      type;
  if ( NULL == InType ) {
    return NULL;
  }

  for ( type = InType; type && type->id != InID; type = type->next )
  {}

  return type;
}

/*****************************************************************************!
 * Function : FuseBreakerTypeFindByName
 *****************************************************************************/
FuseBreakerType*
FuseBreakerTypeFindByName
(FuseBreakerType* InType, string InName)
{
  FuseBreakerType*                      type;
  if ( NULL == InType ) {
    return NULL;
  }

  for ( type = InType; type && !StringEqual(type->name, InName); type = type->next )
  {}

  return type;  
}


/*****************************************************************************!
 * Function : FuseBreakerTypeListDestroy
 *****************************************************************************/
void
FuseBreakerTypeListDestroy
(FuseBreakerType* InType)
{
  FuseBreakerType*                      typeNext;
  FuseBreakerType*                      type;
  if ( NULL == InType ) {
    return;
  }

  type = InType;

  while (type) {
    typeNext = type->next;
    FuseBreakerTypeDestroy(type);
    type = typeNext;
  }
}

/*****************************************************************************!
 * Function : FuseBreakerTypeGetFusesForPanel
 *****************************************************************************/
StringList*
FuseBreakerTypeGetFusesForPanel
(FuseBreakerType* InHead, string InPanelList)
{
  return FuseBreakerTypeGetTypeForPanel(InHead, "fuse", InPanelList);
}

/*****************************************************************************!
 * Function : FuseBreakerTypeGetBreakersForPanel
 *****************************************************************************/
StringList*
FuseBreakerTypeGetBreakersForPanel
(FuseBreakerType* InHead, string InPanelList)
{
  return FuseBreakerTypeGetTypeForPanel(InHead, "breaker", InPanelList);
}

/*****************************************************************************!
 * Function : FuseBreakerTypeGetTypeForPanel
 *****************************************************************************/
StringList*
FuseBreakerTypeGetTypeForPanel
(FuseBreakerType* InHead, string InType, string InPanelList)
{
  StringList*				breakers;
  FuseBreakerType*			type;
  string				breaker;
  
  if ( InHead == NULL || InPanelList == NULL ) {
    return NULL;
  }

  breakers = StringListCreate();
  for ( type = InHead ; type ; type = type->next ) {
    if ( !StringEqualNoCase(type->fuseBreaker, InType) ) {
      continue;
    }
    if ( StringListContains(type->usedInBays, InPanelList) ) {
      char				s3[32];
      sprintf(s3, "%d", type->serialNumber);
      breaker = StringMultiConcat(type->name, ":", type->type, ":", s3, NULL);
      StringListAppend(breakers, breaker);
    }
  }
  return breakers;
}

/*****************************************************************************!
 * Function : FuseBreakerTypeFindBySerialNumber
 *****************************************************************************/
FuseBreakerType*
FuseBreakerTypeFindBySerialNumber
(FuseBreakerType* InHead, int InSerialNumber)
{
  FuseBreakerType*			type;

  if ( InHead == NULL ) {
    return NULL;
  }

  for ( type = InHead ; type ; type = type->next ) {
    if ( type->serialNumber == InSerialNumber ) {
      return type;
    }
  }
  return NULL;
}

