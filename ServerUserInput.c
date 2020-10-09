/*****************************************************************************
 * FILE NAME    : ServerUserInput.c
 * DATE         : August 12 2020
 * PROJECT      : GBB CAN Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <mongoose.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "ServerUserInput.h"
#include "main.h"
#include "WebSocketIF.h"
#include "MemoryManager.h"
#include "JSONIF.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
string
ServerUserInputPort = NULL;

pthread_t
ServerUserInputThreadID;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
void
ServerUserInputHandleRequest
(struct mg_connection* InConnection, struct mg_str InMessageString);

void*
ServerUserInputThread
(void* InParameters);

#include "ServerUserInput/ServerUserInputInit.c"
#include "ServerUserInput/ServerUserInputThread.c"
#include "ServerUserInput/ServerUserInputEventHandle.c"
#include "ServerUserInput/ServerUserInputHandleRequest.c"
#include "ServerUserInput/ServerUserInputJSONArrayToStringList.c"
#include "ServerUserInput/ServerUserInputHandleAdd.c"
#include "ServerUserInput/ServerUserInputHandleRemove.c"
#include "ServerUserInput/ServerUserInputHandleList.c"
#include "ServerUserInput/ServerUserInputHandleClear.c"
#include "ServerUserInput/ServerUserInputHandleSet.c"
#include "ServerUserInput/ServerUserInputHandleShow.c"
#include "ServerUserInput/ServerUserInputHandleBays.c"
#include "ServerUserInput/ServerUserInputHandlePanels.c"
#include "ServerUserInput/ServerUserInputHandleConnections.c"
#include "ServerUserInput/ServerUserInputHandleOptions.c"
#include "ServerUserInput/ServerUserInputHandleClose.c"
#include "ServerUserInput/ServerUserInputHandleStatus.c"
#include "ServerUserInput/ServerUserInputHandleAddDevice.c"
#include "ServerUserInput/ServerUserInputHandleAddBay.c"
#include "ServerUserInput/ServerUserInputHandleAddPanel.c"
#include "ServerUserInput/ServerUserInputHandleRemovePanel.c"
#include "ServerUserInput/ServerUserInputHandleRemoveBay.c"
#include "ServerUserInput/ServerUserInputHandleListDevices.c"
#include "ServerUserInput/ServerUserInputHandleListDeviceTypes.c"
#include "ServerUserInput/ServerUserInputHandleListRegisters.c"
#include "ServerUserInput/ServerUserInputHandleShowDevice.c"
#include "ServerUserInput/ServerUserInputHandleShowGroup.c"
#include "ServerUserInput/ServerUserInputHandleShowBay.c"
#include "ServerUserInput/ServerUserInputHandleShowMessages.c"
#include "ServerUserInput/ServerUserInputHandleShowAll.c"
#include "ServerUserInput/ServerUserInputHandleShowCAN.c"
#include "ServerUserInput/ServerUserInputSendMessage.c"
