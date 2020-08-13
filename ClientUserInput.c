/*****************************************************************************
 * FILE NAME    : ClientUserInput.c
 * DATE         : August 11 2020
 * PROJECT      : GBB CAN Simulator
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
#include <stdarg.h>
#include <unistd.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "ClientUserInput.h"
#include "linenoise.h"
#include "MemoryManager.h"
#include "ascii.h"
#include "ThreadSafePrint.h"
#include "ANSIColors.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
pthread_t
ClientUserInputThreadID;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
void
ClientUserInputHandleShowMessages
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleShowCAN
(StringList* InParameters, string InCommandString);

void
ClientUserInputSendMessage
(string InFormat, ...);

void
ClientUserInputHandleShowBay
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleShowAll
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleShowGroup
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleShowDevice
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleClearDevice
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleClearDevices
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleListRegisters
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleListDeviceTypes
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleListDevices
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleRemovePanel
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleRemoveBay
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleAddPanel
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleAddBay
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleAddDevice
(StringList* InParameters, string InCommandString);

void
ClientUserInputHandleHelp
(StringList* InParameters, string InCommandString);

#include "ClientUserInput/ClientUserInputInit.c"
#include "ClientUserInput/ClientUserInputThread.c"
#include "ClientUserInput/ClientUserInputHandleExit.c"
#include "ClientUserInput/ClientUserInputHandleCommand.c"
#include "ClientUserInput/ClientUserInputHandleHelp.c"
#include "ClientUserInput/ClientUserInputHandleAdd.c"
#include "ClientUserInput/ClientUserInputHandleRemove.c"
#include "ClientUserInput/ClientUserInputHandleList.c"
#include "ClientUserInput/ClientUserInputHandleClear.c"
#include "ClientUserInput/ClientUserInputHandleSet.c"
#include "ClientUserInput/ClientUserInputHandleShow.c"
#include "ClientUserInput/ClientUserInputHandleBays.c"
#include "ClientUserInput/ClientUserInputHandlePanels.c"
#include "ClientUserInput/ClientUserInputHandleOption.c"
#include "ClientUserInput/ClientUserInputHandleAddDevice.c"
#include "ClientUserInput/ClientUserInputHandleAddBay.c"
#include "ClientUserInput/ClientUserInputHandleAddPanel.c"
#include "ClientUserInput/ClientUserInputHandleRemoveBay.c"
#include "ClientUserInput/ClientUserInputHandleRemovePanel.c"
#include "ClientUserInput/ClientUserInputHandleListDevices.c"
#include "ClientUserInput/ClientUserInputHandleListDeviceTypes.c"
#include "ClientUserInput/ClientUserInputHandleListRegisters.c"
#include "ClientUserInput/ClientUserInputHandleClearDevices.c"
#include "ClientUserInput/ClientUserInputHandleClearDevice.c"
#include "ClientUserInput/ClientUserInputHandleShowDevice.c"
#include "ClientUserInput/ClientUserInputHandleShowGroup.c"
#include "ClientUserInput/ClientUserInputHandleShowAll.c"
#include "ClientUserInput/ClientUserInputHandleShowBay.c"
#include "ClientUserInput/ClientUserInputSendMessage.c"
#include "ClientUserInput/ClientUserInputHandleShowCAN.c"
#include "ClientUserInput/ClientUserInputHandleShowMessages.c"
