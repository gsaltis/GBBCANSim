/*****************************************************************************
 * FILE NAME    : PanelConnection.c
 * DATE         : 
 * PROJECT      : 
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "PanelConnection.h"
#include "MemoryManager.h"
#include "JSONIF.h"
#include "Bay.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/

/*****************************************************************************!
 * Function : CreatePanelConnection
 *****************************************************************************/
PanelConnection*
CreatePanelConnection
(int InFromPanel, int InToPanel)
{
  PanelConnection*			connection;
  int					n;

  if ( InFromPanel == 0 || InToPanel == 0 ) {
    return NULL;
  }

  n = sizeof(PanelConnection);
  connection = (PanelConnection*)GetMemory(n);
  memset(connection, 0x00, n);

  connection->fromPanel = InFromPanel;
  connection->toPanel = InToPanel; 
  return connection;
}

/*****************************************************************************!
 * Function : PanelConnectionDestroy
 *****************************************************************************/
void
PanelConnectionDestroy
(PanelConnection* InConnection)
{
  if ( InConnection == NULL ) {
    return;
  }
  FreeMemory(InConnection);
}

/*****************************************************************************!
 * Function : PanelConnectionFromJSONString
 *****************************************************************************/
PanelConnection*
PanelConnectionFromJSONString
(json_value* InValue)
{
  int					toValue, fromValue;

  if ( InValue == NULL ) {
    return NULL;
  }
  toValue = JSONIFGetInt(InValue, "topanel");
  fromValue = JSONIFGetInt(InValue, "frompanel");
  if ( 0 == toValue || 0 == fromValue ) {
    return NULL;
  }
  return CreatePanelConnection(fromValue, toValue);  
}

/*****************************************************************************!
 * Function : PanelConnectionToJSONString
 *****************************************************************************/
string
PanelConnectionToJSONString
(PanelConnection* InConnection, int InIndent)
{
  string				returnString;
  char					s2[16];

  if ( InConnection == NULL ) {
    return NULL;
  }
  returnString = NULL;
  if ( InIndent > 0 ) {
    returnString = StringFill(' ', InIndent);
  }

  if ( returnString ) {
    returnString = StringConcatTo(returnString, "{ \"frompanel\" : ");
  } else {
    returnString = StringCopy(" \"frompanel\" : ");
  }
  sprintf(s2, "%d", InConnection->fromPanel );
  returnString = StringConcatTo(returnString, s2);
  
  returnString = StringConcatTo(returnString, ", \"topanel\" : ");
  sprintf(s2, "%d", InConnection->toPanel);
  returnString = StringConcatTo(returnString, s2);
  returnString = StringConcatTo(returnString, " }");

  return returnString;
}

/*****************************************************************************!
 * Function : PanelConnectionAppend
 *****************************************************************************/
void
PanelConnectionAppend
(PanelConnection* InHead, PanelConnection* InConnection)
{
  PanelConnection*			connection;

  if ( InHead == NULL || InConnection == NULL ) {
    return;
  }
  connection = InHead;
  while ( connection->next ) {
   connection = connection->next;
  }
  connection->next = InConnection;
}

/*****************************************************************************!
 * Function : PanelConnectionsDestroy
 *****************************************************************************/
void
PanelConnectionsDestroy
(PanelConnection* InHead)
{
  PanelConnection*			connection;
  PanelConnection*			connectionNext;

  if ( InHead == NULL ) {
    return;
  }
  connection = InHead;
  while (connection) {
    connectionNext = connection->next;
    PanelConnectionDestroy(connection);
    connection = connectionNext;
  }
}

/*****************************************************************************!
 * Function : PanelConnectionsToJSONString
 *****************************************************************************/
string
PanelConnectionsToJSONString
(PanelConnection* InConnection, int InIndent)
{
  string				returnString;
  PanelConnection*			connection;

  returnString = NULL;
  if ( InIndent ) {
    returnString = StringFill(' ', InIndent);
  }

  if ( returnString ) {
    returnString = StringConcatTo(returnString , "[\n");
  } else {
    returnString = StringCopy("[\n");
  }
  connection = InConnection;
  while (connection) {
    returnString = StringConcatTo(returnString, PanelConnectionToJSONString(connection, InIndent + 2));
    connection = connection->next;
    if (connection) {
      returnString = StringConcatTo(returnString, ",");
    }
    returnString = StringConcatTo(returnString, "\n");
  }
  returnString = StringConcatTo(returnString, "]");
  return returnString;
}

