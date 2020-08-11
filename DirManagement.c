/*******************************************************************************
 * FILE         : DirManagement.c
 * DATE         : September 26 2018
 * PROJECT      : GBB CAN Simulator
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/
 
/*******************************************************************************!
 * Global Headers
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*******************************************************************************!
 * Local Headers
 *******************************************************************************/
#include "DirManagement.h"

/*******************************************************************************!
 * Local Data
 *******************************************************************************/
static const char*
#if DEVELOPMENT
DMBaseDirectoryName = "./";
#else
DMBaseDirectoryName = "/usr/emerson/GBBSim/";
#endif

static const char*
DMDeviceDefsFileName = "devicedefs.txt";

static const char*
DMDeviceDataFileName = "devices.db";

static const char*
DMDeviceProtocolFileName = "protocols.txt";

uint16_t
DMMinFileLength;

/*******************************************************************************!
 * Local Functions
 *******************************************************************************/
static char*
DMGetFileName
(char* InReturnFileName, const char* InFileName, uint16_t InMaxFileNameLength);

/*******************************************************************************!
 * Function : DirManagementInitialize
 *******************************************************************************/
void
DirManagementInitialize
()
{
    DMMinFileLength = strlen(DMBaseDirectoryName) + 64;
}

/*******************************************************************************!
 * Function : GetDeviceDefsFileName
 *******************************************************************************/
char*
GetDeviceDefsFileName
(
 char*                                  InFileName,
 uint16_t                               InMaxFileNameLength
)
{
    return DMGetFileName(InFileName, DMDeviceDefsFileName, InMaxFileNameLength);
}

/*******************************************************************************!
 * Function : GetDeviceDataFileName
 *******************************************************************************/
char*
GetDeviceDataFileName
(
 char*                                  InFileName,
 uint16_t                               InMaxFileNameLength
)
{
    return DMGetFileName(InFileName, DMDeviceDataFileName, InMaxFileNameLength);
}

/*******************************************************************************!
 * Function : GetDeviceProtocolFileName
 *******************************************************************************/
char*
GetDeviceProtocolFileName
(
 char*                                  InFileName,
 uint16_t                               InMaxFileNameLength
)
{
    return DMGetFileName(InFileName, DMDeviceProtocolFileName, InMaxFileNameLength);
}

/*******************************************************************************!
 * Function : DMGetFileName
 *******************************************************************************/
char*
DMGetFileName
(
 char*                                  InReturnFileName,
 const char*                            InFileName,
 uint16_t                               InMaxFileNameLength
)
{
    if ( NULL == InReturnFileName || InMaxFileNameLength < DMMinFileLength ) {
        return NULL;
    }
    strcpy(InReturnFileName, DMBaseDirectoryName);
    strcat(InReturnFileName, InFileName);
    return InReturnFileName;
}
