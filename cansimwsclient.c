/*****************************************************************************
 * FILE NAME    : cansimwsclient.c
 * DATE         : August 11 2020
 * PROJECT      : GBB CAN Simulator
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
#include <pthread.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "linenoise.h"
#include "String.h"
#include "FileUtils.h"
#include "MemoryManager.h"
#include "ClientUserInput.h"

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
 * Function : NewFunction
 *****************************************************************************/
int
main
(int argc, char** argv)
{
  void*									retval;

  ClientUserInputInit();
  pthread_join(ClientUserInputThreadID, &retval);
  return EXIT_SUCCESS;
}

