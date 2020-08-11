/*****************************************************************************
 * FILE NAME    : Rectifier.h
 * DATE         : July 20 2020
 * PROJECT      : BDFB Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
#ifndef _emptyheaderfile_h_
#define _emptyheaderfile_h_

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
#include "CanMsg.h"
#include "String.h"
#include "JSONIF.h"

/*****************************************************************************!
 * Exported Type : Rectifier 
 *****************************************************************************/
struct _Rectifier
{
  uint16_t			index;
  CanDevice*			canDevice;
  struct _Bay*			bay;
  struct _Rectifier*		next;
};
typedef struct _Rectifier Rectifier;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
Rectifier*
CreateRectifier
(uint16_t InIndex);

Rectifier*
CreateRectifierNoDevice
(uint16_t InIndex);

string
RectifierToJSONString
(Rectifier* InRectifier);

void
RectifierFromJSONString
(Rectifier* InRectifier, json_value* InValue);

void
RectifierDisplay
(Rectifier* InRectifier, uint16_t InIndex, bool InVerbose);

#endif /* _emptyheaderfile_h_*/
