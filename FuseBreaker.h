/*****************************************************************************
 * FILE NAME    : FuseBreaker.h
 * DATE         : March 27 2020
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
#ifndef _fusebreaker_h_
#define _fusebreaker_h_

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
#include "CanMsg.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Type : FuseBreakerState
 *****************************************************************************/
typedef enum _FuseBreakerState
{
  FuseBreakerStateNone                  = 0,
  FuseBreakerStateOpen,
  FuseBreakerStateClosed
} FuseBreakerState;

/*****************************************************************************!
 * Exported Type : FuseBreaker
 *****************************************************************************/
typedef struct _FuseBreaker
{
  uint32_t                              index;
  string                                type;
  string				typeClass;
  int                                   rating;
  FuseBreakerState                      state;
  struct _Panel*                        panel;
  ufloatbit32_t                         current;
  CanReg*				canRegister;
} FuseBreaker;

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
bool
FuseBreakerValueFromJSONString
(FuseBreaker* InFuseBreaker, json_value* InValue);

string
FuseBreakerValueToJSONString
(FuseBreaker* InFuseBreaker);

bool
FuseBreakerUpdateFromJSONString
(FuseBreaker* InFuseBreaker, json_value* InFuseBreakerInfo);

string
FuseBreakerToJSONString
(FuseBreaker* InFuseBreaker);

string
FuseBreakerStateToString
(FuseBreakerState InState);

FuseBreaker*
FuseBreakerCreate
(uint32_t InPanelIndex, string InType, string InTypeClass, int InRating,FuseBreakerState InState);

void
FuseBreakerDestroy
(FuseBreaker* InFuseBreaker);

FuseBreaker*
FuseBreakerFromJSONString
(json_value* InValue);

FuseBreakerState
FuseBreakerStateFromString
(string InString);

void
FuseBreakerDisplay
(FuseBreaker* InFuseBreaker, int InIndent);

void
FuseBreakerSetCurrent
(FuseBreaker* InFuseBreaker, float InCurrent);

#endif /* _fusebreaker_h_*/
