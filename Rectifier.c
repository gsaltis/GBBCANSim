/*****************************************************************************
 * FILE NAME    : Rectifier.c
 * DATE         : July 20 2020
 * PROJECT      : BDFB Simulator
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
#include "Rectifier.h"
#include "Devices.h"
#include "Bay.h"
#include "MemoryManager.h"

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
 * Function : CreateRectifier
 *****************************************************************************/
Rectifier*
CreateRectifier
(uint16_t InIndex)
{
  Rectifier*				rectifier;
  int					n;

  n = sizeof(Rectifier);
  rectifier = (Rectifier*)GetMemory(n);
  memset(rectifier, 0x00, n);


  rectifier->index = InIndex;
  rectifier->canDevice = AddSingleDevice("REC");
  return rectifier;
}


/*****************************************************************************!
 * Function : CreateRectifierNoDevice
 *****************************************************************************/
Rectifier*
CreateRectifierNoDevice
(uint16_t InIndex)
{
  Rectifier*				rectifier;
  int					n;

  n = sizeof(Rectifier);
  rectifier = (Rectifier*)GetMemory(n);
  rectifier->index = InIndex;
  memset(rectifier, 0x00, n);

  return rectifier;
}

/*****************************************************************************!
 * Function : RectifierToJSONString
 *****************************************************************************/
string
RectifierToJSONString
(Rectifier* InRectifier)
{
  string				jsonString;
  char					s[14];

  jsonString = StringCopy("      {\n");

  sprintf(s, "%d", InRectifier->index);
  jsonString = StringConcatTo(jsonString, "        \"index\" : ");
  jsonString = StringConcatTo(jsonString, s);
  jsonString = StringConcatTo(jsonString, ",\n");

  sprintf(s, "%d", InRectifier->canDevice->CanAddress);
  jsonString = StringConcatTo(jsonString, "        \"canaddress\" : ");
  jsonString = StringConcatTo(jsonString, s);
  jsonString = StringConcatTo(jsonString, "\n");

  jsonString = StringConcatTo(jsonString, "      }");
  return jsonString;
}

/*****************************************************************************!
 * Function : RectifierFromJSONString
 *****************************************************************************/
void
RectifierFromJSONString
(Rectifier* InRectifier, json_value* InValue)
{
  uint16_t				index, canAddress;

  if ( NULL == InRectifier || NULL == InValue ) {
    return;
  }

  index = JSONIFGetInt(InValue, "index");
  canAddress = JSONIFGetInt(InValue, "canaddress");

  InRectifier->index = index;
  InRectifier->canDevice = AddCANDevice("REC", canAddress);
}

/*****************************************************************************!
 * Function : RectifierDisplay
 *****************************************************************************/
void
RectifierDisplay
(Rectifier* InRectifier, uint16_t InIndent, bool InVerbose)
{
  string				indentString;
  uint16_t				m, n;
 
  if ( InIndent ) {
    indentString = StringFill(' ', InIndent);
  } else {
    indentString = "";
  }
  if ( InVerbose ) {
    return;
  }

  n = printf("%sRectifier %d", indentString, InRectifier->index);
  m = 20 - n;
  printf("%*s%3d\n", m, " ", InRectifier->canDevice->CanAddress);
  if ( InIndent ) {
    FreeMemory(indentString);
  }
}

