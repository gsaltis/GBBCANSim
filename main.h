/*******************************************************************************
 * FILE                : main.h
 * DATE                : September 26 2018
 * COPYRIGHT       : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/
#ifndef _main_h_
#define _main_h_

/*******************************************************************************
 * Global Headers
 *******************************************************************************/
#include <stdbool.h>
#include <mongoose.h>

/*******************************************************************************
 * Local Headers
 *******************************************************************************/
#include "DeviceDef.h"
#include "FuseBreakerType.h"
#include "PanelType.h"
#include "BayType.h"
#include "WebConnection.h"
#include "String.h"

/*******************************************************************************
 * Local Data
 *******************************************************************************/
extern DeviceDefList* mainDeviceDefs;
extern bool MainVerbose;
extern struct mg_serve_http_opts
s_http_server_opts;

extern FuseBreakerType*
MainFuseBreakerTypes;

extern PanelType*
MainPanelTypes;

extern BayType*
MainBayTypes;

extern WebConnectionList*
mainWebConnections;

extern bool
MainMonitorWebRequest;

extern bool
MainSupportRectifiers;

extern string
MainDefaultWebDirectory;

#endif
