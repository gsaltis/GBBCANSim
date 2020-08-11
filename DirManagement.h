/*******************************************************************************
 * FILE         : DirManagement.h
 * DATE         : September 26 2018
 * PROJECT      : GBB CAN Simulator
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/
#ifndef _dirmanagement_h_
#define _dirmanagement_h_

/*******************************************************************************!
 * Global Headers
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*******************************************************************************!
 * Exported Functions
 *******************************************************************************/
void
DirManagementInitialize
();

char*
GetDeviceDefsFileName
(char* InFileName, uint16_t InMaxFileNameLength);

char*
GetDeviceDataFileName
(char* InFileName, uint16_t InMaxFileNameLength);

char*
GetDeviceProtocolFileName
(char* InFileName, uint16_t InMaxFileNameLength);

#endif