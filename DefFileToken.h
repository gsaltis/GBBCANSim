/*******************************************************************************
 * FILE         : DefFileToken.h
 * DATE         : September 26 2018
 * PROJECT      : GBB CAN Simulator
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/
 #ifndef _deffiletoken_h_
 #define _deffiletoken_h_
 
/*******************************************************************************!
 * Global Headers
 *******************************************************************************/
#include <stdio.h>
#include <stdint.h>

/*******************************************************************************!
 * Exported Macros
 *******************************************************************************/
#define DEF_FILE_TOKEN_MAX_LINE_LENGTH      256
#define DEF_FILE_TOKEN_MAX_LENGTH           64

/*******************************************************************************!
 * Function : ReadDefToken
 *******************************************************************************/
char*
ReadDefToken
(char* InReturnToken, char* InString, uint16_t InMaxTokenLength);

char*
ReadDefTokenFromFile
(char* InReturnToken, FILE* InFile, uint16_t InMaxTokenLength);

#endif
