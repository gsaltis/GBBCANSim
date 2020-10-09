/*****************************************************************************
 * FILE NAME    : ServerUserInput.h
 * DATE         : August 12 2020
 * PROJECT      : GBB CAN Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
#ifndef _serveruserinput_h_
#define _serveruserinput_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <json.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "String.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/
#define SERVER_USER_INPUT_PORT			7999

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/
extern string
ServerUserInputPort;

extern pthread_t
ServerUserInputThreadID;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
ServerUserInputSendMessage
(struct mg_connection* InConnection, string InMessage);

void
ServerUserInputHandleShowCAN
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleShowAll
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleShowMessages
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleShowBay
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleShowGroup
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleShowDevice
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleListRegisters
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleListDeviceTypes
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleListDevices
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleRemoveBay
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleRemovePanel
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleAddPanel
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleAddBay
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleAddDevice
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleStatus
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleClose
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleOptions
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleConnections
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandlePanels
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleBays
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleShow
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleSet
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleClear
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleList
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleRemove
(struct mg_connection* InConnection, StringList* InCommandStrings);

void
ServerUserInputHandleAdd
(struct mg_connection* InConnection, StringList* InCommandStrings);

StringList*
ServerUserInputJSONArrayToStringList
(json_value* InJSONArray);

void
ServerUserInputEventHandle
(struct mg_connection* nc, int ev, void* p);

void
ServerUserInputInit
();

#endif // _serveruserinput_h_
