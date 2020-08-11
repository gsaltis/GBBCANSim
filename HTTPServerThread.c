/*****************************************************************************
 * FILE NAME    : HTTPServerThread.c
 * DATE         : April 09 2020
 * PROJECT      : Bay Simulator
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
#include <mongoose.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "HTTPServerThread.h"
#include "main.h"
#include "String.h"
#include "MemoryManager.h"
#include "WebSocketIF.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
pthread_t
HTTPServerThreadID;

string
s_http_port = NULL;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
static void*
HTTPServerThread
(void* InArgs);

void
HTTPEventHandler
(struct mg_connection* nc, int ev, void* p);

/*****************************************************************************!
 * Function : HTTPServerThreadInit
 *****************************************************************************/
void
HTTPServerThreadInit
()
{
  pthread_create(&HTTPServerThreadID, NULL, HTTPServerThread, NULL);
}

/*****************************************************************************!
 * Function : HTTPServerThread
 *****************************************************************************/
void*
HTTPServerThread
(void* InArgs)
{
  struct mg_connection *              nc;
  struct mg_mgr                       mgr;
  if ( MainVerbose ) {
    printf("Starting server thread on port %s:%s\n", WebSocketIFAddress, s_http_port);
  }
  mg_mgr_init(&mgr, NULL);

  nc = mg_bind(&mgr, s_http_port, HTTPEventHandler);
  if (nc == NULL) {
    printf("Failed to create http listener\n");
    return (void*)1;
  }
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = MainDefaultWebDirectory;
  s_http_server_opts.enable_directory_listing = "yes";

  for (;;) {
    mg_mgr_poll(&mgr, 20);            
  }
}

/*****************************************************************************!
 * Function : HTTPEventHandler
 *****************************************************************************/
void
HTTPEventHandler
(struct mg_connection* nc, int ev, void* p)
{
  if (ev == MG_EV_HTTP_REQUEST) {
    mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
  }
}

/*****************************************************************************!
 * Function : HTTPServerSetPort
 *****************************************************************************/
void
HTTPServerSetPort
(string InPortNumber)
{
  if ( InPortNumber == NULL ) {
    return;
  }

  if ( s_http_port ) {
    FreeMemory(s_http_port);
  }
  s_http_port = StringCopy(InPortNumber);
}

