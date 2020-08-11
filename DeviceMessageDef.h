/*******************************************************************************
 * FILE         : DeviceMessageDef.h
 * DATE         : September 24 2018
 * PROJECT      : CanSim
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/
#ifndef _devicemessagedef_h_
#define _devicemessagedef_h_

/*******************************************************************************!
 * Local Headers
 *******************************************************************************/
#include "DeviceDefines.h"
#include "DeviceRegDef.h"
#include "String.h"

/*******************************************************************************!
 * Exported Type : DeviceMessageResponse
 *******************************************************************************/
struct _DeviceMessageResponse {
   int                                  responseType;
   DeviceRegDef*                        responseDef;
};
typedef struct _DeviceMessageResponse DeviceMessageResponse;

/*******************************************************************************!
 * Exported Type : DeviceMessageDef
 *******************************************************************************/
struct  _DeviceMessageDef {
    int                                 msgType;
    string                              messageName;
    DeviceMessageResponse               responses[MAXRESP];
    int                                 responsesCount;
    struct _DeviceDef*			deviceDefParent;
    struct _DeviceMessageDef*           next;
};
typedef struct _DeviceMessageDef        DeviceMessageDef;

/*******************************************************************************!
 * Exported Type : DeviceMessageDefList
 *******************************************************************************/
struct _DeviceMessageDefList {
    DeviceMessageDef*                   definitions;
    DeviceMessageDef*                   lastDefinition;
};
typedef struct _DeviceMessageDefList    DeviceMessageDefList;

/*******************************************************************************!
 * Exported Functions
 *******************************************************************************/
void
DeviceMessageDefAddResponseType
(DeviceMessageDef* InMessageDef, int InMessageType);

DeviceMessageDef*
CreateDeviceMessageDef
();

DeviceMessageDef*
ReadDeviceMessageDef
(FILE* InFile);

void
DestroyDeviceMessageDef
(DeviceMessageDef* InMessageDef);

void
DumpDeviceMessageDefShort
(DeviceMessageDef* InMessageDef, int InIndent);

void
DumpDeviceMessageDef
(DeviceMessageDef* InMessageDef, int InIndent);

int
GetDeviceMessageCount
(DeviceMessageDefList* InMessages);

DeviceMessageDefList*
CreateDeviceMessageDefList
();

void
DumpDeviceMessageDefListShort
(DeviceMessageDefList* InMessageDefs, int InIndent);

void
DumpDeviceMessageDefList
(DeviceMessageDefList* InMessageDefs, int InIndent);

void
AddDeviceMessageDef
(DeviceMessageDefList* InMessages, DeviceMessageDef* InMessage);

DeviceMessageDefList*
ReadDeviceMessageDefList
(FILE* InFile);

void
DestroyDeviceMessageDefList
(DeviceMessageDefList* InMessageDefs);

DeviceMessageDef*
FindMessageDefByMessageType
(DeviceMessageDefList* InMessageDefs, int InMessageType);

DeviceMessageDef*
DeviceMessageDefFromJSON
(json_value* InValue);

void
DeviceMessageResponseSetFromJSON
(DeviceMessageResponse* InResponse, json_value* InValue);

string
DeviceMessageDefToJSON
(DeviceMessageDef* InMessageDef, bool InIncludeResponses);

string
DeviceMessageDefListToJSON
(DeviceMessageDefList* InMessageDefs, bool InIncludeResponses);

#endif // _devicemessagedef_h_
