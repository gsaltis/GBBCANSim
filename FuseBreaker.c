/*****************************************************************************
 * FILE NAME    : FuseBreaker.c
 * DATE         : March 27 2020
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
#include "FuseBreaker.h"
#include "MemoryManager.h"
#include "Panel.h"
#include "Bay.h"
#include "JSONIF.h"
#include "main.h"
#include "Devices.h"

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
 * Function : FuseBreakerDestroy
 *****************************************************************************/
void
FuseBreakerDestroy
(FuseBreaker* InFuseBreaker)
{
  if ( NULL == InFuseBreaker ) {
    return;
  }
  if ( InFuseBreaker->type ) {
    FreeMemory(InFuseBreaker->type);
  }
  if ( InFuseBreaker->typeClass ) {
    FreeMemory(InFuseBreaker->typeClass);
  }
  FreeMemory(InFuseBreaker);
}

/*****************************************************************************!
 * Function : FuseBreakerCreate
 *****************************************************************************/
FuseBreaker*
FuseBreakerCreate
(uint32_t InFuseBreakerIndex, string InType, string InTypeClass, int InRating, FuseBreakerState InState)
{
  FuseBreaker*                          fuseBreaker;
  int					n;
  n = sizeof(FuseBreaker);

  fuseBreaker = (FuseBreaker*)GetMemory(n);
  memset(fuseBreaker, 0x00, n);
  fuseBreaker->type = StringCopy(InType);
  fuseBreaker->typeClass = StringCopy(InTypeClass);
  fuseBreaker->index = InFuseBreakerIndex;
  fuseBreaker->rating = InRating;
  fuseBreaker->state = InState;

  return fuseBreaker;
}

/*****************************************************************************!
 * Function : FuseBreakerStateToString
 *****************************************************************************/
string
FuseBreakerStateToString
(FuseBreakerState InState)
{
  switch (InState) {
    case FuseBreakerStateOpen : {
      return "open";
    }
    case FuseBreakerStateClosed : {
      return "closed";
    }
    case FuseBreakerStateNone : {
      return "none";
      break;
    }
  }
  return "";
}


/*****************************************************************************!
 * Function : FuseBreakerToJSONString
 *****************************************************************************/
string
FuseBreakerToJSONString
(FuseBreaker* InFuseBreaker)
{
  string                                returnString;
  char                                  s[8];
  
  if ( NULL == InFuseBreaker ) {
    return NULL;
  }
  returnString = StringCopy("            {\n");

  returnString = StringConcatTo(returnString, "              \"fusebreakerindex\" : ");
  sprintf(s, "%d,\n", InFuseBreaker->index);
  returnString = StringConcatTo(returnString, s);

  returnString = StringConcatTo(returnString, "              \"panelindex\" : ");
  sprintf(s, "%d,\n", InFuseBreaker->panel->index);
  returnString = StringConcatTo(returnString, s);

  returnString = StringConcatTo(returnString, "              \"bayindex\" : ");
  sprintf(s, "%d,\n", InFuseBreaker->panel->bay->index);
  returnString = StringConcatTo(returnString, s);

  returnString = StringConcatTo(returnString, "              \"typeclass\" : \"");
  returnString = StringConcatTo(returnString, InFuseBreaker->typeClass);
  returnString = StringConcatTo(returnString, "\",\n");

  returnString = StringConcatTo(returnString, "              \"type\" : \"");
  returnString = StringConcatTo(returnString, InFuseBreaker->type);
  returnString = StringConcatTo(returnString, "\",\n");

  returnString = StringConcatTo(returnString, "              \"state\" : \"");
  returnString = StringConcatTo(returnString, FuseBreakerStateToString(InFuseBreaker->state));
  returnString = StringConcatTo(returnString, "\",\n");

  returnString = StringConcatTo(returnString, "              \"rating\" : ");
  sprintf(s, "%d,\n", InFuseBreaker->rating);
  returnString = StringConcatTo(returnString, s);

  returnString = StringConcatTo(returnString, "              \"current\" : ");
  sprintf(s, "%.03f\n", InFuseBreaker->current.fd);
  returnString = StringConcatTo(returnString, s);
  returnString = StringConcatTo(returnString, "            }");

  return returnString;
}

/*****************************************************************************!
 * Function : FuseBreakerFromJSONString
 *****************************************************************************/
FuseBreaker*
FuseBreakerFromJSONString
(json_value* InValue)
{
  string                                stateString;
  FuseBreakerState                      state;
  string                                typeString;
  string				typeClassString;
  int                                   rating;
  int                                   fusebreakerindex;
  FuseBreaker*				fusebreaker;
  float					f;

  if ( NULL == InValue ) {
    return NULL;
  }
  fusebreakerindex = JSONIFGetInt(InValue, "fusebreakerindex");
  rating = JSONIFGetInt(InValue, "rating");

  typeString = JSONIFGetString(InValue, "type");
  typeClassString = JSONIFGetString(InValue, "typeclass");

  stateString = JSONIFGetString(InValue, "state");
  state = FuseBreakerStateFromString(stateString);
  FreeMemory(stateString);
  f = JSONIFGetFloat(InValue, "current");
  
  fusebreaker = FuseBreakerCreate(fusebreakerindex, typeString, typeClassString, rating, state);
  FuseBreakerSetCurrent(fusebreaker, f);

  FreeMemory(typeString);
  FreeMemory(typeClassString);
  return fusebreaker;
}

/*****************************************************************************!
 * Function : FuseBreakerStateFromString
 *****************************************************************************/
FuseBreakerState
FuseBreakerStateFromString
(string InString)
{
  if ( StringEqual("closed", InString) ) {
    return FuseBreakerStateClosed;
  }
  if ( StringEqual("open", InString) ) {
    return FuseBreakerStateOpen;
  }
  return FuseBreakerStateNone;
}

/*****************************************************************************!
 * Function : FuseBreakerUpdateFromJSONString
 *****************************************************************************/
bool
FuseBreakerUpdateFromJSONString
(FuseBreaker* InFuseBreaker, json_value* InFuseBreakerInfo)
{
  FuseBreakerState                      state;
  bool                                  changed;
  string                                typeString;
  string                                stateString;
  int                                   rating;

  changed = false;
  if ( NULL == InFuseBreaker || NULL == InFuseBreakerInfo ) {
    return false;
  }
  typeString    = JSONIFGetString(InFuseBreakerInfo, "type");
  rating        = JSONIFGetInt(InFuseBreakerInfo, "rating");
  stateString   = JSONIFGetString(InFuseBreakerInfo, "state");

  if ( typeString ) {
    InFuseBreaker->type = StringCopy(typeString);
    changed = true;
    FreeMemory(typeString);
  }

  if ( stateString ) {
    state = FuseBreakerStateFromString(stateString);
    if ( state != FuseBreakerStateNone ) {
      InFuseBreaker->state = state;
      changed =true;
    }
    FreeMemory(stateString);
  }

  if ( rating != 0 ) {
    InFuseBreaker->rating = rating;
    changed = true;
  }
  return changed;
}

/*****************************************************************************!
 * Function : FuseBreakerValueToJSONString
 *****************************************************************************/
string
FuseBreakerValueToJSONString
(FuseBreaker* InFuseBreaker)
{
  char                                  s[17];
  string                                returnValue;
  float					value;

  if ( NULL == InFuseBreaker ) {
    return NULL;
  }
  
  if ( InFuseBreaker->canRegister ) {
    value = InFuseBreaker->canRegister->Value.fd;
  } else {
    value = 0.0;
  }
  returnValue = StringCopy("            { ");

  returnValue = StringConcatTo(returnValue, "\"bayindex\" : ");
  sprintf(s, "%d, ", InFuseBreaker->panel->bay->index);
  returnValue = StringConcatTo(returnValue, s);
  
  returnValue = StringConcatTo(returnValue, "\"panelindex\" : ");
  sprintf(s, "%d, ", InFuseBreaker->panel->index);
  returnValue = StringConcatTo(returnValue, s);
  

  returnValue = StringConcatTo(returnValue, "\"fusebreakerindex\" : ");
  sprintf(s, "%d, ", InFuseBreaker->index);
  returnValue = StringConcatTo(returnValue, s);

  returnValue = StringConcatTo(returnValue, "\"current\" : ");
  sprintf(s, "%.02f", value);
  returnValue = StringConcatTo(returnValue, s);
  
  returnValue = StringConcatTo(returnValue, " }");

  return returnValue;
}

/*****************************************************************************!
 * Function : FuseBreakerSetCurrent
 *****************************************************************************/
void
FuseBreakerSetCurrent
(FuseBreaker* InFuseBreaker, float InCurrent)
{
  if ( NULL == InFuseBreaker ) {
    return;
  }
  InFuseBreaker->current.fd = InCurrent;
}

/*****************************************************************************!
 * Function : FuseBreakerValueFromJSONString
 *****************************************************************************/
bool
FuseBreakerValueFromJSONString
(FuseBreaker* InFuseBreaker, json_value* InValue)
{
  if ( NULL == InFuseBreaker || NULL == InValue ) {
    return false;
  }

  InFuseBreaker->current.fd = JSONIFGetFloat(InValue, "current");
  if ( InFuseBreaker->canRegister ) {
    InFuseBreaker->canRegister->Value.fd = InFuseBreaker->current.fd;
  }
  return true;
}

/*****************************************************************************!
 * Function : FuseBreakerDisplay
 *****************************************************************************/
void
FuseBreakerDisplay
(FuseBreaker* InFuseBreaker, int InIndent)
{
  string				indentString;

  if ( NULL == InFuseBreaker ) {
    return;
  }
  if ( InIndent > 0 ) {
    indentString = StringFill(' ', InIndent);
  } else {
    indentString = "";
  }
 
  printf("%sIndex           : %d\n", indentString, InFuseBreaker->index);
  printf("%sType            : %s\n", indentString, InFuseBreaker->type);
  printf("%sType Class      : %s\n", indentString, InFuseBreaker->typeClass);
  printf("%sRating          : %d\n", indentString, InFuseBreaker->rating);
  if ( InFuseBreaker->canRegister ) {
    printf("%sCurrent         : %.02f\n", indentString, InFuseBreaker->canRegister->Value.fd);
  }
  if ( InIndent > 0 ) {
    FreeMemory(indentString);
  } 
}

