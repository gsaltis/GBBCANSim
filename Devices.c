/*****************************************************************************
 FILE NAME      : Devices.c
 DATE           : October 01 2018
 PROJECT        : 
 COPYRIGHT      : Copyright (C) 2018 by Gregory R Saltis
******************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "CanMsg.h"
#include "AllCanDefinitions.h"
#include "CANInterface.h"
#include "ThreadSafePrint.h"
#include "UserInputHandling.h"
#include "Devices.h"
#include "MemoryManager.h"
#include "DeviceDef.h"
#include "main.h"
#include "String.h"
#include "BytesManage.h"

/*****************************************************************************!
 * Loca
 *****************************************************************************/
#define Upcase(s) {int i; for (i = 0; (s[i] != '\0' && s[i] != '\n'); i++) { s[i] = toupper(s[i]); } s[i] = '\0';}
#define ColorRed "\e[1;31m" 
#define ColorWhite "\e[0m"
#define ColorGreen "\e[32m"

CanDevice CanDeviceList[MAXDEVICES];
int NumDevices = 0;

int GetNumDevice() { return NumDevices; }
CanDevice* GetCanDevice() { return &CanDeviceList[0]; }


/*****************************************************************************!
 * Function : CanDeviceListInitialize
 *****************************************************************************/
void
CanDeviceListInitialize
()
{
  memset(CanDeviceList, 0x00, sizeof(CanDeviceList));
}

/*******************************************************************************
 * Function : GetCanDeviceDefByName
 *******************************************************************************/
DeviceDef*
GetCanDeviceDefByName
(char* InDefName)
{
    return FindDeviceDef(mainDeviceDefs, InDefName);
}

/*******************************************************************************
 * Function : GetGroupArray
 *******************************************************************************/
#ifdef IMPLEMENTED
void GetGroupArray(int* FullGroupList, DeviceDef* InDeviceDef)
{
    DeviceRegDef*                       regDef;
    DeviceRegDef*                       regDefPrev;
    int                                 GroupIndex;
    for (int i = 0; i < MAXGROUPS; i++) {
        FullGroupList[i] = -1;
    }
    GroupIndex = 0;
    for ( int reg = 0, regDef = InDeviceDef->regDefs->definitions; regDef; regDef = regDef->next, reg++ ) {
        if (reg == 0) {
            FullGroupList[GroupIndex] = reg;
            GroupIndex++;
        } else {
            if (strcasecmp(regDef->group, regDefPrev->group) != 0) {
                bool Found = false;
                for (int grp = 0; ((grp < MAXGROUPS) && (FullGroupList[grp] != -1)); grp++) {
                    if (strcasecmp(regDef->group, InDeviceDef->RegDefList[FullGroupList[grp]].Group) == 0) {
                        Found = true;
                    }
                }
                if (!Found) {
                    FullGroupList[GroupIndex] = reg; GroupIndex++; // put the group name in the list
                }
            }
        }
        regDefPrev = regDef;
    }
}
#endif

/*****************************************************************************!
 * Function : GetDeviceList
 * Purpose  : Return a list of comma separated device names
 *****************************************************************************/
string
GetDeviceList()
{
  string				deviceList;
  DeviceDef*                            deviceDef;
    
  for ( deviceDef = mainDeviceDefs->defs; deviceDef;  deviceDef = deviceDef->next ) {
    if ( deviceList == NULL ) {
      deviceList = StringCopy(deviceDef->name);
    } else {
      deviceList = StringConcatTo(deviceList, ", ");
      deviceList = StringConcatTo(deviceList, deviceDef->name); 
    }
  }
  return deviceList;
}

/*******************************************************************************!
 * Function : GetDeviceCount
 *******************************************************************************/
void GetDeviceCount(char* CountList, DeviceDef* MyDeviceDef)
{
    int MaxCount = 0;
    for (int i = 0; i < NumDevices; i++) {
        if (CanDeviceList[i].deviceDefinition == MyDeviceDef) {
            MaxCount++;
        }
    }
    sprintf(&CountList[0], "1-%d", MaxCount);
}

/*****************************************************************************!
 * Function : GetGroupList
 *****************************************************************************/
string GetGroupList(DeviceDef* MyDeviceDef)
{
  string				returnString;
  StringList*				list;

  list = StringListCreate();
    
  for (DeviceRegDef* regDef = MyDeviceDef->regDefs->definitions; regDef; regDef = regDef->next ) {
    if ( !StringListContains(list, regDef->group) ) {
      StringListAppend(list, regDef->group);
    }
  }

  returnString = StringListConcat(list, ", ");
  StringListDestroy(list);
  return returnString;
}

/*****************************************************************************!
 * Function : GetAbbrevList
 *****************************************************************************/
void GetAbbrevList(char* AbbrevList, char* GroupName, DeviceDef* MyDeviceDef)
{
    bool FirstFound = false;
    bool FoundNumber = false;
    strcpy(AbbrevList, "<no registers found>");
    char FirstNum[SHORTSTRINGLEN];
    char LastNum[SHORTSTRINGLEN];
    for ( DeviceRegDef* regDef = MyDeviceDef->regDefs->definitions; regDef; regDef = regDef->next ) {
        char MyDeviceGroupName[SHORTSTRINGLEN];
        strcpy(&MyDeviceGroupName[0], regDef->group);
        char IncomingGroupName[SHORTSTRINGLEN];
        strcpy(&IncomingGroupName[0], &GroupName[0]);
        if (strcasecmp(IncomingGroupName, MyDeviceGroupName) == 0)
        {
            if (strcasecmp("1", regDef->shortName) == 0) {
                FoundNumber = true;
                strcpy(&FirstNum[0], regDef->shortName);
            }
            if (FoundNumber)
                strcpy(&LastNum[0], regDef->shortName);
            else if (!FirstFound) {
                strcpy(AbbrevList, regDef->shortName);
                FirstFound = true;
            } else {
                strcat(AbbrevList, ", ");
                strcat(AbbrevList, regDef->shortName);
            }
        }
    }
    if (FoundNumber)
        sprintf(AbbrevList, "%s to %s", FirstNum, LastNum);
}

/*******************************************************************************!
 * Function : GetRegisterByGroup
 *******************************************************************************/
int GetRegisterByGroup(char* MyGroup, char* MyAbbrev, DeviceDef* MyDeviceDef)
{
    DeviceRegDef*                       regDef;
    
    for ( regDef = MyDeviceDef->regDefs->definitions; regDef; regDef = regDef->next ) {
        if ( strcmp(regDef->group, MyGroup) == 0 && strcmp(regDef->shortName, MyAbbrev) == 0 ) {
            return regDef->valueType;
        }
    }
    return 0;
}

/*******************************************************************************!
 * Function : GetGroupAndAbbrev
 *******************************************************************************/
void GetGroupAndAbbrev(int Protocol, int ValueType, char* MyGroup, char* MyAbbrev)
{
    DeviceDef* deviceDef;
    DeviceRegDef* regDef;
    
    strcpy(MyGroup, "<none>");
    strcpy(MyAbbrev, "<none>");
    for ( deviceDef = mainDeviceDefs->defs; deviceDef; deviceDef = deviceDef->next ) {
        if (deviceDef->protocolNumber == Protocol ) {
            regDef = FindRegDefByValue(deviceDef->regDefs, ValueType);
            if ( regDef ) {
                strcpy(MyGroup, regDef->group);
                strcpy(MyAbbrev, regDef->shortName);
            }
        }
    }
}

/*****************************************************************************!
 * Function : DisplayDeviceDefMessages
 * Purpose  : Display all messages for a give Device Definition
 *****************************************************************************/
void
DisplayDeviceDefMessages
(DeviceDef* InDevice)
{
  DeviceMessageDef*                     messageDef;
  int                                   j;
 
  printf_safe("\n\n%sDevice  Start address: 0x%04X Protocol: 0x%04X Device: %s %s\n", ColorRed, InDevice->canAddressStart, InDevice->protocolNumber, InDevice->name, ColorWhite);
  for ( j = 0, messageDef = InDevice->messageDefs->definitions; messageDef; messageDef = messageDef->next, j++ ) {
    printf_safe("Message: %d 0x%04X \n", j, messageDef->msgType);
    for ( int k = 0 ; k < messageDef->responsesCount ; k++ ) {
      int valueType = messageDef->responses[k].responseType;
      DeviceRegDef* regDef = FindRegDefByValue(InDevice->regDefs, valueType);
      if ( regDef) {
        printf_safe("Message sends back: %d 0x%04X %f 0x%08X %s %d \n", k, regDef->valueType, regDef->initialValue.fd, regDef->initialValue.data32, regDef->name, regDef->messageType);
      } 
    } 
  } 
}
 
/*****************************************************************************!
 * Function : HandleShowMessages
 *****************************************************************************/
void 
DisplayDeviceDefsMessages
() 
{
  DeviceDef* deviceDef;
  DeviceMessageDef* messageDef;
  int j;
  for ( deviceDef = mainDeviceDefs->defs; deviceDef; deviceDef = deviceDef->next ) {
    printf_safe("\n\n%sDevice  Start address: 0x%04X Protocol: 0x%04X Device: %s %s\n", ColorRed, deviceDef->canAddressStart, deviceDef->protocolNumber, deviceDef->name, ColorWhite);
    for ( j = 0, messageDef = deviceDef->messageDefs->definitions; messageDef; messageDef = messageDef->next, j++ ) {
      printf_safe("Message: %d 0x%04X \n", j, messageDef->msgType);
      for ( int k = 0 ; k < messageDef->responsesCount ; k++ ) {
        int valueType = messageDef->responses[k].responseType;
        DeviceRegDef* regDef = FindRegDefByValue(deviceDef->regDefs, valueType);
        if ( regDef) {
          printf_safe("Message sends back: %d 0x%04X %f 0x%08X %s %d \n", k, regDef->valueType, regDef->initialValue.fd, regDef->initialValue.data32, regDef->name, regDef->messageType);
        } 
      } 
    } 
  }
}

/*****************************************************************************!
 * Function : DumpDefs
 *****************************************************************************/
void DumpDefs()
{
#ifdef IMPLEMENTED
DeviceDef* deviceDef;
    DeviceRegDef* regDef;
    
    for ( deviceDef = mainDeviceDefs->defs; deviceDef; deviceDef = deviceDef->next ) {
        printf_safe("\n\n%sDevice  Start address: 0x%04X Protocol: 0x%04X Device: %s NumRegisters: %d %s\n", ColorRed, deviceDef->canAddressStart, deviceDef->protocolNumber, deviceDef->name, DeviceDefGetRegCount(deviceDef), ColorWhite);
        for ( regDef = deviceDef->regDefs->definitions; regDef; regDef = regDef->next ) {
            printf_safe("Register: 0x%04X Initial value: %f 0x%08X Name: %s %s Type: 0x%04X \n", RegDef->ValueType, RegDef->InitialValue.fd, RegDef->InitialValue.data32, RegDef->Group, RegDef->Abbrev, RegDef->MsgType);
        }

        char GroupList[COMMANDSIZE]; strcpy(GroupList, "");
        char MyGroup[COMMANDSIZE]; strcpy(MyGroup, "");
        char MyAbbrev[COMMANDSIZE*2]; strcpy(MyAbbrev, "");

        GetGroupList(&GroupList[0], MyDeviceDef);
        printf_safe("\nGroups: %s\n", GroupList);

        int FullGroupList[MAXGROUPS];
        GetGroupArray(&FullGroupList[0], MyDeviceDef);
        for (int i = 0; i < MAXGROUPS; i++)
        {
            if (FullGroupList[i] != -1)
            {
                strcpy(MyGroup, MyDeviceDef->RegDefList[FullGroupList[i]].Group);
                GetAbbrevList(&MyAbbrev[0], &MyGroup[0], MyDeviceDef);
                printf_safe("Register abbreviations for %s: %s \n", MyGroup, MyAbbrev);
            }
        }
    }
#endif
}

/*****************************************************************************!
 * Function FindDevice
 *****************************************************************************/
CanDevice* FindDevice(char* DeviceName, int Count)
{
    for (int i = 0; i < NumDevices; i++)  // for each device
    {
        if (strcasecmp(CanDeviceList[i].deviceDefinition->name, DeviceName) == 0)  // found match for my device name
            if ((CanDeviceList[i].CanAddress - CanDeviceList[i].deviceDefinition->canAddressStart + 1) == Count) // and found my device
                return &CanDeviceList[i];
    }
    return NULL;
}

/*****************************************************************************!
 * Function : DumpDevice
 *****************************************************************************/
void DumpDevice(char* DeviceName, int Count)
{
    CanDevice* MyDevice = FindDevice(DeviceName, Count);
    if (MyDevice == NULL) {
         printf_safe("Device not found. \n");
         return;
    }
    int n = DeviceDefGetRegCount(MyDevice->deviceDefinition);
    for ( int k = 0 ; k < n ; k++ ) {
        CanReg* reg = &MyDevice->Registers[k];
        printf_safe("Device register: 0x%04X %16s %16s value: 0x%08X %f\n", reg->registerDef->valueType, reg->registerDef->group, reg->registerDef->shortName, reg->Value.data32, reg->Value.fd );
    }
}

/*****************************************************************************!
 * Function : ClearDeviceRegisters
 *****************************************************************************/
void
ClearDeviceRegisters
(CanDevice* InDevice)
{
  int					i, n;
  
  n = DeviceDefGetRegCount(InDevice->deviceDefinition);
  for ( i = 0; i < n ; i++ ) {
    CanReg* reg = &InDevice->Registers[i];
    reg->Value.data32 = 0;
  }
}

/*****************************************************************************!
 * Function : DumpGroup
 *****************************************************************************/
void DumpGroup(char* DeviceName, int Count, char* GroupName)
{
    CanDevice* MyDevice = FindDevice(DeviceName, Count);
    if (MyDevice == NULL) {
        printf_safe("Device not found. \n");
        return;
    }
    
    for ( DeviceRegDef* regDef = MyDevice->deviceDefinition->regDefs->definitions; regDef; regDef = regDef->next ) {
        CanReg* Reg = GetRegisterByDef(MyDevice, regDef);
        if (strcasecmp(regDef->group, GroupName) == 0) {
            printf_safe("Device register: 0x%04X %16s %16s value: 0x%08X %f\n", regDef->valueType, regDef->group, regDef->shortName, Reg->Value.data32, Reg->Value.fd);
        }
    }
}

/*******************************************************************************
 * Function : SetRegisterValueByName
 *******************************************************************************/
int SetRegisterValueByName(char* DeviceName, int Count, char* AbbrevName, char* Group, float NewValue)
{
  CanReg*                       reg;
  CanDevice*                    device = FindDevice(DeviceName, Count);
  if ( device == NULL ) {
    return DEVICE_ERROR_DEVICE_NOT_FOUND;
  }
  reg = DeviceFindRegister(device, AbbrevName, Group);
  if ( NULL == reg ) {
    return DEVICE_ERROR_REGISTER_NOT_FOUND;
  }
  reg->Value.fd = NewValue;
  return DEVICE_ERROR_NONE;
}

/*****************************************************************************!
 * Function : SetDeviceRegister
 *****************************************************************************/
int
SetDeviceRegister
(CanDevice* InDevice, string InShortName, string InGroup, float InValue)
{ 
  CanReg*				reg;

  reg = DeviceFindRegister(InDevice, InShortName, InGroup);
  if ( NULL == reg ) {
    return DEVICE_ERROR_REGISTER_NOT_FOUND;
  }
  reg->Value.fd = InValue;
  return DEVICE_ERROR_NONE;
}

/*****************************************************************************!
 * Function : GetDeviceRegisterFloat
 *****************************************************************************/
float
GetDeviceRegisterFloat
(CanDevice* InDevice, int InIndex, string InGroup)
{
  CanReg*				reg;
  char					s[17];
  
  snprintf(s, sizeof(s) - 1, "%d", InIndex);
  reg = DeviceFindRegister(InDevice, s, InGroup);
  if ( reg == NULL ) {
    return 0.0;
  }
  return reg->Value.fd;
}

/*****************************************************************************!
 * Function : SetRegisterValue
 *****************************************************************************/
void SetRegisterValue(char* DeviceName, int Count, int RegisterNum, ufloatbit32_t NewValue)
{
    CanDevice* MyDevice = FindDevice(DeviceName, Count);
    if (MyDevice == NULL) {
        printf_safe("Device not found. \n");
        return;
    }
    int n = DeviceDefGetRegCount(MyDevice->deviceDefinition);
    for (int k = 0; k < n; k++) {
        CanReg* Reg = &MyDevice->Registers[k];
        if (RegisterNum == Reg->registerDef->valueType) {
            Reg->Value.data32 = NewValue.data32;
            printf_safe("%f 0x%08X is the new value of 0x%03X %s in %s - %d \n", 
                        Reg->Value.fd, Reg->Value.data32, Reg->registerDef->valueType, Reg->registerDef->name, MyDevice->deviceDefinition->name,
                        MyDevice->CanAddress);
            break;
        }
    }
}

/*****************************************************************************!
 * Function : SetRegisterValueHex
 *****************************************************************************/
void SetRegisterValueHex(char* DeviceName, int Count, int RegisterNum, uint32_t NewValue)
{
    ufloatbit32_t Value; Value.data32 = NewValue;
    SetRegisterValue(DeviceName, Count, RegisterNum, Value);
}

/*****************************************************************************!
 * Function : SetRegisterValueFlat
 *****************************************************************************/
void SetRegisterValueFloat(char* DeviceName, int Count, int RegisterNum, float NewValue)
{
    ufloatbit32_t Value; Value.fd = NewValue;
    SetRegisterValue(DeviceName, Count, RegisterNum, Value);
}

/*******************************************************************************!
 * Function : RemoveDevice
 *  Purpose : Remove a device from the CanDeviceList
 *     Note : This involves not only removing the devuce but shuffline the list
 *            to close up the gap created when the device was removed.
 *******************************************************************************/
int
RemoveDevice
(
 char*                                  InDeviceName,
 int                                    InDeviceNumber
)
{
    int                                 dev, newDeviceCount;
    CanDevice*                          currDevice;
    bool                                found;
    int                                 foundIndex;
    int                                 deviceAddress;
    
    // The external ID is the devices CAN Address + 1
    deviceAddress = InDeviceNumber - 1;
    
    // A small sanity check before we start things
    if ( NumDevices == 0 ) {
        return DEVICE_ERROR_DEVICE_NOT_FOUND;
    }
    
    found = false;
    foundIndex = -1;
    for ( dev = 0 ; dev < NumDevices && !found ; dev++ ) {
        currDevice = &CanDeviceList[dev];
        if ( strcasecmp(currDevice->deviceDefinition->name, InDeviceName) == 0 &&
             deviceAddress == currDevice->CanAddress ) {
            // Save the index of the now empty index space for when we shuffle the 
            // device list later
            foundIndex = dev;
            found = true;
            break;
        }
    }
    
    // We didn't find it, return an error
    if ( !found ) {
        return DEVICE_ERROR_DEVICE_NOT_FOUND;
    }
    
    // Clear the removed device and its associated registers
    ClearDevice(currDevice, true);

    // Remove device from table by shifting the devices higher in the table 
    // down one to fill the empty space in the array
    newDeviceCount = NumDevices-1;
    for ( dev = foundIndex; dev < NumDevices-1; dev++) {
        CopyDevice(&CanDeviceList[dev], &CanDeviceList[dev+1]);
    }
    
    // Clear the last entry in the array (it's been moved one to the left)
    ClearDevice(&CanDeviceList[newDeviceCount], false);
    NumDevices = newDeviceCount;
    return DEVICE_ERROR_NONE;
}

/*******************************************************************************!
 * Function : ClearDevice
 *  Purpose : Clear the contencts of a device structure. 
 *     Note : The register may be destroyed if InFreeRegisters is true
 *            otherwise the pointer is just set to NULL
 *******************************************************************************/
void
ClearDevice
(
 CanDevice*                             InDevice,
 bool                                   InFreeRegisters
)
{
    if ( NULL == InDevice ) {
        return;
    }
    if ( InFreeRegisters ) {
        FreeMemory(InDevice->Registers);
    }
    InDevice->CanAddress = 0;
    InDevice->State      = 0;
    InDevice->Registers  = NULL;
    InDevice->deviceDefinition        = NULL;
}

/*******************************************************************************!
 * Function : CopyDevice
 * Purpose  : Copy the contents of one device to another.
 *     Note : This is not a 'deep' copy, just the pointer to the registers
 *            is copied not all the registers.
 *******************************************************************************/
void
CopyDevice
(
 CanDevice*                             InTargetDevice,
 CanDevice*                             InSourceDevice
)
{
    if ( NULL == InTargetDevice || NULL == InSourceDevice ) {
        return;
    }
    InTargetDevice->CanAddress = InSourceDevice->CanAddress;
    InTargetDevice->State      = InSourceDevice->State;
    InTargetDevice->deviceDefinition        = InSourceDevice->deviceDefinition;
    InTargetDevice->Registers  = InSourceDevice->Registers;
}

/*****************************************************************************!
 * 
 *****************************************************************************/
CanDevice*
AddCANDevice
(string InDeviceDefName, uint16_t InDeviceCANAddress)
{
  CanDevice*				device;
  DeviceDef*				deviceDef;

  device = FindCANDeviceByDefNameAddress(InDeviceDefName, InDeviceCANAddress);
  if ( device ) {
    return device;
  }

  deviceDef = FindDeviceDef(mainDeviceDefs, InDeviceDefName);  
  if ( NULL == deviceDef ) {
    return NULL;
  }

  device = CreateDevice(deviceDef, InDeviceCANAddress);

  return device; 
}

/*****************************************************************************!
 * Function : CreateDevice 
 *****************************************************************************/
CanDevice*
CreateDevice
(DeviceDef* InDeviceDef, uint16_t InDeviceCANAddress)
{
  CanDevice* 				device;
  int					m, reg;
  DeviceRegDef*				regDef;

  device = &CanDeviceList[NumDevices];  
  NumDevices++;
  device->deviceDefinition = InDeviceDef;
  device->CanAddress = InDeviceCANAddress;
  m = DeviceDefGetRegCount(InDeviceDef);
  device->Registers = GetMemory(sizeof(CanReg) * m);
  device->registersCount = m;
  for ( reg = 0, regDef = InDeviceDef->regDefs->definitions; regDef; regDef = regDef->next, reg++) {
    device->Registers[reg].Value = regDef->initialValue;
    device->Registers[reg].registerDef = regDef;
  }
  device->State = normal;
  return device;
}

/*****************************************************************************!
 * Function : FindCANDeviceByDefNameAddress
 *****************************************************************************/
CanDevice*
FindCANDeviceByDefNameAddress
(string InDeviceDefName, uint16_t InDeviceCANAddress)
{
  int					i;
  CanDevice*				device;

  for ( i = 0 ; i < NumDevices ; i++ ) {
    device = &CanDeviceList[i];
    if ( StringEqual(device->deviceDefinition->name, InDeviceDefName) ) {
      if ( device->CanAddress == InDeviceCANAddress ) {
     	return device;
      } 
    }
  }
  return NULL;
}

/*******************************************************************************!
 * Function : AddSingleDevice
 *******************************************************************************/
CanDevice* AddSingleDevice(char* DeviceName)
{
  CanDevice*                            CurrDevice;
  int                                   MaxCanAddr = 0;
  uint32_t                              MaxSNLow = 0;
  bool                                  ExistingDevice = false;
  DeviceDef*                            deviceDef;
  int                                   reg;
  DeviceRegDef*                         regDef;
  int					m;

  // This section finds the largest CAN address and the largest SNLow used so far in existing devices.
  for (int dev = 0; dev < NumDevices; dev++) {
    CurrDevice = &CanDeviceList[dev];
    if (strcasecmp(DeviceName, CurrDevice->deviceDefinition->name) == 0) {
      ExistingDevice = true;  
      MaxCanAddr = max(MaxCanAddr, CurrDevice->CanAddress); 
      if (CurrDevice->deviceDefinition->UniqueSNLow) {
        for (reg = 0, regDef = CurrDevice->deviceDefinition->regDefs->definitions; regDef; regDef = regDef->next, reg++) {
          if ( regDef->valueType == 0x54 ) {
            MaxSNLow = max(MaxSNLow, CurrDevice->Registers[reg].Value.data32);
          }
        }
      }
    }
  }
  if (ExistingDevice) {
    MaxCanAddr = MaxCanAddr + 1; // use one higher than the highest CAN address found
    MaxSNLow = MaxSNLow + 1; // use the next serial number
  }


  deviceDef = FindDeviceDef(mainDeviceDefs, DeviceName);
  if ( deviceDef == NULL ) {
    return NULL;
  }

  CanDevice* Device = &CanDeviceList[NumDevices];  
  NumDevices++;
  Device->deviceDefinition = deviceDef;
  Device->CanAddress = max(deviceDef->canAddressStart, MaxCanAddr);
  m = DeviceDefGetRegCount(deviceDef);
  Device->Registers = GetMemory(sizeof(CanReg) * m);
  Device->registersCount = m;
  for ( reg = 0, regDef = deviceDef->regDefs->definitions; regDef; regDef = regDef->next, reg++) {
    Device->Registers[reg].Value = regDef->initialValue;
    Device->Registers[reg].registerDef = regDef;
#ifdef IMPLEMENTED
    if ((deviceDef->uniqueSNLow) && (DeviceDef->RegDefList[reg].ValueType == 0x54))
      Device->Registers[reg].Value.data32 = max(DeviceDef->StartingSNLow.data32, MaxSNLow);

#endif
  }
  Device->State = normal;
  return Device;
}

/*****************************************************************************!
 * Function AddDevice
 *****************************************************************************/
bool AddDevice(char* DeviceName, int Count)
{
    for (int j = 0; j < Count; j++) {
        CanDevice* device = AddSingleDevice(DeviceName);
        if ( device ) {
            printf_safe("Added Device: %s at address %04X with protocol %03X \n", DeviceName, device->CanAddress, device->deviceDefinition->protocolNumber);
        }
    }
    return true;
}

/*****************************************************************************!
 * Function : EnableDevice
 *****************************************************************************/
bool EnableDevice(char* DeviceName, int Num)
{
    CanDevice* MyDevice = FindDevice(DeviceName, Num);
    if (MyDevice != NULL)
        MyDevice->State = normal;
    else
        printf_safe("Device is not found. \n");
    return true;
}

/*****************************************************************************!
 * Function : DisableDevice
 *****************************************************************************/
bool DisableDevice(char* DeviceName, int Num)
{
    CanDevice* MyDevice = FindDevice(DeviceName, Num);
    if (MyDevice != NULL)
        MyDevice->State = offline;
    else
        printf_safe("Device is not found. \n");
    return true;
}

/*****************************************************************************!
 * Function : GetRequestBreakdown
 *****************************************************************************/
void GetRequestBreakdown(frameid Id, dataframe Data, int* Protocol, int* SrcAddr, int* DestAddr, int* IncomingMsgType, int* IncomingErrType, int* IncomingValueType, ufloatbit32_t* IncomingValue)
{
  ParseId(Id, Protocol, SrcAddr, DestAddr);
  ParseData(Data, IncomingMsgType, IncomingValueType, IncomingErrType, IncomingValue);
  return;
}

/*****************************************************************************!
 * Function : SendResponses
 *****************************************************************************/
void SendResponses(CANInterface* InInterface, int Protocol, int SrcAddr, int DestAddr, int IncomingMsgType, int IncomingValueType, ufloatbit32_t IncomingValue, time_t InTime)
{
  // printf_safe("Sending responses for 0x%02X 0x%02X 0x%02X 0x%04X 0x%04X 0x%08X %f \n" , Protocol, SrcAddr, DestAddr, IncomingMsgType, IncomingValueType, IncomingValue.data32 , IncomingValue.fd);
  for (int dev = 0; dev<NumDevices; dev++) {  // look through the list of devices
    CanDevice* CurrDevice = &CanDeviceList[dev];
    DeviceDef* currDeviceDef = CurrDevice->deviceDefinition;
        
    if (CurrDevice->State == offline) {
      continue;
    }
    if (currDeviceDef->protocolNumber != Protocol) {
      continue;
    }
    if ((CurrDevice->CanAddress != DestAddr) && (DestAddr != 0xFF)) {
      continue;
    }
    CurrDevice->lastContactTime = InTime;
    if ((IncomingMsgType == 0x01) || (IncomingMsgType == 0x73)) { // if this is a get request
      int n = DeviceDefGetRegCount(currDeviceDef);
      for (int reg = 0; reg < n; reg++) { // for each register in this device
        if (CurrDevice->Registers[reg].registerDef->valueType == IncomingValueType)  {// if the ValueType matches the ValueType in the request
          int Cont = 0;
          CanReg Reg = CurrDevice->Registers[reg];    // get the value for this register              
          frameid Id = GetFrameId(Protocol, CurrDevice->CanAddress, SrcAddr, Cont);
          dataframe Data = GetDataFrame(CurrDevice->State, Reg.registerDef->messageType, Reg.registerDef->valueType, Reg.Value);
          CANInterfaceWrite(InInterface, Id.data32, Data.data64, 8); // and send the value in the response
        }
      }
    } else if ((IncomingMsgType == 0x03) || (IncomingMsgType == 0x74)) { //if this is a set request
      int n = DeviceDefGetRegCount(currDeviceDef);
      for (int reg = 0; reg < n; reg++) { // for each register in the this device
        if (CurrDevice->Registers[reg].registerDef->valueType == IncomingValueType) { // if the ValueType matches the ValueType in the request
          CurrDevice->Registers[reg].Value = IncomingValue;  // set the value of this register to the incoming value in the request
        }
      }
    } else { // look up the response
      DeviceMessageDef* messageDef = FindMessageDefByMessageType(currDeviceDef->messageDefs, IncomingMsgType);
      if ( messageDef ) {
        for (int resp = 0; resp < messageDef->responsesCount; resp++) { // for each response in the list of responses for this MsgType
          DeviceMessageResponse Response = messageDef->responses[resp]; // this is the ValueType to send back
          DeviceRegDef* regDef = FindRegDefByValue(currDeviceDef->regDefs, Response.responseType);
          if ( regDef ) {
            CanReg* reg = GetRegisterByDef(CurrDevice, regDef);
            if ( reg ) {
	      uint64_t					d;

              int Cont = resp + 1 < messageDef->responsesCount ? 1 : 0;
              frameid Id = GetFrameId(Protocol, CurrDevice->CanAddress, SrcAddr, Cont);
              dataframe Data = GetDataFrame(CurrDevice->State, reg->registerDef->messageType, reg->registerDef->valueType, reg->Value);
	      d = ByteManageSwap8(Data.data64);
              CANInterfaceWrite(InInterface, Id.data32, d, 8);  // and send the register
            }
          }
        }
      }
    }
  }
}

/*****************************************************************************!
 * Function : GetRegisterByName
 *****************************************************************************/
CanReg*
GetRegisterByName
(CanDevice* InDevice, char* InName)
{
  int					n, i;

  n = GetDeviceRegCount(InDevice->deviceDefinition->regDefs);
  for (i = 0; i < n; i++) {
    if ( StringEqual(InDevice->Registers[i].registerDef->name, InName) ) {
      return &(InDevice->Registers[i]);
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : GetRegisterByDef
 *****************************************************************************/
CanReg*
GetRegisterByDef
(
 CanDevice*                             InDevice,
 DeviceRegDef*                          InRegDef
)
{
    int                                 n = GetDeviceRegCount(InDevice->deviceDefinition->regDefs);
    for (int i = 0; i < n; i++) {
        if ( InDevice->Registers[i].registerDef == InRegDef ) {
            return &(InDevice->Registers[i]);
        }
    }
    return NULL;
}

/*****************************************************************************!
 * Function : HandleReguest
 *****************************************************************************/
void HandleRequest(CANInterface* InInterface, frameid Id, dataframe Data, time_t InTime)
{
    int Protocol;
    int SrcAddr;
    int DestAddr;
    int IncomingMsgType;
    int IncomingValueType, IncomingErrType;
    ufloatbit32_t IncomingValue;
    GetRequestBreakdown(Id, Data, &Protocol, &SrcAddr, &DestAddr, &IncomingMsgType, &IncomingErrType, &IncomingValueType, &IncomingValue);
    SendResponses(InInterface, Protocol, SrcAddr, DestAddr, IncomingMsgType, IncomingValueType, IncomingValue, InTime);
    return;
}

/*****************************************************************************!
 * Function : HandleCanTraffic
 *****************************************************************************/
#define BUFFERSIZE 1024
bool PortOpened = false;
void* HandleCanTraffic()
{
  CANInterface*				interface;
  interface = CANInterfaceInit("slcan0");
  if ( NULL == interface ) {
    while (true) { 
      sleep(1);
    }
  }
  while (true)
  {
    uint32_t				id;
    uint64_t				data;
    uint8_t				dataLength;
    uint8_t 				result;

    result = CANInterfaceRead(interface, &id, &data, &dataLength);
    switch ( result ) {
      case CAN_READ_TIMEOUT : {
        break;
      }
      case CAN_READ_OK : {
        frameid fid;
        fid.data32 = id;
        dataframe df;
        df.data64 = ByteManageSwap8(data);
    	HandleRequest(interface, fid, df, time(NULL)); 
	break;
      }
      case CAN_READ_ERROR : {
	break;
      }
      default : {
	break;
      }
    }
  }
  return 0;
}

/*******************************************************************************!
 * Function : GetDeviceTypeCount
 *******************************************************************************/
int
GetDeviceTypeCount
(
 char*                          InDeviceName
)
{
 
  int                           deviceCount = 0;

  for ( int i = 0 ; i < NumDevices ; i++) {
    if ( strcasecmp(CanDeviceList[i].deviceDefinition->name, InDeviceName) == 0 ) {
      deviceCount++;
    }
  }
  return deviceCount;
}                       

/*******************************************************************************!
 * Function : GetFirstDevice
 * Purpose  : Get the first device of a given type
 *******************************************************************************/
CanDevice*
GetFirstDevice
(
 char*                                  InDeviceName
)
{
  if ( InDeviceName == NULL || 0x00 == *InDeviceName ) {
    return NULL;
  }
  for ( int i = 0 ; i < NumDevices ; i++ ) {
    if ( strcasecmp(CanDeviceList[i].deviceDefinition->name, InDeviceName) == 0 ) {
      return &CanDeviceList[i];
    }
  }
  return NULL;
}

/*******************************************************************************!
 * Function : GetNextDevice
 * Purpose  : Get the device of the same type following a given device
 *******************************************************************************/
CanDevice*
GetNextDevice
(
 CanDevice*                             InCurrentDevice
)
{
  int                                   i;

  if ( NULL == InCurrentDevice ) {
    return NULL;
  }

  // Find the location of the current device
  for ( i = 0 ; i < NumDevices; i++ ) {
    if ( &CanDeviceList[i] == InCurrentDevice ) {
      break;
    }
  }

  // We should not have exhausted the list but in case we do
  if ( i >= NumDevices ) {
    return NULL;
  }

  // Starting at the next location find the device with the same device type
  for ( i++ ; i < NumDevices; i++ ) {
    if ( strcasecmp(CanDeviceList[i].deviceDefinition->name, InCurrentDevice->deviceDefinition->name) == 0 ) {
      // We found is so return it
      return &(CanDeviceList[i]);
    }
  }
  // InCurrentDevice was the last one, so return NULL;
  return NULL;
}

/*******************************************************************************!
 * Function : DeviceFindRegister
 * Purpose  : Find a Register in name by short name and gorup
 *******************************************************************************/
CanReg*
DeviceFindRegister
(
 CanDevice*                             InDevice,
 char*                                  InShortName,
 char*                                  InGroup
)
{
    int                                 n = GetDeviceRegCount(InDevice->deviceDefinition->regDefs);
    for (int i = 0; i < n; i++) {
        if ( strcmp(InDevice->Registers[i].registerDef->group, InGroup) == 0 &&
             strcmp(InDevice->Registers[i].registerDef->shortName, InShortName) == 0 ) {
            return &(InDevice->Registers[i]);
        }
    }
    return NULL;
}

/*****************************************************************************!
 * Function : GetDeviceNumbersString
 *****************************************************************************/
string
GetDeviceNumbersString
(string InDeviceName)
{
  CanDevice*				device;
  int					i;
  string				returnString;

  returnString = NULL;
  if ( InDeviceName == NULL ) {
    return NULL;
  }
  
  for ( i = 0; i < NumDevices; i++ ) {
    device = &CanDeviceList[i];
    if ( StringEqual(device->deviceDefinition->name, InDeviceName) ) {
      if ( returnString == NULL ) {
      }
    }
  }
  return returnString;
}

/*****************************************************************************!
 * Function : FindRegisterByIndexGroup
 *****************************************************************************/
CanReg*
FindRegisterByIndexGroup
(CanDevice* InDevice, int InIndex, string InGroup)
{
  CanReg*				reg;
  char					s[17];
  if ( InDevice == NULL || InGroup == NULL ) {
    return NULL;
  }
  
  snprintf(s, sizeof(s) - 1, "%d", InIndex);
  reg = DeviceFindRegister(InDevice, s, InGroup);
  return reg;
}
