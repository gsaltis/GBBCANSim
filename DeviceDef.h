/*******************************************************************************
 * FILE         : DeviceDef.h
 * DATE         : September 24 2018
 * PROJECT      : CanSim
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/
#ifndef _devicedef_h_
#define _devicedef_h_

/*******************************************************************************!
 * Global Headers
 *******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************!
 * Local Headers
 *******************************************************************************/
#include "DeviceDefines.h"
#include "DeviceRegDef.h"
#include "DeviceMessageDef.h"
#include "NumericTypes.h"
#include "json.h"
#include "JSONIF.h"

/*******************************************************************************!
 * Exported Macros
 *******************************************************************************/
#define DEVICE_DEF_OK                   1
#define DEVICE_DEF_DUPLICATE_DEVICE     2
#define DEVICE_DEF_MISSING_NAME         3
#define DEVICE_DEF_MISSING_DESC         4
#define DEVICE_DEF_MISSING_CAN_ADDRESS  5
#define DEVICE_DEF_MISSING_PROTOCOL     6
    

#define DEVICE_DEF_NULL_DEVICE_LIST     500
#define DEVICE_DEF_GENERAL_ERROR        1001

/*******************************************************************************!
 * Exported Type : DeviceDef
 *******************************************************************************/
struct _DeviceDef {
    int                                 canAddressStart;
    int                                 protocolNumber;
    string                              name;
    string                              desc;
    int                                 messageType;
    DeviceRegDefList*                   regDefs;
    DeviceMessageDefList*               messageDefs;
    bool                                UniqueSNLow;
    ufloatbit32_t                       StartingSNLow;
    int                                 displayColumns;
    int                                 DisplayType;
    struct _DeviceDef*                  next;
};
typedef struct _DeviceDef               DeviceDef;

/*******************************************************************************!
 * Exported Type : DeviceDefList
 *******************************************************************************/
struct _DeviceDefList {
    DeviceDef*                          defs;
    DeviceDef*                          lastDef;
};
typedef struct _DeviceDefList           DeviceDefList;

/*******************************************************************************!
 * Exported Functions
 *******************************************************************************/
DeviceDef*
CreateDeviceDef
();

int
CreateNewDeviceDef
(char* InName, char* InDesc, int InCANAddressStart, int InProtocolNumber, int InMessageType, int InDisplayColumns, int InDisplayType, DeviceDefList* InDeviceDefs);

DeviceDef*
ReadDeviceDef
(FILE* InFile);

void
DestroyDeviceDef
(DeviceDef* InDeviceDef);

void
DumpDeviceDef
(DeviceDef* InDeviceDef, int InIndent);

void
DumpDeviceDefShort
(DeviceDef* InDeviceDef, int InIndent, int InNameIndent, int InDestIndent,
 bool InDumpRegs, bool InDumpMessages, int InSortOrder);

DeviceDefList*
CreateDeviceDefList
();

DeviceDefList*
ReadDeviceDefList
(FILE* InFile);

void
DestroyDeviceDefList
(DeviceDefList* InDeviceDefs);

void
DumpDeviceDefList
(DeviceDefList* InDeviceDefs, int InIndent);

void
AddDeviceDef
(DeviceDefList* InDeviceDefs, DeviceDef* InDeviceDef);

void
RemoveDeviceDef
(DeviceDefList* InDeviceDefs, DeviceDef* InDeviceDef);

void
RemoveDeviceDefByName
(DeviceDefList* InDeviceDefs, char* InName);

DeviceDef*
FindDeviceDef
(DeviceDefList* InDevciceDefs, char* InName);

DeviceDef*
FindDeviceDefByProtocol
(DeviceDefList* InDeviceDefs, int InProtocol);

int
DeviceDefGetMessageCount
(DeviceDef* InDeviceDef);

int
DeviceDefGetRegCount
(DeviceDef* InDeviceDef);

char*
DeviceDefErrorCodeToString
(int InErrorCode);

DeviceMessageDef*
DeviceDefFindMessageByType
(DeviceDef* InDeviceDef, int InMessageType);

DeviceRegDef*
DeviceDefFindRegByValueType
(DeviceDef* InDeviceDef, int InType);

DeviceRegDef*
FindRegDefByValueType
(DeviceDefList* InDeviceDefs, int InValueType);

DeviceDefList*
DeviceDefListFromJSON
(json_value* InJSON);

string
DeviceDefToJSON
(DeviceDef* InDeviceDef);

string
DeviceDefListToJSON
(DeviceDefList* InDeviceDefList);

DeviceDef*
DeviceDefFromJSON
(json_value* InJSON);

void
DeviceDefListAppendDef
(DeviceDefList* InDeviceDefList, DeviceDef* InDeviceDef);

#endif // _devicedef_h_
