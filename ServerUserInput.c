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
