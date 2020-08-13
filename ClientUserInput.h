/*****************************************************************************
 * FILE NAME    : ClientUserInput.h
 * DATE         : August 11 2020
 * PROJECT      : GBB CAN Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
#ifndef _clientuserinput_h_
#define _clientuserinput_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "String.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/
extern pthread_t
ClientUserInputThreadID;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
ClientUserInputHandleOption
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandlePanels
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleBays
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleShow
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleSet
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleClear
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleList
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleRemove
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleAdd
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleCommand
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleExit
(StringList* InParameters, string InCommandString);

void*
ClientUserInputThread
(void* InParameters);

void
ClientUserInputInit
();

#endif /* _clientuserinput_h_*/