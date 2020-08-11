/*****************************************************************************
 * FILE NAME    : StringUtils.c
 * DATE         : March 17 2020
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
#include <ctype.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "StringUtils.h"

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
 * Function : TrimLine
 *****************************************************************************/
char*
TrimLine
(char* InString)
{
  char*                                 s;

  // Little more than a wrapper around functions to trim start and end of line
  s = TrimStartOfLine(InString);
  TrimEndOfLine(s);

  return s;
}

/*****************************************************************************!
 * Function : TrimStartOfLine
 *****************************************************************************/
char*
TrimStartOfLine
(char* InString)
{
  char*                                 start;
  char*                         s;
  int                           n;

  // Check for an empty string 
  if ( NULL == InString ) {
    return NULL;
  }

  start = InString;

  // skip to first non-space character or end of string
  while (*start && isspace(*start)) {
    start++;
  }

  // If it's all spaces just return a NUL string
  if ( *start == 0x00 ) {
    s = (char*)malloc(1);
    *s = 0x00;
    return s;
  }

  // Copy everything from the first non space character
  n = strlen(start);
  s = (char*)malloc(n + 1);
  strcpy(s, start);
  return s;
}

/*****************************************************************************!
 * Function : TrimEndOfLine
 *****************************************************************************/
void
TrimEndOfLine
(char* InString)
{
  int                                   n;
  if ( NULL == InString ) {
    return;
  }

  n = strlen(InString);
  if ( n == 0 ) {
    return;
  }

  n--;
  while (n >= 0 && (InString[n] == '\r' || InString[n] == '\n') ) {
    n--;
  }
  InString[n+1] = 0x00;
}

/*****************************************************************************!
 * Function : GetBoolFromString
 *****************************************************************************/
bool
GetBoolFromString
(char* InString)
{
  if ( strcmp(InString, "TRUE") == 0 ||
       strcmp(InString, "true") == 0 ||
       strcmp(InString, "1") == 0 ) {
    return true;
  }
  return false;
}

/*****************************************************************************!
 * Function : StringToFloat
 *****************************************************************************/
int
StringToFloat
(char* InString)
{
  char*                                 decPart;
  char*                                 intPart;
  int                                   n, i, d;
  char*                                 period;
  char*                                 start;
  start = InString;
  period = start;

  decPart = NULL;
  intPart = NULL;
  d = 0;
  
  while (*period != 0x00 && *period != '.' ) {
    period++;
  }
  n = period - start;
  intPart = (char*)malloc(n + 1);
  strncpy(intPart, start, n);
  intPart[n] = 0x00;

  if ( *period == '.' ) {
    period++;
    n = strlen(period);
    decPart = (char*)malloc(n + 1);
    strncpy(decPart, period, n);
    decPart[n] = 0x00;
  }

  i = atoi(intPart);
  if ( decPart ) {
    d = atoi(decPart);
    if ( strlen(decPart) == 1 ) {
      d *= 10;
    }
  }
  
  i *= 100;
  i += d;
  free(intPart);
  if ( decPart ) {
    free(decPart);
  }
  return i;
}



