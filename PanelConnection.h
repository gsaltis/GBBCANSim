/*****************************************************************************
 * FILE NAME    : PanelConnection.h
 * DATE         : 
 * PROJECT      : 
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
#ifndef _panelconnection_h_
#define _panelconnection_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <json.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "String.h"

/*****************************************************************************!
 * Exported Type : PanelConnection
 *****************************************************************************/
struct _PanelConnection
{
  int				fromPanel;
  int				toPanel;
  struct _Bay*			bay;
  struct _PanelConnection*	next;
};
typedef struct _PanelConnection PanelConnection;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
PanelConnection*
CreatePanelConnection
(int InFromPanel, int InToPanel);

void
PanelConnectionDestroy
(PanelConnection* InConnection);

PanelConnection*
PanelConnectionFromJSONString
(json_value* InValue);

string
PanelConnectionToJSONString
(PanelConnection* InConnection, int InIndent);

void
PanelConnectionAppend
(PanelConnection* InHead, PanelConnection* InConnection);

void
PanelConnectionsDestroy
(PanelConnection* InHead);

string
PanelConnectionsToJSONString
(PanelConnection* InConnection, int InIndent);

#endif /* _panelconnection_h_*/
