/*****************************************************************************
 * FILE NAME    : PanelType.c
 * DATE         : April 15 2020
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
#include "ascii.h"
#include "PanelType.h"

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
 * Function : PanelTypeCreate
 *****************************************************************************/
PanelType*
PanelTypeCreate
()
{
  PanelType*                            type;
  type = (PanelType*)GetMemory(sizeof(PanelType));
  memset(type, NUL, sizeof(PanelType));
  return type;
}

/*****************************************************************************!
 * Function : PanelTypeDestroy
 *****************************************************************************/
void
PanelTypeDestroy
(PanelType* InPanelType)
{
  if ( NULL == InPanelType ) {
    return;
  }
  if ( InPanelType->name ) {
    FreeMemory(InPanelType->name);
  }

  if ( InPanelType->listNumber ) {
    FreeMemory(InPanelType->listNumber);
  }

  if ( InPanelType->usedin ) {
    FreeMemory(InPanelType->usedin);
  }
  FreeMemory(InPanelType);
}

/*****************************************************************************!
 * Function : PanelTypeAppend
 *****************************************************************************/
void
PanelTypeAppend
(PanelType* InHead, PanelType* InPanelType)
{
  PanelType*                            type;
  if ( NULL == InHead || NULL == InPanelType ) {
    return;
  }

  type = InHead;
  while ( type->next ) {
    type = type->next;
  }
  type->next = InPanelType;
}

/*****************************************************************************!
 * Function : PanelTypeFindByListNumber
 *****************************************************************************/
PanelType*
PanelTypeFindByListNumber
(PanelType* InHead, string InListNumber)
{
  PanelType*                            type;
  if ( NULL == InHead || NULL == InListNumber ) {
    return NULL;
  }

  for (type = InHead; type ; type = type->next ) {
    if ( StringEqual(type->listNumber, InListNumber) ) {
      return type;
    }
  }
  return NULL;
}


/*****************************************************************************!
 * Function : PanelTypeFindByName
 *****************************************************************************/
PanelType*
PanelTypeFindByName
(PanelType* InHead, string InName)
{
  PanelType*                            type;
  if ( NULL == InHead || NULL == InName ) {
    return NULL;
  }

  for (type = InHead; type ; type = type->next ) {
    if ( StringEqual(type->name, InName) ) {
      return type;
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : PanelTypeFromJSONString
 *****************************************************************************/
PanelType*
PanelTypeFromJSONString
(json_value* InValue)
{
  PanelType*                            type;
  if ( NULL == InValue ) {
    return NULL;
  }

  type = PanelTypeCreate();

  type->name = JSONIFGetString(InValue, "name");
  type->listNumber = JSONIFGetString(InValue, "listnumber");
  type->maxPositions = JSONIFGetInt(InValue, "maxpositions");
  type->usedin = JSONIFGetString(InValue, "usedin");
  type->usesSMDUH2 = JSONIFGetBool(InValue, "usesSMDUH2");
  return type;
}

/*****************************************************************************!
 * Function : PanelTypesDestroy
 *****************************************************************************/
void
PanelTypesDestroy
(PanelType* InHead)
{
  PanelType*                            typeNext;
  PanelType*                            type;
  if ( NULL == InHead ) {
    return;
  }

  type = InHead;
  while ( type ) {
    typeNext = type->next;
    PanelTypeDestroy(type);
    type = typeNext;
  }
}

/*****************************************************************************!
 * Function : PanelTypeToJSONString
 *****************************************************************************/
string
PanelTypeToJSONString
(PanelType* InType)
{
  string				returnString;
  char					s[8];

  if ( NULL == InType ) {
    return NULL;
  }
  returnString = StringCopy("{ \"name\" : \"");
  returnString = StringConcatTo(returnString, InType->name);
  returnString = StringConcatTo(returnString, "\", \"listnumber\" : \"");
  returnString = StringConcatTo(returnString, InType->listNumber);
  returnString = StringConcatTo(returnString, "\", \"maxpositions\" : ");
  sprintf(s, "%d", InType->maxPositions);
  returnString = StringConcatTo(returnString, s);
  returnString = StringConcatTo(returnString, ", \"usesSMDUH2\" : ");
  returnString = StringConcatTo(returnString, InType->usesSMDUH2 ? "true" : "false");
  returnString = StringConcatTo(returnString, ", \"usedin\" : \"");
  returnString = StringConcatTo(returnString, InType->usedin);
  returnString = StringConcatTo(returnString, "\"");
  returnString = StringConcatTo(returnString, " }");
  
  return returnString;
}

/*****************************************************************************!
 * Function : PanelTypesToJSONString 
 *****************************************************************************/
string
PanelTypesToJSONString
(PanelType* InType)
{
  PanelType*				type;
  string				returnString;
  string				s;

  if ( NULL == InType ) {
    return NULL;
  }
  returnString = StringCopy("[ ");

  for ( type = InType; type; type = type->next ) {
    s = PanelTypeToJSONString(type);
    returnString = StringConcatTo(returnString, s);
    FreeMemory(s);
    if ( type->next ) {
      returnString = StringConcatTo(returnString, ", ");
    }
  }
  returnString = StringConcatTo(returnString, " ]");
  return returnString;
}

/*****************************************************************************!
 * Function : PanelTypeGetNamesList
 *****************************************************************************/
StringList*
PanelTypeGetNamesList
(PanelType* InType)
{
  StringList*				typeList;
  PanelType*				type;

  if ( NULL == InType ) {
    return NULL;
  }
  typeList = StringListCreate();

  for (type = InType; type; type = type->next) {
    StringListAppend(typeList, StringCopy(type->name));    
  }
  return typeList;
}

