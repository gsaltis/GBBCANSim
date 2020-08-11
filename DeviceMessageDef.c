/*******************************************************************************
 * FILE         : DeviceMessageDef.c
 * DATE         : September 24 2018
 * PROJECT      : CanSim
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/

/*******************************************************************************!
 * Global Headers
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*******************************************************************************!
 * Local Headers
 *******************************************************************************/
#include "DeviceMessageDef.h"
#include "MemoryManager.h"
#include "DefFileToken.h"
#include "ThreadSafePrint.h"
#include "DeviceDef.h"

/*******************************************************************************!
 * Function : CreateDeviceMessageDef
 *******************************************************************************/
DeviceMessageDef*
CreateDeviceMessageDef
()
{
    DeviceMessageDef*                   messageDef;
    
    messageDef = (DeviceMessageDef*)GetMemory(sizeof(DeviceMessageDef));
    memset(messageDef, 0x00, sizeof(DeviceMessageDef));
    
    return messageDef;
}

/*******************************************************************************!
 * Function : DestroyDeviceMessageDef
 *******************************************************************************/
void
DestroyDeviceMessageDef
(
 DeviceMessageDef*                      InMessageDef
)
{
    if ( NULL == InMessageDef ) {
        return;
    }
    FreeMemory(InMessageDef);
}

/*******************************************************************************!
 * Function : DumpDeviceMessageDef
 *******************************************************************************/
void
DumpDeviceMessageDef
(
 DeviceMessageDef*                      InMessageDef,
 int                                    InIndent
)
{
    int i;
    printf_safe("%*s          MESSAGE DEF :\n", InIndent, InIndent > 0 ? " " : "");
    printf_safe("%*s         Message Type : %d\n", InIndent, InIndent > 0 ? " " : "", InMessageDef->msgType);
    printf_safe("%*s       Response Count : %d\n", InIndent, InIndent > 0 ? " " : "", InMessageDef->responsesCount);
    printf_safe("%*s            Responses :\n", InIndent, InIndent > 0 ? " " : "");
    for ( i = 0 ; i < InMessageDef->responsesCount ; i++ ) {
        printf_safe("%*s          Response %2d : %d\n", InIndent, InIndent > 0 ? " " : "", i + 1, InMessageDef->responses[i].responseType);
    }
}

/*******************************************************************************!
 * Function : DumpDeviceMessageDefShort
 *******************************************************************************/
void
DumpDeviceMessageDefShort
(DeviceMessageDef* InMessageDef, int InIndent)
{
  int                                   i;
  if ( InIndent > 0 ) {
    printf_safe("%*s", InIndent, " ");
  }    
  printf_safe("%4d  %2d [", InMessageDef->msgType, InMessageDef->responsesCount);
  for ( i = 0 ; i < InMessageDef->responsesCount ; i++ ) {
    printf_safe("%4d", InMessageDef->responses[i].responseType);
    if ( i + 1 != InMessageDef->responsesCount ) {
      printf_safe(" ");
    }
  }
  printf_safe("]\n");
}

/*******************************************************************************!
 * Function : DumpDeviceMessageDefListShort
 *******************************************************************************/
void
DumpDeviceMessageDefListShort
(DeviceMessageDefList* InMessageDefs, int InIndent)
{
  DeviceMessageDef*                     messageDef;

  if ( NULL == InMessageDefs ) {
    return;
  }
  for ( messageDef = InMessageDefs->definitions; messageDef; messageDef = messageDef->next ) {
    DumpDeviceMessageDefShort(messageDef, InIndent);
  }
}

/*******************************************************************************!
 * Function : DumpDeviceMessageDefList
 *******************************************************************************/
void
DumpDeviceMessageDefList
(
 DeviceMessageDefList*                  InMessageDefs,
 int                                    InIndent
)
{
    DeviceMessageDef* messageDef;
    if ( NULL == InMessageDefs ) {
        return;
    }
    printf_safe("%*s         MESSAGE DEFS :\n", InIndent, InIndent > 0 ? " " : "");
    for ( messageDef = InMessageDefs->definitions; messageDef; messageDef = messageDef->next ) {
        DumpDeviceMessageDef(messageDef, InIndent+4);
        if ( messageDef->next ) {
            printf_safe("\n");
        } 
    }
}

/*******************************************************************************!
 * Function : DestroyDeviceMessageDeviceList
 *******************************************************************************/
void
DestroyDeviceMessageDefList
(
 DeviceMessageDefList*                  InMessageDefs
)
{
    DeviceMessageDef*                   messageDef;
    DeviceMessageDef*                   nextMessageDef;
    
    if ( NULL == InMessageDefs ) {
        return;
    }
    
    messageDef = InMessageDefs->definitions;
    while (messageDef) {
        nextMessageDef = messageDef->next;
        DestroyDeviceMessageDef(messageDef);
        messageDef = nextMessageDef;
    }
    FreeMemory(InMessageDefs);
}

/*******************************************************************************!
 * Function : CreateDeviceMessageDefList
 *******************************************************************************/
DeviceMessageDefList*
CreateDeviceMessageDefList
()
{
    DeviceMessageDefList*               messageDefs;

    messageDefs = (DeviceMessageDefList*)GetMemory(sizeof(DeviceMessageDefList));
    memset(messageDefs, 0x00, sizeof(DeviceMessageDefList));
    return messageDefs;
}    

/*******************************************************************************!
 * Function : DeviceMessageDefAddResponseType
 *******************************************************************************/
void
DeviceMessageDefAddResponseType
(
 DeviceMessageDef*                      InMessageDef,
 int                                    InMessageType
)
{
    if ( NULL == InMessageDef ) {
        return;
    }
    
    if ( InMessageDef->responsesCount >= MAXRESP ) {
        return;
    }
    
    InMessageDef->responses[InMessageDef->responsesCount].responseType = InMessageType;
    InMessageDef->responsesCount++;
}

/*******************************************************************************!
 * Function : AddDeviceMessageDef
 *******************************************************************************/
void
AddDeviceMessageDef
(
 DeviceMessageDefList*                  InMessageDefs,
 DeviceMessageDef*                      InMessageDef
)
{
    if ( NULL == InMessageDefs || NULL == InMessageDef ) {
        return;
    }
    if ( InMessageDefs->definitions == NULL ) {
        InMessageDefs->definitions = InMessageDef;
        InMessageDefs->lastDefinition = InMessageDef;
    } else {
        InMessageDefs->lastDefinition->next = InMessageDef;
        InMessageDefs->lastDefinition = InMessageDef;
    }
}

/*******************************************************************************!
 * Function : GetDeviceMessageCount
 *******************************************************************************/
int
GetDeviceMessageCount
(
 DeviceMessageDefList*                  InMessageDefs
)
{
    int                                 i;
    DeviceMessageDef*                   messageDef;
    
    if ( NULL == InMessageDefs ) {
        return 0;
    }
    
    for ( messageDef = InMessageDefs->definitions, i = 0 ;
          messageDef;
          messageDef = messageDef->next, i++) {}
    
    return i;
}

/*******************************************************************************!
 * Function : FindMessageDefByMessageType
 *******************************************************************************/
DeviceMessageDef*
FindMessageDefByMessageType
(
 DeviceMessageDefList*                  InMessageDefs,
 int                                    InMessageType
)
{
    DeviceMessageDef*               messageDef;
    
    for ( messageDef = InMessageDefs->definitions; messageDef; messageDef = messageDef->next ) {
        if ( messageDef->msgType == InMessageType ) {
            return messageDef;
        }
    }
    return NULL;
}

/*****************************************************************************!
 * Function : DeviceMessageDefFromJSON
 *****************************************************************************/
DeviceMessageDef*
DeviceMessageDefFromJSON
(json_value* InValue)
{
  int						i;
  DeviceMessageDef*				message;
  json_value*					responseArray;

  if ( NULL == InValue ) {
    return NULL;
  }
  message = (DeviceMessageDef*)GetMemory(sizeof(DeviceMessageDef));
  memset(message, 0x00, sizeof(DeviceMessageDef));

  message->msgType = JSONIFGetInt(InValue, "request");
  message->messageName = JSONIFGetString(InValue, "name");

  responseArray = JSONIFGetArray(InValue, "responses");
  if ( NULL == responseArray ) {
    return message;
  }
  message->responsesCount = responseArray->u.array.length;
  if ( message->responsesCount + 1 >= MAXRESP ) {
    message->responsesCount = MAXRESP;
  }
  for (i = 0; i < message->responsesCount; i++) {
    DeviceMessageResponseSetFromJSON(&(message->responses[i]), responseArray->u.array.values[i]);
  }
  return message;
}

/*****************************************************************************!
 * Function : DeviceMessageDefToJSON 
 *****************************************************************************/
string
DeviceMessageDefToJSON
(DeviceMessageDef* InMessageDef, bool InIncludeResponses)
{
  string				response;
  char					s[16];
  int					i;

  response = StringCopy("    {\n");
  response = StringConcatTo(response, "      \"name\" : \"");
  response = StringConcatTo(response, InMessageDef->messageName);
  response = StringConcatTo(response, "\",\n");

  response = StringConcatTo(response, "      \"deviceDefName\" : \"");
  response = StringConcatTo(response, InMessageDef->deviceDefParent->name);
  response = StringConcatTo(response, "\",\n");

  response = StringConcatTo(response, "      \"request\" : ");
  sprintf(s, "%d", InMessageDef->msgType);
  response = StringConcatTo(response, s);

  if ( InIncludeResponses ) {
    response = StringConcatTo(response, ",\n");
    response = StringConcatTo(response, "      \"responses\" : ");
    response = StringConcatTo(response, "      [ \n");
    for ( i = 0; i < InMessageDef->responsesCount; i++ ) {
      response = StringConcatTo(response, "        { \"responsetype\" : ");
      sprintf(s, "%d, ", InMessageDef->responses[i].responseType);
      response = StringConcatTo(response, s);
      response = StringConcatTo(response, " \"name\" : \"");
      response = StringConcatTo(response, InMessageDef->responses[i].responseDef->name);
      response = StringConcatTo(response, "\" }");      
      if ( i + 1 < InMessageDef->responsesCount ) {
        response = StringConcatTo(response, ",\n");
      } else {
        response = StringConcatTo(response, "\n");
      }
    }
    response = StringConcat(response, "]");
  }
  response = StringConcatTo(response, "\n    }");
  return response;
}

/*****************************************************************************!
 * Function : DeviceMessageDefListToJSON
 *****************************************************************************/
string
DeviceMessageDefListToJSON
(DeviceMessageDefList* InMessageList, bool InIncludeResponses)
{
  string				response;
  DeviceMessageDef*			messageDef;
  string				s;

  response = StringCopy("  [\n");
  for ( messageDef = InMessageList->definitions; messageDef; messageDef = messageDef->next ) {
    s = DeviceMessageDefToJSON(messageDef, InIncludeResponses);
    response = StringConcatTo(response, s);
    FreeMemory(s);
    if ( messageDef->next ) {
      response = StringConcatTo(response, ",\n");
    } else {
      response = StringConcatTo(response, "\n");
    }
  }
  response = StringConcatTo(response, "\n  ]");
  return response;
}

/*****************************************************************************!
 * Function : DeviceMessageResponseSetFromJSON
 *****************************************************************************/
void
DeviceMessageResponseSetFromJSON
(DeviceMessageResponse* InResponse, json_value* InValue)
{
  bool					b;
  if ( InResponse == NULL ) {
    return;
  }
  InResponse->responseType = GetIntValueFromString(&b, InValue->u.string.ptr);
}

