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
string
MainProgramName = "cansimwsclient";

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
void
ProcessCommandLine
(int argc, char** argv);

void
MainDisplayHelp
();

/*****************************************************************************!
 * Function : NewFunction
 *****************************************************************************/
int
main
(int argc, char** argv)
{
  void*									retval;
  void*									retval2;

  ClientUserInputInit();
  ProcessCommandLine(argc, argv);
  ClientUserInputStartThread();
  pthread_join(ClientUserInputThreadID, &retval);
  pthread_join(ClientUserInputCommThreadID, &retval2);
  return EXIT_SUCCESS;
}

/*****************************************************************************!
 * Function : ProcessCommandLine
 *****************************************************************************/
void
ProcessCommandLine
(int argc, char** argv)
{
  int									i;
  string								command;

  for ( i = 1 ; i < argc ; i++ ) {
  	command = argv[i];

    if ( StringEqualsOneOf(command, "-h", "--help", NULL) ) {
      MainDisplayHelp();
      exit(EXIT_SUCCESS); 
	}

    if ( StringEqualsOneOf(command, "-a", "--address", NULL) ) {
	  i++;
	  if ( i == argc ) {
		fprintf(stderr, "%s is missing an address\n", command);
		MainDisplayHelp();
		exit(EXIT_FAILURE);
	  }
      ClientUserInputSetAddress(argv[i]);
      continue;
    }

    if ( StringEqualsOneOf(command, "-p", "--port", NULL) ) {
	  i++;
	  if ( i == argc ) {
	    fprintf(stderr, "%s is missing a port number\n", command);
	    MainDisplayHelp();
 	    exit(EXIT_FAILURE);
      }
	  ClientUserInputSetPort(argv[i]);
	  continue;
	}
    fprintf(stderr, "%s is an invalid command\n", command);
    MainDisplayHelp();
	exit(EXIT_FAILURE);
  }
}

/*****************************************************************************!
 * Function : MainDisplayHelp
 *****************************************************************************/
void
MainDisplayHelp
()
{
  printf("Usage : %s options\n", MainProgramName);
  printf("        -g, --help              Display this message\n");
  printf("        -a, --address address   Specify the ip address of the remote host\n");
  printf("        -p, --port portnumber   Specify the port of the the remote host\n");
}

