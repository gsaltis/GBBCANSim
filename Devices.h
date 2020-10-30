/*******************************************************************************
 * FILE		: Devices.h
 * DATE		: April 26 2018
 * COPYRIGHT	: Copyright (c) 2018 Vertiv Company
 *******************************************************************************/
#ifndef _devices_h_
#define _devices_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <time.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "CanMsg.h"
#include "String.h"
#include "CANInterface.h"

/*******************************************************************************
 * Exported Macros
 *******************************************************************************/
#define DEVICE_ERROR_NONE			0
#define DEVICE_ERROR_DEVICE_NOT_FOUND		1
#define DEVICE_ERROR_REGISTER_NOT_FOUND		2

/*******************************************************************************
 * Export Functions
 *******************************************************************************/
int 
GetNumDeviceDef();

CanDeviceDef* 
GetCanDeviceDef();

DeviceDef*
GetCanDeviceDefByName
(char* InDefName);

void 
DumpDefs();

/*******************************************************************************
 * Device API
 *******************************************************************************/
void
CanDeviceListInitialize
();

CanDevice* 
FindDevice
(char* DeviceName, int Count);

void 
DumpDevice
(char* DeviceName, int Count);

bool
EnableDevice
(char* DeviceName, int Num);

bool 
DisableDevice
(char* DeviceName, int Num);

bool 
AddDevice
(char* DeviceName, int Count);

CanDevice*
AddSingleDevice
(char* DeviceName);

CanDevice*
AddCANDevice
(string InDeviceDefName, uint16_t InDeviceCANAddress);

CanDevice*
CreateDevice
(DeviceDef* InDeviceDef, uint16_t InDeviceCANAddress);

CanDevice*
FindCANDeviceByDefNameAddress
(string InDeviceDefName, uint16_t InDeviceCANAddress);

int
RemoveDevice
(char* InDeviceName, int InDeviceNumber);

int
GetDeviceTypeCount
(char* DeviceName);

CanDevice*
GetFirstDevice
(char* InDeviceType);

CanDevice*
GetNextDevice
(CanDevice* InCurrentDevice);

void
CopyDevice
(CanDevice* InTargetDevice, CanDevice* InSourceDevice);

void
ClearDevice
(CanDevice* InDevice, bool InFreeRegisters);

CanReg*
GetRegisterByDef
(CanDevice* InDevice, DeviceRegDef* InRegDef);

CanReg*
DeviceFindRegister
(CanDevice* device, char* AbbrevName, char* Group);

CanReg*
DeviceFindRegisterByValueType
(CanDevice* device, int InValueType);

/*******************************************************************************
 * Devices API
 *******************************************************************************/
string
GetDeviceList();

string
GetDeviceNumbersString
(string InDeviceName);

void 
GetDeviceCount(char* CountList, DeviceDef* MyDeviceDef);

int GetNumDevice();
CanDevice* GetCanDevice();

/*******************************************************************************
 * Group API
 *******************************************************************************/
void 
DumpGroup(char* DeviceName, int Count, char* GroupName );

string 
GetGroupList(DeviceDef* InDeviceDef);

void 
GetAbbrevList(char* AbbrevList, char* GroupName, DeviceDef* MyDeviceDef);

void 
SetRegisterValue(char* DeviceName, int Count, int RegisterNum, ufloatbit32_t NewValue);

int
SetRegisterValueByName(char* DeviceName, int Count, char* AbbrevName, char* Group, float NewValue);

void 
SetRegisterValueHex(char* DeviceName, int Count, int RegisterNum, uint32_t NewValue);

void 
SetRegisterValueFloat(char* DeviceName, int Count, int RegisterNum, float NewValue);

void 
GetRequestBreakdown(frameid Id, dataframe Data, int* Protocol, int* SrcAddr, int* DestAddr, int* IncomingMsgType, int* IncomingErrType, int* IncomingValueType, ufloatbit32_t* IncomingValue);

void 
SendResponses(CANInterface* InInterface, int Protocol, int SrcAddr, int DestAddr, int IncomingMsgType, int IncomingValueType, ufloatbit32_t IncomingValue, time_t InTime);

void 
HandleRequest(CANInterface* InInterface, frameid Id, dataframe Data, time_t InTime);

void* 
HandleCanTraffic();


int
GetRegisterByGroup
(char* MyGroup, char* MyAbbrev, DeviceDef* MyDeviceDef);

void
GetGroupAndAbbrev
(int Protocol, int ValueType, char* MyGroup, char* MyAbbrev);

CanReg*
FindRegisterByIndexGroup
(CanDevice* InDevice, int Index, string InGroup);

CanReg*
GetRegisterByName
(CanDevice* InDevice, char* InName);

void
ClearDeviceRegisters
(CanDevice* InDevice);

int
SetDeviceRegister
(CanDevice* InDevice, string InShortName, string InGroup, float InValue);

float
GetDeviceRegisterFloat
(CanDevice* InDevice, int InIndex, string InGroup);

void
DisplayDeviceDefMessages
(DeviceDef* InDevice);

void
DisplayDeviceDefsMessages
();


#endif
