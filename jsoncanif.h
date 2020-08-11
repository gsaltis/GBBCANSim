/*******************************************************************************
 * FILE		: jsoncanif.h
 * DATE		: April 25 2018
 * PROJECT	:
 * COPYRIGHT	: Copyright (c) 2018 by Vertiv Company
 *******************************************************************************/
#ifndef _jsoncanif_h_
#define _jsoncanif_h_

/*******************************************************************************
 * Global Headers
 *******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <json.h>

/*******************************************************************************
 * Local Headers
 *******************************************************************************/
#include "CanMsg.h"

/*******************************************************************************
 * Exported Macros
 *******************************************************************************/
#define JI_READ_REQUEST_SIZE_MAX	12
#define JI_RECTNAME_SIZE_MAX		16
#define JI_RESPONSE_MESSAGE_SIZE	128
#define JIDEVICE_NAME_SIZE_MAX		16

/*******************************************************************************
 * Exported Type : JICommandType
 *******************************************************************************/
typedef enum 
{
  JICOMMAND_TYPE_NONE			= 0,
  JICOMMAND_TYPE_READ,
  JICOMMAND_TYPE_WRITE,
  JICOMMAND_TYPE_CREATE,
  JICOMMAND_TYPE_REMOVE
} JICommandType;

/*******************************************************************************
 * Exported Type : JISubCommandType
 *******************************************************************************/
typedef enum
{
  JISUBCOMMAND_TYPE_NONE                = 0,
  JISUBCOMMAND_TYPE_ADD,
  JISUBCOMMAND_TYPE_REMOVE,
  JISUBCOMMAND_TYPE_UPDATE
} JISubCommandType;

/*******************************************************************************
 * Exported Type : JIDeviceType 
 *******************************************************************************/
typedef enum
{
  JIDEVICE_TYPE_NONE			= 0,
  JIDEVICE_TYPE_RECTIFIER,
  JIDEVICE_TYPE_BAY,
  JIDEVICE_TYPE_PANEL,
  JIDEVICE_TYPE_RECTIFIER_12,
  JIDEVICE_TYPE_SMTEMP,
  JIDEVICE_TYPE_BBU,
  JIDEVICE_TYPE_CONFIG
} JIDeviceType;

/*******************************************************************************
 * Exported Type : JIReadType 
 *******************************************************************************/
typedef enum
{
  JIREAD_TYPE_NONE			= 0,
  JIREAD_TYPE_COUNTS,
  JIREAD_TYPE_IDS,
  JIREAD_TYPE_ALL_DATA,
  JIREAD_TYPE_REGNAMES,
  JIREAD_TYPE_MESSAGES,
  JIREAD_TYPE_DEVICES
} JIReadType;

/*******************************************************************************
 * Exported Type : JIResponseType
 *******************************************************************************/
typedef enum
{
  JIRESPONSE_TYPE_NONE			= 0,
  JIRESPONSE_TYPE_COUNT,
  JIRESPONSE_TYPE_IDS,
  JIRESPONSE_TYPE_VALUES
} JIResponseType;

/*******************************************************************************
 * Exported Type : JIResponseStatusType
 *******************************************************************************/
typedef enum 
{
  JIRESPONSE_STATUS_TYPE_NONE		= 0,
  JIRESPONSE_STATUS_TYPE_OK,
  JIRESPONSE_STATUS_TYPE_ERROR
} JIResponseStatusType;

/*******************************************************************************
 * Exported Type : JIWriteValue
 *******************************************************************************/
typedef struct _JIWriteValue {
 
  CanMsgDef*				definition;
  float					value; 
  struct _JIWriteValue*			next;
} JIWriteValue;

/*******************************************************************************
 * Exported Type : JIRegisterValueResponse
 * Purpose       : Returns the data written after a write request along with the
 *                 status of the write (OK|ERROR)
 *******************************************************************************/
typedef struct _JIRegisterResponseValue {
 
  JIWriteValue*				writeValue;
  int					status;
  struct JIRegisterResponseValue*	next; 
} JIRegisterResponseValue;

/*******************************************************************************
 * Exported Type : JIIDResponse
 *******************************************************************************/
typedef struct _JIIDResponse 
{
  JIDeviceType				deviceType;
  char					deviceName[JIDEVICE_NAME_SIZE_MAX+1];
  int					deviceID;
  struct _JIIDResponse*			next;
} JIIDResponse;

/*******************************************************************************
 * Exported Type : JIReponseValue
 *******************************************************************************/
typedef struct _JIResponseValue
{
  JIResponseType			type;
  union 
  {
    int					rectifierCounts;
    JIIDResponse*			ids;
    JIRegisterResponseValue*		writeValues;
  } ResponseData;
} JIResponseValue;

/*******************************************************************************
 * Exported Type : JICommand
 *******************************************************************************/
typedef struct 
{
  JICommandType				                 command;
  char					                       deviceName[STRINGLEN+1];
  JIDeviceType			                   deviceType;
  union {
    struct {
      JIReadType			                  readType;
      int				                        index;
      char				                      readRequest[JI_READ_REQUEST_SIZE_MAX+1];
    } readRequest;
    struct {
      JIReadType			                  readType;
      char*     			                  device;
    } configRequest;
    struct {
      int				                        index;
      json_value*			                  valueStruct;
    } writeRequest;
    struct {
      int				                        index;
    } createRequest;
    struct {
      int                               index;
    } removeRequest;
    struct { 
      int				                        index;
      JIWriteValue*			                element;
    };
  } requests;

} JICommand;

/*******************************************************************************
 * Exported Type : JIResponse
 *******************************************************************************/
typedef struct
{
  JICommandType				command;
  JISubCommandType                      subCommnad;
  JIReadType				deviceType;
  JIResponseStatusType			status;
  char					message[JI_RESPONSE_MESSAGE_SIZE+1];
  JIResponseValue*			values;
  char*					stringResponse;
} JIResponse;

/*******************************************************************************
 * Exported Functions
 *******************************************************************************/
void
JIProcessRequest
(json_value* INJSONDoc, JIResponse** InResponse);

JICommand*
JICreateCommand
(json_value* InJSONDoc);

void
JIFreeCommand
(JICommand* InCommand);

JIResponse*
JIGetResponse
(JICommand* InCommand);

void
JIFreeResponse
(JIResponse* InResponse);

char*
JIDeviceTypeToString
(JIDeviceType InDeviceType);

char*
JIResponseStatusToString
(JIResponseStatusType InStatus);

void
JIDumpJSONValue
(json_value* InJSONValue, int InIndentSize, bool InNeedTrailingComma);

void
JICommandDump
(JICommand* InCommand);

#endif // _jsoncanif_h_
