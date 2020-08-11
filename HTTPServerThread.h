/*****************************************************************************
 * FILE NAME    : HTTPServerThread.h
 * DATE         : April 09 2020
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
#ifndef _httpserverthread_h_
#define _httpserverthread_h_

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "String.h"

/*****************************************************************************!
 * Exported Macros
 *****************************************************************************/

/*****************************************************************************!
 * Exported Data
 *****************************************************************************/
extern pthread_t
HTTPServerThreadID;

extern string
s_http_port;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/
void
HTTPServerThreadInit
();

void
HTTPServerSetPort
(string InPortNumber);

#endif /* _httpserverthread_h_*/
