/*******************************************************************************
 * FILE         : DeviceRegDef.h
 * DATE         : September 24 2018
 * PROJECT      : CanSim
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/
#ifndef _deviceregdef_h_
#define _deviceregdef_h_

/*******************************************************************************!
 * Global Headers
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*******************************************************************************!
 * Local Headers
 *******************************************************************************/
#include "DeviceDefines.h"
#include "NumericTypes.h"
#include "JSONIF.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/
#define DEVICE_REG_SORT_NONE		0
#define DEVICE_REG_SORT_GROUP		1
#define DEVICE_REG_SORT_NUMBER		2
#define DEVICE_REG_SORT_NAME		4

/*******************************************************************************!
 * Exported Type : DeviceRegDef
 *******************************************************************************/
struct  _DeviceRegDef {
    int                                 valueType;
    ufloatbit32_t                       initialValue;
    int                                 messageType;
    char                                shortName[SHORTSTRINGLEN];
    char                                groupsort[SHORTSTRINGLEN];
    char                                group[SHORTSTRINGLEN];
    char                                name[STRINGLEN];
    char                                displayLabel[STRINGLEN];
    int					formatType;
    struct _DeviceRegDef*               next;
};
typedef struct _DeviceRegDef            DeviceRegDef;

/*******************************************************************************!
 * Exported Type : DeviceRegDefList
 *******************************************************************************/
struct _DeviceRegDefList {
    DeviceRegDef*                       definitions;
    DeviceRegDef*                       lastDefinition;
};
typedef struct _DeviceRegDefList        DeviceRegDefList;

/*******************************************************************************!
 * Exported Functions
 *******************************************************************************/
DeviceRegDefList*
ReadDeviceRegList
(FILE* InFile);

void
DestroyDeviceRegList
(DeviceRegDefList* InRegDefs);

DeviceRegDef*
ReadDeviceReg
(FILE* InFile);

void
DumpDeviceRegDef
(DeviceRegDef* InRegDef, int InIndent);

void
DumpDeviceRegDefShort 
(DeviceRegDef* InRegDef, int InIndent, int InNameIndent, int InShortNameIndent, int InDisplayLabelIndent, int InGroupIndent);

DeviceRegDef*
FindRegDefByName
(DeviceRegDefList* InDefinitions, char* InName);

DeviceRegDef*
FindRegDefByValue
(DeviceRegDefList* InDefinitions, int InValue);

DeviceRegDef*
CreateDeviceRegDef
();

DeviceRegDefList*
CreateDeviceRegDefList
();

void
DumpDeviceRegDefListShort
(DeviceRegDefList* InRegDefs, int InIndent, int InSortType);

DeviceRegDef*
DeviceDefListFindRegDef
(int IncomingValueType);

void
DumpDeviceRegDefList
(DeviceRegDefList* InRegDefs, int InIndent);

void
AddDeviceRegDef
(DeviceRegDefList* InDeviceRegDefs, DeviceRegDef* InDeviceRegDef);

void
DestroyDeviceRegDef
(DeviceRegDef* InRegDef);

int
GetDeviceRegCount
(DeviceRegDefList* InDeviceRegDefs);

DeviceRegDef*
DeviceRegDefFromJSON
(json_value* InValue);

string
DeviceRegDefListToJSON
(DeviceRegDefList* InDefinitions);

string
DeviceRegDefToJSON
(DeviceRegDef* InDefinition);

#endif // _deviceregdef_h_
