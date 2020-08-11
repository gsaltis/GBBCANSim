/*******************************************************************************
 * FILE         : DeviceDef.c
 * DATE         : September 24 2018
 * PROJECT      : CanSim
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/
 
/*******************************************************************************!
 * Local Headers
 *******************************************************************************/
#include "DeviceDef.h"
#include "DirManagement.h"
#include "MemoryManager.h"
#include "DefFileToken.h"
#include "ThreadSafePrint.h"

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
static void
DeviceDefResolveResponseDefs
(DeviceDef* InDeviceDef);

/*******************************************************************************!
 * Function : CreateDeviceDef
 *******************************************************************************/
DeviceDef*
CreateDeviceDef
()
{
  DeviceDef*                          deviceDef;
  deviceDef = (DeviceDef*)GetMemory(sizeof(DeviceDef));
  memset(deviceDef, 0x00, sizeof(DeviceDef));
  return deviceDef;
}

/*******************************************************************************!
 * Function : CreateNewDeviceDef
 *******************************************************************************/
int
CreateNewDeviceDef
(
 char* InName, char* InDesc, int InCANAddressStart, int InProtocolNumber, int InMessageType, int InDisplayColumns, int InDisplayType, DeviceDefList* InDeviceDefs
)
{
    DeviceDef*                          deviceDef;
    
    if ( NULL == InDeviceDefs ) {
        return DEVICE_DEF_NULL_DEVICE_LIST;
    }
    
    if ( InName == NULL || InName[0] == 0x00 ) {
        return DEVICE_DEF_MISSING_NAME;
    }
    if ( InDesc == NULL || InDesc[0] == 0x00 ) {
        return DEVICE_DEF_MISSING_DESC;
    }
    if ( InProtocolNumber == 0 ) {
        return DEVICE_DEF_MISSING_PROTOCOL;
    }
    deviceDef = CreateDeviceDef();
    deviceDef->name = StringCopy(InName);
    deviceDef->desc = StringCopy(InDesc);
    deviceDef->messageType = InMessageType;
    deviceDef->protocolNumber = InProtocolNumber;
    deviceDef->displayColumns = InDisplayColumns;
    deviceDef->DisplayType = InDisplayType;
    AddDeviceDef(InDeviceDefs, deviceDef);
    return DEVICE_DEF_OK;
}

/*******************************************************************************!
 * Function : DestroyDeviceDef
 *******************************************************************************/
void
DestroyDeviceDef
(
 DeviceDef*                             InDeviceDef
)
{
  if ( InDeviceDef == NULL ) {
    return;
  }
  if ( InDeviceDef->regDefs ) {
    DestroyDeviceRegList(InDeviceDef->regDefs);
  }
    
  if ( InDeviceDef->messageDefs ) {
    DestroyDeviceMessageDefList(InDeviceDef->messageDefs);
  }
  FreeMemory(InDeviceDef->name);
  FreeMemory(InDeviceDef->desc);
  FreeMemory(InDeviceDef);
}

/*****************************************************************************!
 * Function : DumpDeviceDefShort
 *****************************************************************************/
void
DumpDeviceDefShort
(DeviceDef* InDeviceDef, int InIndent, int InNameIndent, int InDescIndent,
 bool InDumpRegs, bool InDumpMessages, int InRegSortOrder)
{
  printf_safe("%*s%*s   %*s      %4d     (%03d)%2x       %4d\n",
              InIndent, InIndent > 0 ? " " : "",
              InNameIndent, InDeviceDef->name,
              InDescIndent, InDeviceDef->desc,
              InDeviceDef->canAddressStart,
              InDeviceDef->protocolNumber,
              InDeviceDef->protocolNumber,
              InDeviceDef->displayColumns);
  if ( InDumpRegs ) {
    DumpDeviceRegDefListShort(InDeviceDef->regDefs, InIndent + 4, InRegSortOrder);
  }
  if ( InDumpMessages ) {
    DumpDeviceMessageDefListShort(InDeviceDef->messageDefs, InIndent);
  }
}

/*******************************************************************************!
 * Function : DumpDeviceDef
 *******************************************************************************/
void
DumpDeviceDef
(
 DeviceDef*                             InDeviceDef,
 int                                    InIndent
)
{
    if ( NULL == InDeviceDef ) {
        return;
    }
    printf_safe("%*s           DEVICE DEF :\n", InIndent, InIndent > 0 ? " " : "");
    printf_safe("%*s                 Name : %s\n", InIndent, InIndent > 0 ? " " : "", InDeviceDef->name);
    printf_safe("%*s                 Desc : %s\n", InIndent, InIndent > 0 ? " " : "", InDeviceDef->desc);
    printf_safe("%*s    CAN Address Start : %d\n", InIndent, InIndent > 0 ? " " : "", InDeviceDef->canAddressStart);
    printf_safe("%*s      Protocol Number : %d\n", InIndent, InIndent > 0 ? " " : "", InDeviceDef->protocolNumber);
    printf_safe("%*s      Display Columns : %d\n", InIndent, InIndent > 0 ? " " : "", InDeviceDef->displayColumns);
    DumpDeviceRegDefList(InDeviceDef->regDefs, InIndent + 4);
    DumpDeviceMessageDefList(InDeviceDef->messageDefs, InIndent + 4);
}

/*******************************************************************************!
 * Function : CreateDeviceDefList
 *******************************************************************************/
DeviceDefList*
CreateDeviceDefList
()
{
    DeviceDefList*                      deviceDefs;
    deviceDefs = (DeviceDefList*)GetMemory(sizeof(DeviceDefList));
    memset(deviceDefs, 0x00, sizeof(DeviceDefList));
    return deviceDefs;
}

/*******************************************************************************!
 * Function : DumpDeviceDefList
 *******************************************************************************/
void
DumpDeviceDefList
(
 DeviceDefList*                         InDeviceDefList,
 int                                    InIndent
)
{
    DeviceDef*                          deviceDef;
    
    if ( NULL == InDeviceDefList ) {
        return;
    }
    printf_safe("%*s      Device Def List :\n", InIndent, InIndent > 0 ? " " : "");
    for ( deviceDef = InDeviceDefList->defs; deviceDef; deviceDef = deviceDef->next ) {
        DumpDeviceDef(deviceDef, InIndent+4);
        printf_safe("\n");
    }
}

/*****************************************************************************!
 * Function : DeviceDefListFromJSON
 *****************************************************************************/
DeviceDefList*
DeviceDefListFromJSON
(json_value* InJSON)
{
  DeviceDefList*	                deviceDefList;
  int                                   i;
  json_value*				value;
  DeviceDef*				deviceDef;

  if ( InJSON == NULL ) {
    return NULL;
  }

  if ( InJSON->type != json_array ) {
    return NULL;
  }

  deviceDefList = CreateDeviceDefList();
  for ( i = 0 ; i < InJSON->u.array.length; i++ ) {
    value = InJSON->u.array.values[i];
    deviceDef = DeviceDefFromJSON(value);
    if ( deviceDef ) {
      AddDeviceDef(deviceDefList, deviceDef);
    }
  }

  return deviceDefList;
}

/*****************************************************************************!
 * Function : DeviceDefFromJSON
 *****************************************************************************/
DeviceDef*
DeviceDefFromJSON
(json_value* InJSON)
{
  DeviceDef*				deviceDef;
  json_value*				regs_array;
  json_value*				msgs_array;
  json_value*				value;
  int					i;
  DeviceRegDef*				reg;
  DeviceMessageDef*			msg;

  if ( NULL == InJSON ) {
    return NULL;
  }

  deviceDef = (DeviceDef*)GetMemory(sizeof(DeviceDef));
  memset(deviceDef, 0x00, sizeof(DeviceDef));

  deviceDef->canAddressStart = JSONIFGetInt(InJSON, "startingcanaddress");
  deviceDef->protocolNumber = JSONIFGetInt(InJSON, "protocol");

  deviceDef->name = JSONIFGetString(InJSON, "name");
  deviceDef->desc = JSONIFGetString(InJSON, "description");
  
  deviceDef->regDefs = CreateDeviceRegDefList();
  deviceDef->messageDefs = CreateDeviceMessageDefList();

  regs_array = JSONIFGetArray(InJSON, "regs");
  if ( regs_array ) {
    for ( i = 0; i < regs_array->u.array.length; i++ ) {
      value = regs_array->u.array.values[i];
      reg = DeviceRegDefFromJSON(value);
      AddDeviceRegDef(deviceDef->regDefs, reg);
    } 
  }

  msgs_array = JSONIFGetArray(InJSON, "messages");
  if ( msgs_array ) {
    for (i = 0; i < msgs_array->u.array.length; i++) {
      value = msgs_array->u.array.values[i];
      msg = DeviceMessageDefFromJSON(value);
      AddDeviceMessageDef(deviceDef->messageDefs, msg);
      msg->deviceDefParent = deviceDef;
    }
  }
  DeviceDefResolveResponseDefs(deviceDef);
  return deviceDef;
}

/*****************************************************************************!
 * Function : DeviceDefToJSON
 *****************************************************************************/
string
DeviceDefToJSON
(DeviceDef* InDeviceDef)
{
  string				returnString;
  char					s[16];  
  if ( NULL == InDeviceDef ) {
    return NULL;
  }
  returnString = StringCopy("    {\n");
  returnString = StringConcatTo(returnString, "      \"name\" : \"");
  returnString = StringConcatTo(returnString, InDeviceDef->name);
  returnString = StringConcatTo(returnString, "\",\n");
  
  returnString = StringConcatTo(returnString, "      \"description\" : \"");
  returnString = StringConcatTo(returnString, InDeviceDef->desc);
  returnString = StringConcatTo(returnString, "\",\n");
 
  returnString = StringConcatTo(returnString, "      \"startcanaddress\" : ");
  sprintf(s, "%d,\n", InDeviceDef->canAddressStart);
  returnString = StringConcatTo(returnString, s);
  
  returnString = StringConcatTo(returnString, "      \"protocol\" : ");
  sprintf(s, "%d\n", InDeviceDef->protocolNumber);
  returnString = StringConcatTo(returnString, s);
  
  returnString = StringConcatTo(returnString, "    }");

  return returnString;
}
 
/*****************************************************************************!
 * Function : DeviceDefListToJSON
 *****************************************************************************/
string
DeviceDefListToJSON
(DeviceDefList* InDeviceDefs)
{
  string				returnString;
  string				defString;
  DeviceDef*				def;

  if ( InDeviceDefs == NULL ) {
    return NULL;
  }
  returnString = StringCopy("  [\n");

  for ( def = InDeviceDefs->defs; def; def = def->next ) {
    defString = DeviceDefToJSON(def);
    returnString = StringConcatTo(returnString, defString);
    FreeMemory(defString);
    if ( def->next ) {
      returnString = StringConcatTo(returnString, ",\n");
    } else {
      returnString = StringConcatTo(returnString, "\n");
    }
  }
  returnString = StringConcatTo(returnString, "  ]");
  return returnString;
}


/*****************************************************************************!
 * Function : DeviceDefResolveResponseDefs
 *****************************************************************************/
void
DeviceDefResolveResponseDefs
(DeviceDef* InDeviceDef)
{
  DeviceMessageDef*			message;
  DeviceMessageResponse*		response;
  int					i;

  for ( message = InDeviceDef->messageDefs->definitions; message; message = message->next) {
    for ( i = 0; i < message->responsesCount; i++ ) {
      response = &message->responses[i];
      response->responseDef = FindRegDefByValue(InDeviceDef->regDefs, response->responseType); 
    }
  }
}

/*****************************************************************************!
 * Function : DeviceDefListAppendDef 
 *****************************************************************************/
void
DeviceDefListAppendDef
(DeviceDefList* InDeviceDefList, DeviceDef* InDeviceDef)
{
  if ( InDeviceDefList == NULL || InDeviceDef == NULL ) {
    return;
  }
}

/*******************************************************************************!
 * Function : DestroyDeviceDefList
 *******************************************************************************/
void
DestroyDeviceDefList
(
 DeviceDefList*                         InDeviceDefs
)
{
    DeviceDef*                          deviceDef;
    if ( InDeviceDefs == NULL ) {
        return;
    }

    for (deviceDef = InDeviceDefs->defs;  deviceDef; deviceDef = deviceDef->next ) {
        DestroyDeviceDef(deviceDef);
    }    
    FreeMemory(InDeviceDefs);
}

/*******************************************************************************!
 * Function : AddDeviceDef
 *******************************************************************************/
void
AddDeviceDef
(
 DeviceDefList*                         InDeviceDefs,
 DeviceDef*                             InDeviceDef
)
{
    if ( InDeviceDefs == NULL || InDeviceDef == NULL ) {
        return;
    }
    if (InDeviceDefs->defs == NULL) {
        InDeviceDefs->defs = InDeviceDef;
        InDeviceDefs->lastDef = InDeviceDef;
    } else {
        InDeviceDefs->lastDef->next = InDeviceDef;
        InDeviceDefs->lastDef = InDeviceDef;
    }
}

/*******************************************************************************!
 * Function : RemoveDeviceDef
 *******************************************************************************/
void
RemoveDeviceDef
(
 DeviceDefList*                         InDeviceDefs,
 DeviceDef*                             InDeviceDef
)
{
    DeviceDef*                          deviceDef;
    if ( InDeviceDefs == NULL || InDeviceDef == NULL ) {
        return;
    }
    for (deviceDef = InDeviceDefs->defs;  deviceDef; deviceDef = deviceDef->next ) {
        if ( !strcmp(deviceDef->name, InDeviceDef->name) ) {
            RemoveDeviceDefByName(InDeviceDefs, InDeviceDef->name);
        }
    }    
}

/*******************************************************************************!
 * Function : RemoveDeviceDefByName
 *******************************************************************************/
void
RemoveDeviceDefByName
(
 DeviceDefList*                         InDeviceDefs,
 char*                                  InName
)
{
    DeviceDef*                          deviceDef;
    DeviceDef*                          previousDef;
    
    if ( InDeviceDefs == NULL || InName == NULL ) {
        return;
    }
    previousDef = NULL;
    for (deviceDef = InDeviceDefs->defs;  deviceDef; deviceDef = deviceDef->next ) {
        if ( !strcmp(deviceDef->name, InName) ) {
            break;
        }
        previousDef = deviceDef;
    }
    if ( deviceDef == NULL ) {
        return;
    }
    if ( deviceDef == InDeviceDefs->defs ) {
        if ( InDeviceDefs->lastDef == deviceDef ) {
            InDeviceDefs->defs = NULL;
            InDeviceDefs->lastDef = NULL;
        } else {
            InDeviceDefs->defs = deviceDef->next;
        }
    } else {
        previousDef->next = deviceDef->next;
        if ( InDeviceDefs->lastDef == deviceDef ) {
            InDeviceDefs->lastDef = previousDef;
        }
    }
    DestroyDeviceDef(deviceDef);
}

/*******************************************************************************!
 * Function : FindDeviceDef 
 *******************************************************************************/
DeviceDef*
FindDeviceDef
(
 DeviceDefList*                         InDeviceDefs,
 char*                                  InName
)
{
    DeviceDef*                          deviceDef;
    if ( InDeviceDefs == NULL || InName == NULL) {
        return NULL;
    }
    for (deviceDef = InDeviceDefs->defs;  deviceDef; deviceDef = deviceDef->next ) {
        if ( !strcmp(deviceDef->name, InName) ) {
            return deviceDef;            
        }
    }
    return NULL;
}

/*******************************************************************************!
 * Function : DeviceDefGetRegCount
 *******************************************************************************/
int
DeviceDefGetRegCount
(
 DeviceDef*                             InDeviceDef
)
{
    if ( NULL == InDeviceDef ) {
        return 0;
    }
    return GetDeviceRegCount(InDeviceDef->regDefs);
}

/*******************************************************************************!
 * Function : DeviceDefGetMessageCount
 *******************************************************************************/
int
DeviceDefGetMessageCount
(
 DeviceDef*                             InDeviceDef
)
{
    if ( NULL == InDeviceDef ) {
        return 0;
    }
    return GetDeviceMessageCount(InDeviceDef->messageDefs);
}

/*******************************************************************************!
 * Function : DeviceDefErrorCodeToString
 *******************************************************************************/
char*
DeviceDefErrorCodeToString
(
 int                                    InErrorCode
)
{
    switch(InErrorCode) {
        case DEVICE_DEF_OK :
            return "OK";
            
        case DEVICE_DEF_DUPLICATE_DEVICE :
            return "Duplicate Device";
            
        case DEVICE_DEF_MISSING_NAME :
            return "Missing name";

        case DEVICE_DEF_MISSING_DESC :
            return "Missing description";
            
        case DEVICE_DEF_MISSING_CAN_ADDRESS :
            return "CAN Address";
            
        case DEVICE_DEF_MISSING_PROTOCOL :
            return "Missing Protocol";
            
        case DEVICE_DEF_NULL_DEVICE_LIST :
            return "Empty Device Definition List";
            
        case DEVICE_DEF_GENERAL_ERROR : 
            return "General Error";
    }
    return "";
}


/*****************************************************************************!
 * Function : DeviceDefFindMessageByType
 *****************************************************************************/
DeviceMessageDef*
DeviceDefFindMessageByType
(DeviceDef* InDeviceDef, int InMessageType)
{
  return FindMessageDefByMessageType(InDeviceDef->messageDefs, InMessageType); 
}

/*****************************************************************************!
 * Function : DeviceDefFindRegByValueType
 *****************************************************************************/
DeviceRegDef*
DeviceDefFindRegByValueType
(DeviceDef* InDeviceDef, int InValueType)
{
  return FindRegDefByValue(InDeviceDef->regDefs, InValueType);
}

/*****************************************************************************!
 * Function : FindRegDefByValueType
 *****************************************************************************/
DeviceRegDef*
FindRegDefByValueType
(DeviceDefList* InDeviceDefs, int InValueType)
{
  DeviceDef*                            def;
  DeviceRegDef*				deviceRegDef;

  for ( def = InDeviceDefs->defs ; def ; def = def->next ) {
    for ( deviceRegDef = def->regDefs->definitions; deviceRegDef; deviceRegDef = deviceRegDef->next ) {
      if ( deviceRegDef->valueType == InValueType ) {
        return deviceRegDef;
      }
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : FindDeviceDefByProtocol
 *****************************************************************************/
DeviceDef*
FindDeviceDefByProtocol
(DeviceDefList* InDeviceDefs, int InProtocol)
{
  for ( DeviceDef* def = InDeviceDefs->defs; def; def = def->next ) {
    if ( def->protocolNumber == InProtocol ) {
      return def;
    }
  }
  return NULL;
}

