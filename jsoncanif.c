/*******************************************************************************
 * FILE         : jsoncanif.c
 * DATE         : April 25 2018
 * PROJECT      : 
 * COPYRIGHT    : Copyright (c) 2018 by Vertiv Company
 *******************************************************************************/

/*******************************************************************************
 * Global Headers
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*******************************************************************************
 * Local Headers
 *******************************************************************************/
#include "jsoncanif.h"
#include "json.h"
#include "CanMsg.h"
#include "AllCanDefinitions.h"
#include "ThreadSafePrint.h"
#include "Devices.h"
#include "MemoryManager.h"
#include "DeviceDef.h"
#include "main.h"
#include "NumericTypes.h"

/*******************************************************************************
 * Local Macros
 *******************************************************************************/
#define JIRESPONSE_BUFFER_SIZE_MAX      (1024 * 64)

/*******************************************************************************
 * Local Functions
 *******************************************************************************/
static JICommandType
JIGetJSONCommand                        (json_value* InJSONDoc);

static JIDeviceType
JIGetJSONDevice                         (json_value* InJSONDoc, 
                                         char* InDeviceName, 
                                         int InDeviceNameLen);

static void
JIProcessRead                           (JICommand* InCommand, json_value* InJSONDoc);

static void
JIProcessRemove
(JICommand* InCommand, json_value* InJSONDoc);

static void
JIProcessWrite                          (JICommand* InCommand, json_value* InJSONDoc);

static void
JIProcessDeviceWrite                    (JICommand* InCommand, json_value* InJSONDoc);

static json_value*
JIFindJSONValue                         (json_value* InJSONDoc, char* InName);

static JIResponse*
JIGetReadResponse                       (JICommand* InCommand);

static JIResponse*
JIGetWriteResponse                      (JICommand* InCommand);

static JIResponse*
JIGetRemoveResponse
(JICommand* InCommand);

static JIResponse*
JIGetCreateResponse                     (JICommand* InCommand);

static void
JIGetConfigReadResponse                 (JICommand* InCommand, JIResponse* InResponse);

static void
JIGetDeviceCreateResponse               (JICommand* InCommand, JIResponse* response);

static void
JIGetDeviceWriteResponse                (JICommand* InCommand, JIResponse* response);

static JIResponseValue*
JIResponseValueCreate
();

static void
JIGetDeviceRemoveResponse(JICommand* InCommand, JIResponse* InResponse);

static void
JIGetDeviceDataResponse
(JICommand* InCommand, JIResponse* InResponse);

void
JIGetRegNamesResponse
(JICommand* InCommand, JIResponse* InResponse);

void
JIGetMessagesResponse
(JICommand* InCommand, JIResponse* InResponse);

void
JIGetAllDevicesResponse
(JICommand* InCommand, JIResponse* InResponse);

static void
JIGetIDsResponse
(JICommand* InCommand, JIResponse* InResponse);

static void
JIProcessRegisterDefAdd
(JICommand* InCommand, json_value* InJSONDoc);

static void
JIProcessMessageDefAdd
(JICommand* InCommand, json_value* InJSONDoc);

static void
JIProcessDeviceDefAdd
(JICommand* InCommand, json_value* InJSONDoc);

/*******************************************************************************
 * Local Data
 *******************************************************************************/
char JIResponseBuffer[JIRESPONSE_BUFFER_SIZE_MAX+1];

/*******************************************************************************
 * Function : JICreateCommand
 *******************************************************************************/
JICommand*
JICreateCommand
(
 json_value*                            InJSONDoc
)
{
  JICommand*                            returnCommand;

  returnCommand = (JICommand*)GetMemory(sizeof(JICommand));
  memset(returnCommand, 0x00, sizeof(JICommand));
  
  if ( NULL == InJSONDoc ) {
    return NULL;
  }

  returnCommand->command     = JIGetJSONCommand(InJSONDoc);
  returnCommand->deviceType  = JIGetJSONDevice(InJSONDoc, returnCommand->deviceName, STRINGLEN);
  switch (returnCommand->command) {
    case JICOMMAND_TYPE_READ :
      JIProcessRead(returnCommand, InJSONDoc);
      break;
    case JICOMMAND_TYPE_WRITE :
      JIProcessWrite(returnCommand, InJSONDoc);
      break;
    case JICOMMAND_TYPE_CREATE :
      break;
    case JICOMMAND_TYPE_REMOVE :
      JIProcessRemove(returnCommand, InJSONDoc);
      break;
    case JICOMMAND_TYPE_NONE :
      break;
  }

  return returnCommand;
}

/*******************************************************************************
 * Function : JIGetResponse
 *******************************************************************************/
JIResponse*
JIGetResponse
(
 JICommand*                             InCommand
)
{
  if ( NULL == InCommand ) {
    return NULL;
  }

  switch (InCommand->command) {
    case JICOMMAND_TYPE_READ : {
      return JIGetReadResponse(InCommand);
    }
    case JICOMMAND_TYPE_WRITE : {
      return JIGetWriteResponse(InCommand);
    }
    case JICOMMAND_TYPE_CREATE : {
      return JIGetCreateResponse(InCommand);
    }
    case JICOMMAND_TYPE_REMOVE : {
      return JIGetRemoveResponse(InCommand);
    }
    case JICOMMAND_TYPE_NONE : {
      // We should never reach this
      break;
    }
  }
  return NULL;
}


/*******************************************************************************
 * Function : JIProcessRemove
 *******************************************************************************/
void
JIProcessRemove
(
  JICommand*                            InCommand,
  json_value*                           InJSONDoc
)
{
  json_value*                           values;
  json_value*                           nameValue;
  json_value*                           indexValue;
  char*                                 s;
  
  values = JIFindJSONValue(InJSONDoc, "value");
  if ( NULL == values ) {
    return;
  }
  
  nameValue = JIFindJSONValue(values, "name");
  if ( NULL == nameValue ) {
    return;
  }

  indexValue = JIFindJSONValue(values, "index");
  if ( NULL == indexValue ) {
    return;
  }
  if ( indexValue->type != json_string ) {
    return;
  }
  s = indexValue->u.string.ptr;
  InCommand->requests.removeRequest.index = atoi(s);
}

/*******************************************************************************
 * Function : JIProcessRead
 *******************************************************************************/
void
JIProcessRead
(
 JICommand*                                                   InCommand,
 json_value*                            InJSONDoc
)
{
  json_value*                                               values;
  json_value*                           nameValue;
  char                                                          deviceName[STRINGLEN+1];

  values = JIFindJSONValue(InJSONDoc, "value");
  if ( NULL == values ) {
    return;
  }
  nameValue = JIFindJSONValue(values, "name");
  if ( NULL == nameValue ) {
    return;
  }
  
  InCommand->requests.readRequest.readType = JIREAD_TYPE_NONE;
  if ( nameValue->type != json_string ) {
    return;
  }
  if ( strcmp("ids", nameValue->u.string.ptr) == 0 ) {
    InCommand->requests.readRequest.readType = JIREAD_TYPE_IDS;
  } else if ( strcmp("regnames", nameValue->u.string.ptr) == 0 ) {
    InCommand->requests.configRequest.readType = JIREAD_TYPE_REGNAMES;
    InCommand->requests.configRequest.device = deviceName;
  } else if ( strcmp("messages", nameValue->u.string.ptr) == 0 ) {
    InCommand->requests.configRequest.readType = JIREAD_TYPE_MESSAGES;
    InCommand->requests.configRequest.device = deviceName;
  } else if ( strcmp("devices", nameValue->u.string.ptr) == 0 ) {
    InCommand->requests.readRequest.readType = JIREAD_TYPE_DEVICES;
  } else if ( strcmp("all", nameValue->u.string.ptr) == 0 ) {
    char*                               s;
    json_value*                         indexValue;

    InCommand->requests.readRequest.readType = JIREAD_TYPE_ALL_DATA;
    indexValue = JIFindJSONValue(values, "index");
    if ( NULL == indexValue ) {
      return;
    }
    if ( indexValue->type != json_string ) {
      return;
    }
    s = indexValue->u.string.ptr;
    InCommand->requests.readRequest.index = atoi(s);
  }
}


/*******************************************************************************
 * Function : JIGetWriteResponse
 *******************************************************************************/
JIResponse*
JIGetWriteResponse
(
 JICommand*                             InCommand
)
{
  JIResponse*                           response;

  response = (JIResponse*)GetMemory(sizeof(JIResponse));
  if ( NULL == response ) {
    printf_safe("Malloc error = time to bail\n");
    exit(EXIT_FAILURE);
  }
  memset(response, 0x00, sizeof(JIResponse));
  response->command = InCommand->command;
  JIGetDeviceWriteResponse(InCommand, response);
  return response;
}

/*******************************************************************************
 * Function : JIGetDeviceCreateResponse
 *******************************************************************************/
void
JIGetDeviceCreateResponse
(
 JICommand*                             InCommand,
 JIResponse*                            InResponse
)
{
  char*                                 deviceType;
  char                                  outLine[129];
  int                                   index;

  deviceType = InCommand->deviceName;
  CanDevice*                            device;
  device = AddSingleDevice(deviceType);
  if ( NULL == device ) {
    return;
  }
  index = (device->CanAddress - device->deviceDefinition->canAddressStart) + 1;
  memset(JIResponseBuffer, 0x00, JIRESPONSE_BUFFER_SIZE_MAX);
  strcat(JIResponseBuffer, "{\n");
  strcat(JIResponseBuffer,  "\"command\":\"create\",\n");
  sprintf(outLine, "\"device\":\"%s\",\n", deviceType);
  strcat(JIResponseBuffer, outLine);
  strcat(JIResponseBuffer, "\"status\":\"OK\",\n");
  strcat(JIResponseBuffer, "\"message\":\"\",\n");
  strcat(JIResponseBuffer, "\"value\" :\n");
  sprintf(outLine,"{\"index\":%d, ", index);
  strcat(JIResponseBuffer, outLine);
  sprintf(outLine, " \"name\":\"%s-%d\"}\n",deviceType, index);
  strcat(JIResponseBuffer, outLine);
  strcat(JIResponseBuffer, "}\n");
  InResponse->stringResponse = JIResponseBuffer;
}

/*******************************************************************************
 * Function : JIGetCreateResponse
 *******************************************************************************/
JIResponse*
JIGetCreateResponse
(
 JICommand*                             InCommand
)
{
  JIResponse*                           response;
  response = (JIResponse*)GetMemory(sizeof(JIResponse));
  if ( NULL == response ) {
    printf_safe("Malloc error - time to bail\n");
    exit(EXIT_FAILURE);
  }
  memset(response, 0x00, sizeof(JIResponse));
  response->command = InCommand->command;
  JIGetDeviceCreateResponse(InCommand, response);  
  return response;
}  
  
/*******************************************************************************
 * Function : JIGetRemoveResponse
 *******************************************************************************/
JIResponse*
JIGetRemoveResponse
(
  JICommand*                            InCommand
)
{
  JIResponse*                           response;
  response = (JIResponse*)GetMemory(sizeof(JIResponse));
  if ( NULL == response ) {
    printf_safe("Malloc error - time to bail\n");
    exit(EXIT_FAILURE);
  }
  memset(response, 0x00, sizeof(JIResponse));
  response->command = InCommand->command;
  JIGetDeviceRemoveResponse(InCommand, response);
  return response;
}

/*******************************************************************************
 * Function : JIGetDeviceRemoveResponse
 *******************************************************************************/
void
JIGetDeviceRemoveResponse
(
  JICommand*                            InCommand,
  JIResponse*                           InResponse
)
{
  char                                  outline[128];
  int                                   devStatus;
  
  devStatus = RemoveDevice(InCommand->deviceName, 
                             InCommand->requests.removeRequest.index);
  memset(JIResponseBuffer, 0x00, sizeof(JIResponseBuffer));
  strcat(JIResponseBuffer, "{\n");
  strcat(JIResponseBuffer, "\"command\":\"remove\",\n");
  sprintf(outline, "\"device\":\"%s\",\n", InCommand->deviceName);
  strcat(JIResponseBuffer, outline);
  sprintf(outline, "\"id\":\"%d\",\n", InCommand->requests.removeRequest.index);
  strcat(JIResponseBuffer, outline);
  strcat(JIResponseBuffer, "\"value\" : {");
  switch (devStatus) {
    case DEVICE_ERROR_DEVICE_NOT_FOUND :
      strcat(JIResponseBuffer, "\"status\" : \"error\", ");
      strcat(JIResponseBuffer, "\"message\", \"Device not found\"");
      break;
    case DEVICE_ERROR_NONE :
      strcat(JIResponseBuffer, "\"status\" : \"OK\",");
      sprintf(outline, "\"message\" : \"Device %d removed\"", InCommand->requests.removeRequest.index);
      strcat(JIResponseBuffer, outline);
      break;
    default :
      strcat(JIResponseBuffer, "\"status\" : \"error\", ");
      strcat(JIResponseBuffer, "\"message\" : Misc Error\"");
      break;
  }
  strcat(JIResponseBuffer, "}\n");
  strcat(JIResponseBuffer, "}\n");
  InResponse->stringResponse = JIResponseBuffer;
}

/*******************************************************************************
 * Function : JIGetDeviceWriteResponse
 *******************************************************************************/
void
JIGetDeviceWriteResponse
(
 JICommand*                             InCommand,
 JIResponse*                            InResponse
)
{
  json_value**                          elements;
  float                                 floatValue;
  char                                  outLine[128];
  char                                  returnMessage[128];
  int                                   n, m;
  json_value*                           dataValue;
  json_value*                           elementValues;
  json_value*                           valueStruct = InCommand->requests.writeRequest.valueStruct;
  JIResponseStatusType                  returnStatus;
  CanReg*                               reg;
  CanDevice*                            device;

  returnStatus = JIRESPONSE_STATUS_TYPE_OK;
  reg = NULL;
  memset(JIResponseBuffer, 0x00, sizeof(JIResponseBuffer));
  elementValues = JIFindJSONValue(valueStruct, "elements");
  if ( NULL == elementValues ) {
    return;
  }
  if ( elementValues->type != json_array ) {
    return;
  }
  strcat(JIResponseBuffer, "{\n");
  strcat(JIResponseBuffer, "\"command\":\"write\",\n");
  sprintf(outLine, "\"device\":\"%s\",\n", InCommand->deviceName);
  strcat(JIResponseBuffer, outLine);
  sprintf(outLine, "\"id\":%d,\n", InCommand->requests.writeRequest.index);
  strcat(JIResponseBuffer, outLine); 
  strcat(JIResponseBuffer, "\"value\":[\n");
  elements  = elementValues->u.array.values;
  n = elementValues->u.array.length;
  for ( int i = 0 ; i < n ; i++ ) {
    json_value* elementValue = elements[i];
    json_value* nameValue = JIFindJSONValue(elementValue, "name");
    if ( NULL == nameValue ) {
      return;
    }
    if ( nameValue->type != json_string ) {
      return;
    }
    char* name = nameValue->u.string.ptr;
    json_value* groupValue = JIFindJSONValue(elementValue, "group");
    if ( NULL == groupValue ) {
      return;
    }
    if ( groupValue->type != json_string ) {
      return;
    }

    dataValue = JIFindJSONValue(elementValue, "value");
    if ( NULL == dataValue ) {
      return;
    }

    InResponse->stringResponse = JIResponseBuffer;
   
    char* group = groupValue->u.string.ptr;
    floatValue = atof(dataValue->u.string.ptr);
    m = SetRegisterValueByName(JIDeviceTypeToString(InCommand->deviceType), InCommand->requests.writeRequest.index, name, group, floatValue);
    device = FindDevice(JIDeviceTypeToString(InCommand->deviceType), InCommand->requests.writeRequest.index);
    if ( device ) {
        reg = DeviceFindRegister(device, name, group);
    }
    sprintf(outLine, "{\n\"name\":\"%s\",\n", name);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "\"group\":\"%s\",\n", group);
    strcat(JIResponseBuffer, outLine);
    switch (m) {
      case DEVICE_ERROR_NONE : 
        sprintf(outLine, "\"status\" :\"OK\",\n");
        strcat(JIResponseBuffer, outLine);
        if ( reg ) {
            sprintf(outLine, "\"value\" : \"%.02f\"\n", reg->Value.fd);
            strcat(JIResponseBuffer, outLine);
        }
        sprintf(returnMessage, "\"Device %d Updated\"", InCommand->requests.writeRequest.index);
        break;
      case DEVICE_ERROR_REGISTER_NOT_FOUND :
        returnStatus = JIRESPONSE_STATUS_TYPE_ERROR;
        strcat(JIResponseBuffer, "\"status\":\"error\",\n");
        sprintf(returnMessage, "\"Device Register not found\"");
        break;
      case DEVICE_ERROR_DEVICE_NOT_FOUND :
        returnStatus = JIRESPONSE_STATUS_TYPE_ERROR;
        strcat(JIResponseBuffer, "\"status\":\"error\",\n");
        sprintf(returnMessage, "\"Device Not Found\"");
        break;
    }
    if ( i + 1 < n ) {
      strcat(JIResponseBuffer, "},\n");
    } else {
      strcat(JIResponseBuffer, "}\n");
    }
    
  }  
  strcat(JIResponseBuffer,"],\n"); 
  strcat(JIResponseBuffer, "\"message\":");
  strcat(JIResponseBuffer, returnMessage);
  strcat(JIResponseBuffer, ",\n");
  sprintf(outLine, "\"status\":\"%s\"\n", JIResponseStatusToString(returnStatus));
  strcat(JIResponseBuffer, outLine);
  strcat(JIResponseBuffer, "}\n");
}

/*******************************************************************************
 * Function : JIGetReadResponse
 *******************************************************************************/
JIResponse*
JIGetReadResponse
(
 JICommand*                             InCommand
)
{
  JIResponse*                           response;

  response = (JIResponse*)GetMemory(sizeof(JIResponse));
  if ( NULL == response ) {
    printf_safe("Malloc error - time to bail\n");
    exit(EXIT_FAILURE);
  }
  memset(response, 0x00, sizeof(JIResponse));
  response->command = InCommand->command;

  if ( InCommand->requests.readRequest.readType == JIREAD_TYPE_IDS ) {
    JIGetIDsResponse(InCommand, response);
    return response;
  } else if ( InCommand->requests.readRequest.readType == JIREAD_TYPE_REGNAMES ) {
    JIGetRegNamesResponse(InCommand, response);
  } else if ( InCommand->requests.readRequest.readType == JIREAD_TYPE_MESSAGES ) {
    JIGetMessagesResponse(InCommand, response);
    return response;
  } else if ( InCommand->requests.readRequest.readType == JIREAD_TYPE_ALL_DATA ) {
    JIGetDeviceDataResponse(InCommand, response);
    return response;
  }
  switch (InCommand->deviceType) {
    case JIDEVICE_TYPE_NONE :
      break;
    case JIDEVICE_TYPE_BBU :
      break;
    case JIDEVICE_TYPE_RECTIFIER :
      break;
    case JIDEVICE_TYPE_BAY :
      break;
    case JIDEVICE_TYPE_PANEL :
      break;
    case JIDEVICE_TYPE_RECTIFIER_12 :
      break;
    case JIDEVICE_TYPE_SMTEMP :
      break;
    case JIDEVICE_TYPE_CONFIG :
      JIGetConfigReadResponse(InCommand, response);
      break;
  }
  return response;
}

/*******************************************************************************
 * Function : JIGetIDsResponse
 *******************************************************************************/
void
JIGetIDsResponse
(
 JICommand*                             InCommand,
 JIResponse*                            InResponse
)
{
  if ( InCommand == NULL || InResponse == NULL ) {
    return;
  }

  char*                                 deviceType;
  CanDevice*                            device;
  int                                   deviceCount;
  char                                  outLine[129];

  deviceType = JIDeviceTypeToString(InCommand->deviceType);
  InResponse->command = InCommand->command;
  InResponse->deviceType = InCommand->deviceType;
  InResponse->status = JIRESPONSE_STATUS_TYPE_OK;
  InResponse->message[0] = 0x00;
  InResponse->values = JIResponseValueCreate();
  InResponse->values->type = InCommand->requests.readRequest.readType;

  deviceCount = GetDeviceTypeCount(deviceType);

  memset(JIResponseBuffer, 0x00, JIRESPONSE_BUFFER_SIZE_MAX);
  strcat(JIResponseBuffer, "{\n");
  strcat(JIResponseBuffer, "  \"command\" : \"read\",\n");
  strcat(JIResponseBuffer, "  \"type\"    : \"ids\",\n");
  sprintf(outLine, "  \"device\"  : \"%s\",\n", InCommand->deviceName);
  strcat(JIResponseBuffer, outLine);
  strcat(JIResponseBuffer, "  \"status\"  : \"OK\",\n");
  strcat(JIResponseBuffer, "  \"message\" : \"\",\n");
  strcat(JIResponseBuffer, "  \"values\"  : [\n");
  strcat(JIResponseBuffer, "     {\n");
  sprintf(outLine, "      \"type\"  : \"%s\",\n", deviceType);
  strcat(JIResponseBuffer, outLine);
  sprintf(outLine, "      \"value\" : %d,\n", deviceCount);
  strcat(JIResponseBuffer, outLine);
  strcat(JIResponseBuffer, "      \"ids\" : [\n");
  for ( device = GetFirstDevice(deviceType); device; ) {
    strcat(JIResponseBuffer, "        {\n");
    sprintf(outLine, "          \"name\"  : \"%s-%d\",\n", InCommand->deviceName, device->CanAddress + 1);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "          \"index\" : \"%d\"\n,", (device->CanAddress - device->deviceDefinition->canAddressStart)+ 1);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "          \"device\": \"%s\"\n", InCommand->deviceName);
    strcat(JIResponseBuffer, outLine);
    device = GetNextDevice(device);
    sprintf(outLine, "        }%s\n", device ? "," : "");
    strcat(JIResponseBuffer, outLine);
  }
  strcat(JIResponseBuffer, "      ]\n");
  strcat(JIResponseBuffer, "    }\n");
  strcat(JIResponseBuffer, "  ]\n");
  strcat(JIResponseBuffer, "}\n");

  InResponse->stringResponse = JIResponseBuffer;
}

/*******************************************************************************
 * Function : JIGetConfigReadResponse
 *******************************************************************************/
void
JIGetConfigReadResponse
(
 JICommand*                             InCommand,
 JIResponse*                            InResponse
)
{
  switch ( InCommand->requests.configRequest.readType ) {
    case JIREAD_TYPE_DEVICES:
      JIGetAllDevicesResponse(InCommand, InResponse);
      break;
    case JIREAD_TYPE_REGNAMES :
      JIGetRegNamesResponse(InCommand, InResponse);
      break;
    case JIREAD_TYPE_MESSAGES :
      JIGetMessagesResponse(InCommand, InResponse);
      break;
    default :
      break;
  }
}

/*******************************************************************************
 * Function : JIGetAllDevicesResponse
 *******************************************************************************/
void
JIGetAllDevicesResponse
(
 JICommand*                             InCommand,
 JIResponse*                            InResponse
)
{
  DeviceDef*                            deviceDef;
  char                                  outLine[129];
  char*                                 deviceType;

  memset(JIResponseBuffer, 0x00, JIRESPONSE_BUFFER_SIZE_MAX);
  deviceType = JIDeviceTypeToString(InCommand->deviceType);

  strcat(JIResponseBuffer, "{\n");
  strcat(JIResponseBuffer, " \"command\":\"read\",\n");
  sprintf(outLine, " \"device\" : \"%s\",\n",deviceType);
  strcat(JIResponseBuffer, outLine);
  strcat(JIResponseBuffer, " \"status\" : \"OK\",\n");
  strcat(JIResponseBuffer, " \"message\":\"\",\n");
  strcat(JIResponseBuffer, " \"value\": {\n");
  strcat(JIResponseBuffer, "  \"name\":\"devices\",\n");
  strcat(JIResponseBuffer, "  \"devices\": [\n");
  
  for ( deviceDef = mainDeviceDefs->defs; deviceDef; deviceDef = deviceDef->next ) {
    sprintf(outLine, "  { \"name\" : \"%s\", ", deviceDef->name);
    strcat(JIResponseBuffer, outLine);    
    sprintf(outLine, " \"desc\" : \"%s\", ",deviceDef->desc); 
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, " \"CANAddressStart\" : \"0x%02x\", ",deviceDef->canAddressStart); 
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, " \"ProtocolNumber\" : \"0x%02x\", ",deviceDef->protocolNumber); 
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, " \"MessageType\" : \"%d\", ",deviceDef->messageType); 
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "\"DisplayColumns\" : %d, ", deviceDef->displayColumns);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "\"displayType\" : %d }%s\n", deviceDef->DisplayType, deviceDef->next ? ", " : "");
    strcat(JIResponseBuffer, outLine);
  }
  strcat(JIResponseBuffer, "  ]\n");
  strcat(JIResponseBuffer, " }\n");
  strcat(JIResponseBuffer, "}\n");
  InResponse->stringResponse = JIResponseBuffer;
}

/*******************************************************************************
 * Function : JIGetRegNamesResponse
 *******************************************************************************/
void
JIGetRegNamesResponse
(
 JICommand*                             InCommand,
 JIResponse*                            InResponse
)
{
  char                                  outLine[129];
  DeviceDef*                            deviceDef;
  char*                                 requestedDevice;

  InResponse->command = InCommand->command;
  requestedDevice = InCommand->deviceName;
  InResponse->deviceType = InCommand->deviceType;
  memset(JIResponseBuffer, 0x00, JIRESPONSE_BUFFER_SIZE_MAX);
  strcat(JIResponseBuffer, "{\n");
  strcat(JIResponseBuffer, "\"command\":\"read\",\n");
  sprintf(outLine, "\"device\":\"%s\",\n", InCommand->deviceName);
  strcat(JIResponseBuffer, outLine);
  strcat(JIResponseBuffer, "\"status\":\"OK\",\n");
  strcat(JIResponseBuffer, "\"message\":\"\",\n");
  strcat(JIResponseBuffer, "\"values\": {\n");
  sprintf(outLine, " \"device\" : \"%s\",\n", requestedDevice); 
  strcat(JIResponseBuffer, outLine);
  strcat(JIResponseBuffer, "\"type\":\"regnames\",\n");
  strcat(JIResponseBuffer, "\"registers\":[\n");
  deviceDef = GetCanDeviceDefByName(requestedDevice);
  if ( NULL == deviceDef ) {
    return;
  }
  for (DeviceRegDef* regDef = deviceDef->regDefs->definitions; regDef; regDef = regDef->next ) {
    sprintf(outLine, "{ \"name\":\"%s\", ", regDef->name);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "\"group\":\"%s\", ", regDef->group);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "\"desc\":\"%s\", ", regDef->shortName);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "\"messageType\":\"0x%02x\", ", regDef->messageType);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "\"valueType\":\"0x%02x\", ", regDef->valueType);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "\"initValue\":\"%.02f\", ", regDef->initialValue.fd);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "\"displayLabel\":\"%s\" }%s\n", regDef->displayLabel, regDef->next ? "," : "");
    strcat(JIResponseBuffer, outLine);
  }
  strcat(JIResponseBuffer, "]\n");
  strcat(JIResponseBuffer, "}\n");
  strcat(JIResponseBuffer, "}\n");
  InResponse->stringResponse = JIResponseBuffer; 
}

/*******************************************************************************
 * Function : JIGetMessagesResponse
 *******************************************************************************/
void
JIGetMessagesResponse
(
 JICommand*                             InCommand,
 JIResponse*                            InResponse
)
{
  char                                  outLine[129];
  DeviceDef*                            deviceDef;

  InResponse->command = InCommand->command;
  InResponse->deviceType = InCommand->deviceType;
  memset(JIResponseBuffer, 0x00, JIRESPONSE_BUFFER_SIZE_MAX);
  strcat(JIResponseBuffer, "{\n");
  strcat(JIResponseBuffer, "\"command\":\"read\",\n");
  sprintf(outLine, "\"device\":\"%s\",\n", InCommand->deviceName);
  strcat(JIResponseBuffer, outLine);
  strcat(JIResponseBuffer, "\"status\":\"OK\",\n");
  strcat(JIResponseBuffer, "\"message\":\"\",\n");
  strcat(JIResponseBuffer, "\"values\": {\n");
  sprintf(outLine, " \"device\" : \"%s\",\n", InCommand->deviceName); 
  strcat(JIResponseBuffer, outLine);
  strcat(JIResponseBuffer, "\"type\":\"messages\",\n");
  strcat(JIResponseBuffer, "\"messages\":[\n");
  deviceDef = GetCanDeviceDefByName(InCommand->deviceName);
  if ( NULL == deviceDef ) {
    return;
  }

  for (DeviceMessageDef* messageDef = deviceDef->messageDefs->definitions; messageDef; messageDef = messageDef->next ) {
    sprintf(outLine, "{ \"messageType\":\"0x%02x\"\n, ", messageDef->msgType);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "\"messageName\" : \"%s\"\n,", messageDef->messageName);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "\"responses\": [\n");
    strcat(JIResponseBuffer, outLine);
    for ( int i = 0 ; i < messageDef->responsesCount; i++ ) {
        sprintf(outLine, "  \"0x%02x\"%s\n", messageDef->responses[i].responseType, i + 1 < messageDef->responsesCount ? "," : "");
        strcat(JIResponseBuffer, outLine);
    }
    sprintf(outLine, "] \n}%s\n", messageDef->next ? "," : "");
    strcat(JIResponseBuffer, outLine);
  }
  strcat(JIResponseBuffer, "]\n");
  strcat(JIResponseBuffer, "}\n");
  strcat(JIResponseBuffer, "}\n");
  InResponse->stringResponse = JIResponseBuffer; 
}

/*******************************************************************************
 * Function : JIGetDeviceDataResponse
 *******************************************************************************/
void
JIGetDeviceDataResponse
(
 JICommand*                             InCommand,
 JIResponse*                            InResponse
)
{  
  CanDevice*                            device;
  char                                  outLine[129];

  InResponse->command = InCommand->command;
  InResponse->deviceType = InCommand->deviceType;
  InResponse->status = JIRESPONSE_STATUS_TYPE_OK;
  InResponse->message[0] = 0x00;
  InResponse->values = JIResponseValueCreate();
  InResponse->values->type = InCommand->requests.readRequest.readType;

  device = FindDevice(InCommand->deviceName, InCommand->requests.readRequest.index);
  if ( NULL == device ) {
    memset(JIResponseBuffer, 0x00, JIRESPONSE_BUFFER_SIZE_MAX);
    strcat(JIResponseBuffer, "{\n");
    strcat(JIResponseBuffer, "  \"command\" : \"read\",\n");
    sprintf(outLine, "  \"device\"  : \"%s\",\n", InCommand->deviceName);
    strcat(JIResponseBuffer,outLine);
    strcat(JIResponseBuffer, "  \"type\"    : \"all\",\n");
    strcat(JIResponseBuffer, "  \"status\"  : \"ERROR\",\n");
    sprintf(outLine, "    \"message\"   : \"Device %s %d not found\"\n", InCommand->deviceName, InCommand->requests.readRequest.index);
    strcat(JIResponseBuffer, outLine);
    strcat(JIResponseBuffer, "}\n");
    InResponse->stringResponse = JIResponseBuffer;
    return;
  }
  memset(JIResponseBuffer, 0x00, JIRESPONSE_BUFFER_SIZE_MAX);
  strcat(JIResponseBuffer, "{\n");
  strcat(JIResponseBuffer, "  \"command\" : \"read\",\n");
  strcat(JIResponseBuffer, "  \"type\"    : \"all\",\n");
  strcat(JIResponseBuffer, "  \"status\"  : \"OK\",\n");
  sprintf(outLine, "  \"device\"  : \"%s\",\n", InCommand->deviceName);
  strcat(JIResponseBuffer, outLine);
  sprintf(outLine,         "  \"id\"      : \"%d\",\n", InCommand->requests.readRequest.index);
  strcat(JIResponseBuffer, outLine);
  strcat(JIResponseBuffer, "  \"values\"  : [\n");
  int n = GetDeviceRegCount(device->deviceDefinition->regDefs);
  for ( int i = 0 ; i < n ; i++ ) {
    CanReg*                     regValue = &(device->Registers[i]);
    strcat(JIResponseBuffer, "    { ");
    sprintf(outLine, " \"name\" : \"%s\",", regValue->registerDef->shortName);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, " \"group\": \"%s\",", regValue->registerDef->group);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, " \"value\": \"%.02f\"", regValue->Value.fd);
    strcat(JIResponseBuffer, outLine);
    sprintf(outLine, "}%s\n", i + 1 < n ? "," : "");
    strcat(JIResponseBuffer, outLine);
  }
  strcat(JIResponseBuffer, "  ]\n");
  strcat(JIResponseBuffer, "}\n");
  InResponse->stringResponse = JIResponseBuffer;
}

/*******************************************************************************
 * Function : JIResponseValueCreate
 *******************************************************************************/
JIResponseValue*
JIResponseValueCreate
()
{
  JIResponseValue*                      value;
 
  value = (JIResponseValue*)GetMemory(sizeof(JIResponseValue));
  memset(value, 0x00, sizeof(JIResponseValue));

  return value;
}

/*******************************************************************************
 * Function : JIDeviceTypeToString
 *******************************************************************************/
char*
JIDeviceTypeToString
(
 JIDeviceType                           InDeviceType
)
{
  switch (InDeviceType) {
    case JIDEVICE_TYPE_NONE :
      return "";
    case JIDEVICE_TYPE_RECTIFIER :
      return "REC";
    case JIDEVICE_TYPE_BAY :
      return "BAY";
    case JIDEVICE_TYPE_PANEL :
      return "PANEL";
    case JIDEVICE_TYPE_RECTIFIER_12 :
      return "REC12";
    case JIDEVICE_TYPE_SMTEMP :
      return "SMTEMP";
    case JIDEVICE_TYPE_BBU :
      return "BBU";
    case JIDEVICE_TYPE_CONFIG :
      return "CONFIG";
  }
  return "";
}

/*******************************************************************************
 * Function : JIProcessWrite
 *******************************************************************************/
void 
JIProcessWrite
(
 JICommand*                             InCommand,
 json_value*                            InJSONDoc
)
{
  char*                                 subCommand;
  char*                                 type;
  json_value*                           value;
  
  JIDumpJSONValue(InJSONDoc, 1, false);
  value = JIFindJSONValue(InJSONDoc, "subCommand");
  subCommand = value->u.string.ptr;
  
  value = JIFindJSONValue(InJSONDoc, "type");
  type = value->u.string.ptr;
  
  if ( strcmp(subCommand, "add") == 0 ) {
      
        if ( strcmp(type, "devicedef") == 0 ) {
            JIProcessDeviceDefAdd(InCommand, InJSONDoc);
            return;
        }
        if ( strcmp(type, "registerdef") == 0 ) {
            JIProcessRegisterDefAdd(InCommand, InJSONDoc);
            return;
        }
        if ( strcmp(type, "messageDef") == 0 ) {
            JIProcessMessageDefAdd(InCommand, InJSONDoc);
            return;
        }
    }
    JIProcessDeviceWrite(InCommand, InJSONDoc);
}

/*******************************************************************************
 * Function : JIProcessDeviceDefAdd
 *******************************************************************************/
void
JIProcessDeviceDefAdd
(
 JICommand*                             InCommand,
 json_value*                            InJSONDoc
)
{
    json_value*                         deviceValues;
    json_value*                         value;
    char*                               name;
    char*                               desc;
    bool                                b;
    int                                 canAddressStart;
    int                                 protocolNumber;
    int                                 messageType;
    int                                 displayType;
    int                                 displayColumns;
    char                                outLine[128];
    int                                 n, rvalue;
    
    deviceValues = JIFindJSONValue(InJSONDoc, "device");
    value = JIFindJSONValue(deviceValues, "name");
    if ( value && value->type == json_string ) {
        name = value->u.string.ptr;
    }
    value = JIFindJSONValue(deviceValues, "desc");
    if ( value && value->type == json_string ) {
        desc = value->u.string.ptr;
    }
    value = JIFindJSONValue(deviceValues, "CANAddressStart");
    if ( value && value->type == json_string ) {
        n = GetIntValueFromString(&b, value->u.string.ptr);
        if ( b ) {
            canAddressStart = n;
        }
    }
    value = JIFindJSONValue(deviceValues, "protocolNumber");
    if ( value && value->type == json_string ) {
        n = GetIntValueFromString(&b, value->u.string.ptr);
        if ( b ) {
            protocolNumber = n;
        }
    }
    value = JIFindJSONValue(deviceValues, "messageType");
    if ( value && value->type == json_string ) {
        n = GetIntValueFromString(&b, value->u.string.ptr);
        if ( b ) {
            messageType = n;
        }
    }
    value = JIFindJSONValue(deviceValues, "displayColumns");
    if ( value && value->type == json_string ) {
        n = GetIntValueFromString(&b, value->u.string.ptr);
        if ( b ) {
            displayColumns = n;
        }
    }
    value = JIFindJSONValue(deviceValues, "displayType");
    if ( value && value->type == json_string ) {
        n = GetIntValueFromString(&b, value->u.string.ptr);
        if ( b ) {
            displayType = n;
        }
    }
    rvalue = CreateNewDeviceDef(name, desc, canAddressStart, protocolNumber, messageType, displayColumns, displayType, mainDeviceDefs);
    memset(JIResponseBuffer, 0x00, sizeof(JIResponseBuffer));   
    strcat(JIResponseBuffer, "{\n");
    strcat(JIResponseBuffer, "  \"command\" : \"write\",\n");
    strcat(JIResponseBuffer, "  \"subCommand\" : \"add\",\n");
    strcat(JIResponseBuffer, "  \"type\" : \"devicedef\",\n");
    if ( rvalue == DEVICE_DEF_OK) {
        strcat(JIResponseBuffer, "  \"status\" : \"OK\",\n");
        sprintf(outLine, "Device definition %s added", name);
        strcat(JIResponseBuffer, "  \"message\" : \"");
        strcat(JIResponseBuffer, outLine);
        strcat(JIResponseBuffer, "\"\n");
    } else {
        strcat(JIResponseBuffer, "  \"status\" : \"FAIL\",\n");
        strcpy(outLine, DeviceDefErrorCodeToString(rvalue));
        strcat(JIResponseBuffer, "  \"message\" : \"");
        strcat(JIResponseBuffer, outLine);
        strcat(JIResponseBuffer, "\"\n");
    }
    strcat(JIResponseBuffer, "}\n");
}

/*******************************************************************************
 * Function : JIProcessRegisterDefAdd
 *******************************************************************************/
void
JIProcessRegisterDefAdd
(
 JICommand*                             InCommand,
 json_value*                            InJSONDoc
)
{
    
}

/*******************************************************************************
 * Function : JIProcessMessageDefAdd
 *******************************************************************************/
void
JIProcessMessageDefAdd
(
 JICommand*                             InCommand,
 json_value*                            InJSONDoc
)
{
    
}

/*******************************************************************************
 * Function : JIProcessDeviceWrite
 *******************************************************************************/
void
JIProcessDeviceWrite
(
 JICommand*                             InCommand,
 json_value*                            InJSONDoc
)
{
  json_value*                           indexValue;
  int                                   index;
  json_value*                           valueStruct = JIFindJSONValue(InJSONDoc, "value");
  if ( NULL == valueStruct ) {
    return;
  }
  InCommand->requests.writeRequest.valueStruct = valueStruct;
  indexValue = JIFindJSONValue(valueStruct, "index");
  if ( indexValue == NULL ) {
    return;
  }
  if ( indexValue->type != json_string ) {
    return;
  }
  index = atoi(indexValue->u.string.ptr);
  InCommand->requests.writeRequest.index = index;
}


/*******************************************************************************
 * Function : JIFindJSONValue
 *******************************************************************************/
json_value*
JIFindJSONValue
(
 json_value*                            InJSONDoc,
 char*                                  InName
)
{
  int                                   i;

  if ( NULL == InJSONDoc ) {
    return NULL;
  }
  for ( i = 0 ; i < InJSONDoc->u.object.length ; i++ ) {
    json_object_entry*                  value = &(InJSONDoc->u.object.values[i]);
    if ( strcmp(value->name, InName) == 0 ) {
      return value->value;
    }  
  }
  return NULL;
}

/*******************************************************************************
 * Function : JIFreeCommand
 *******************************************************************************/
void
JIFreeCommand
(
 JICommand*                             InCommand
)
{
  if ( NULL == InCommand ) {
    return;
  }
  FreeMemory(InCommand);
}

/*******************************************************************************
 * Function : FreeResponse
 ******************************************************************************/
void
JIFreeResponse
(
 JIResponse*                            InResponse
)
{
  if ( NULL == InResponse ) {
    return;
  }
  if ( InResponse->values ) {
    FreeMemory(InResponse->values);
  }
  FreeMemory(InResponse);
}

/*******************************************************************************
 * Function : JIGetJSONDevice
 * Purpose  : Return the value of the 'device' field in the topmost JSON object
 *******************************************************************************/
JIDeviceType
JIGetJSONDevice
(
 json_value*                            InJSONDoc,
 char*                                  InDeviceName,
 int                                    InDeviceNameLen
)
{
  int                                   i;
  if ( NULL == InJSONDoc ) {
    return JIDEVICE_TYPE_NONE;
  }
  
  for ( i = 0 ; i < InJSONDoc->u.object.length ; i++ ) {
    json_object_entry*                  value = &(InJSONDoc->u.object.values[i]);
    if ( strcmp(value->name, "device") == 0 ) {
      struct _json_value*               deviceValue;
      deviceValue = value->value;
      if ( deviceValue->type == json_string ) {
        char* str = deviceValue->u.string.ptr;
        strncpy(InDeviceName, str, InDeviceNameLen);
        InDeviceName[InDeviceNameLen] = 0x00;
        if ( strcmp(str, "REC") == 0 ) {
          return JIDEVICE_TYPE_RECTIFIER;
        } else if ( strcmp(str, "BAY") == 0 ) {
          return JIDEVICE_TYPE_BAY;
        } else if ( strcmp(str, "PANEL") == 0 ) {
          return JIDEVICE_TYPE_PANEL;
        } else if ( strcmp(str, "SMTEMP") == 0 ) {
          return JIDEVICE_TYPE_SMTEMP;
        } else  if ( strcmp(str, "REC12") == 0 ) {
          return JIDEVICE_TYPE_RECTIFIER_12;
        } else if ( strcmp(str, "BBU") == 0 ) {
          return JIDEVICE_TYPE_BBU;
        } else if ( strcmp(str, "CONFIG") == 0 ) {
          return JIDEVICE_TYPE_CONFIG;
        } else {
          return JIDEVICE_TYPE_NONE;
        }
      }
    }
  }
  return JIDEVICE_TYPE_NONE;
}

/*******************************************************************************
 * Function : JIGetJSONCommand
 * Purpose  : Return the value of the 'command' field in the topmost JSON object
 *******************************************************************************/
JICommandType
JIGetJSONCommand
(
 json_value*                            InJSONDoc
)
{
  int                                   i;
  if ( NULL == InJSONDoc ) {
    return JICOMMAND_TYPE_NONE;
  }
  
  for ( i = 0 ; i < InJSONDoc->u.object.length ; i++ ) {
    json_object_entry*                  value = &(InJSONDoc->u.object.values[i]);
    if ( strcmp(value->name, "command") == 0 ) {
      struct _json_value*               commandValue;
      commandValue = value->value;
      if ( commandValue->type == json_string ) {
        char* str = commandValue->u.string.ptr;
        if ( strcmp(str, "read") == 0 ) {
          return JICOMMAND_TYPE_READ;
        } else if ( strcmp(str, "write") == 0 ) {
          return JICOMMAND_TYPE_WRITE;
        } else if ( strcmp(str, "create") == 0 ) {
          return JICOMMAND_TYPE_CREATE;
        } else if ( strcmp(str, "remove") == 0 ) {
          return JICOMMAND_TYPE_REMOVE;
        } else {
          return JICOMMAND_TYPE_NONE;
        }
      }
    }
  }
  return JICOMMAND_TYPE_NONE;
}

/*******************************************************************************
 * Function : JIResponseStatusToString
 *******************************************************************************/
char*
JIResponseStatusToString
(
 JIResponseStatusType                   InStatus
)
{
  switch(InStatus) {
    case JIRESPONSE_STATUS_TYPE_NONE :
      return"";
    case JIRESPONSE_STATUS_TYPE_OK :
      return"OK";
    case JIRESPONSE_STATUS_TYPE_ERROR :
      return "Error";
  }
  return "";
}

/*******************************************************************************
 * Function : JIDumpJSONValue
 *******************************************************************************/
void
JIDumpJSONValue
(
 json_value*                            InJSONValue,
 int                                    InIndentSize,
 bool                                   InNeedTrailingComma
)
{
  int                                   i;

  if ( InJSONValue == NULL ) {
    return;
  }

  if ( InIndentSize > 0 ) {
    printf("%*s", InIndentSize, " ");
  }
  switch (InJSONValue->type) {
    case json_object :
      printf("\n");
      for ( i = 0 ; i < InJSONValue->u.object.length ; i++ ) {
        printf("%*s", InIndentSize + 2, " ");
        printf("\"%s\" : ", InJSONValue->u.object.values[i].name);
        JIDumpJSONValue(InJSONValue->u.object.values[i].value, 0, i + 1 < InJSONValue->u.object.length);
        if ( InIndentSize > 0 ) {
          printf("%*s", InIndentSize, " ");
        }
      }
      break;
    case json_array :
      printf("[\n");
      for ( i = 0 ; i < InJSONValue->u.array.length ; i++ ) {
        printf("%*s", InIndentSize + 2, " ");
        JIDumpJSONValue(InJSONValue->u.array.values[i], 0, i + 1 < InJSONValue->u.array.length);
        if ( InIndentSize > 0 ) {
          printf("%*s", InIndentSize, " ");
        }
        printf("]");
      }
      break;
    case json_integer :
      printf("%lld", InJSONValue->u.integer);
      break;
    case json_double :
      printf("%f", InJSONValue->u.dbl);
      break;
    case json_string :
      printf("\"%s\"", InJSONValue->u.string.ptr);
      break;
    case json_boolean :
      printf("%s", InJSONValue->u.boolean ? "true" : "false");
      break;
    case json_null :
      printf("null");
      break;
    case json_none :
      break;
  }
  printf("%s\n", InNeedTrailingComma ? "," : "");
}

/*******************************************************************************
 * Function : JICommandDump
 *******************************************************************************/
void
JICommandDump
(
 JICommand*                             InCommand
)
{
  char*                                 readType;
  char*                                 command;

  switch(InCommand->command) {
    case JICOMMAND_TYPE_NONE :
      command = "None";
      break;
    case JICOMMAND_TYPE_WRITE :
      command = "Write";
      break;
    case JICOMMAND_TYPE_CREATE :
      command = "Create";
      break;
    case JICOMMAND_TYPE_REMOVE :
      command = "Remove";
      break;
    case JICOMMAND_TYPE_READ :
      command = "Read";
      break;
  }

  printf_safe("JICommand\n");
  printf_safe("      Command  : %s(%d)\n", command, InCommand->command);
  printf_safe("      Device   : %s\n", InCommand->deviceName);
  switch(InCommand->command) {
    case JICOMMAND_TYPE_READ : 
      switch(InCommand->requests.readRequest.readType) {
        case JIREAD_TYPE_COUNTS : 
          readType = "Counts";
          break;
        case JIREAD_TYPE_IDS :
          readType = "IDs";
          break;
        case JIREAD_TYPE_ALL_DATA :
          readType = "All";
          break;
        case JIREAD_TYPE_REGNAMES :
          readType = "RegNames";
          break;
        case JIREAD_TYPE_MESSAGES :
          readType = "Messagess";
          break;
        case JIREAD_TYPE_DEVICES :
          readType = "Device";
          break;
        case JIREAD_TYPE_NONE :
          readType = "None";
          break;
      }
      printf_safe("     Read Type : %s(%d)\n", readType, InCommand->requests.readRequest.readType);
      printf_safe("         Index : %d\n", InCommand->requests.readRequest.index);
      printf_safe("  Read Request : %s\n", InCommand->requests.readRequest.readRequest);
      break;
    case JICOMMAND_TYPE_WRITE :
      printf_safe(" Write Request : %d\n", InCommand->requests.writeRequest.index);
      break;
    case JICOMMAND_TYPE_CREATE :
      printf_safe("Create Request : %d\n", InCommand->requests.createRequest.index); 
      break;
    case JICOMMAND_TYPE_REMOVE :
      break;
    case JICOMMAND_TYPE_NONE :
      break;
  }
}

/*******************************************************************************
 * Function : JIProcessRequeset
 *******************************************************************************/
void
JIProcessRequest
(
 json_value*                            InJSONDoc,
 JIResponse**                           InResponse
)
{
    JICommand*                          command;
    JIResponse*                         response;
    
    command = JICreateCommand(InJSONDoc);
    response = JIGetResponse(command);
    
    JIFreeCommand(command);

    *InResponse = response;
}

