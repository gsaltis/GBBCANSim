/*****************************************************************************
 * FILE NAME    : BayType.c
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
#include "BayType.h"
#include "MemoryManager.h"
#include "JSONIF.h"
#include "ascii.h"

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
 * Function : BayTypeCreate
 *****************************************************************************/
BayType*
BayTypeCreate
()
{
  int                                   n;
  BayType*                              type;

  n = sizeof(BayType);
  type = (BayType*)GetMemory(n);
  memset(type, NUL, n);

  return type;
}

/*****************************************************************************!
 * Function : BayTypeDestroy
 *****************************************************************************/
void
BayTypeDestroy
(BayType* InType)
{
  if ( NULL == InType ) {
    return;
  }
  if ( InType->name ) {
    FreeMemory(InType->name);
  }
  if ( InType->listNumber ) {
    FreeMemory(InType->listNumber);
  }
  FreeMemory(InType);
}

/*****************************************************************************!
 * Function : BayTypeFindByName
 *****************************************************************************/
BayType*
BayTypeFindByName
(BayType* InType, string InName)
{
  BayType*                              type;
  if ( NULL == InType || NULL == InName ) {
    return NULL;
  }
  type = InType;
  while (type) {
    if ( StringEqual(type->name, InName) ) {
      return type;
    }
    type = type->next;
  }
  return NULL;
}

/*****************************************************************************!
 * Function : BayTypeAppend
 *****************************************************************************/
void
BayTypeAppend
(BayType* InHead, BayType* InType)
{
  BayType*                              type;
  if ( NULL == InHead || NULL == InType ) {
    return;
  }

  type = InHead;
  while (type) {
    if ( type->next == NULL ) {
      type->next = InType;
      return;
    }
  }
}

/*****************************************************************************!
 * Function : BayTypesDestroy
 *****************************************************************************/
void
BayTypesDestroy
(BayType* InType)
{
  BayType*                              typeNext;
  BayType*                              type;
  if ( NULL == InType ) {
    return;
  }
  
  type = InType;

  while (type) {
    typeNext = type->next;
    BayTypeDestroy(type);
    type = typeNext;
  }
}

/*****************************************************************************!
 * Function : BayTypeFromJSONString
 *****************************************************************************/
BayType*
BayTypeFromJSONString
(json_value* InValue)
{
  BayType*                              type;
  if ( NULL == InValue ) {
    return NULL;
  }

  type = BayTypeCreate();
  type->name = JSONIFGetString(InValue, "name");
  type->listNumber = JSONIFGetString(InValue, "listnumber");
  type->panelCount = JSONIFGetInt(InValue, "panelcount");
  return type;
}


/*****************************************************************************!
 * Function : BayTypeToJSONString
 *****************************************************************************/
string
BayTypeToJSONString
(BayType* InBayType)
{
  string				returnString;
  char					s[8];

  if ( InBayType == NULL ) {
    return NULL;
  }

  returnString = StringCopy("{ \"name\" : \"");
  returnString = StringConcatTo(returnString, InBayType->name);
  returnString = StringConcatTo(returnString, "\", \"listnumber\" : \"");
  returnString = StringConcatTo(returnString, InBayType->listNumber);
  sprintf(s, "%d", InBayType->panelCount);
  
  returnString = StringConcatTo(returnString, "\", \"panelcount\" : ");
  returnString = StringConcatTo(returnString, s);
  returnString = StringConcatTo(returnString, " }");
 
  return returnString;
}

/*****************************************************************************!
 * Function : BayTypesToJSONString
 *****************************************************************************/
string
BayTypesToJSONString
(BayType* InBayType)
{
  string                               	returnString;
  BayType*				type;
  if ( InBayType == NULL ) {
    return NULL;
  }

  returnString = StringCopy("[ ");

  for (type = InBayType; type; type = type->next) {
    returnString = StringConcatTo(returnString, BayTypeToJSONString(type));
    if ( type->next ) {
      returnString = StringConcatTo(returnString, ", ");
    }
  }
  returnString = StringConcatTo(returnString, " ]");
  return returnString;
}

/*****************************************************************************!
 * Function : BayTypeGetNamesList
 *****************************************************************************/
StringList*
BayTypeGetNamesList
(BayType* InBayType)
{
  StringList*				returnList;
  BayType*				type;

  returnList = StringListCreate();
  
  for ( type = InBayType; type; type = type->next ) {
    StringListAppend(returnList, StringCopy(type->name));
  }
  return returnList;
}

