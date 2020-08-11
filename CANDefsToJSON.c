/*****************************************************************************
 * FILE NAME    : CANDefsToJSON.c
 * DATE         : June 1 2020
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

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "String.h"
#include "FileUtils.h"
#include "MemoryManager.h"
#include "AllCanDefinitions.h"
#include "CanMsg.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
void
CreateJSON
(FILE* InFile, string InDeviceName, CanRegDef* InRegDefList, int InRegDefListSize, CanMsgDef* InMsgDefList, int InMsgDefListSize, int InProtocol, string InName, string InDescription, int InStartingCANAddress);

/*****************************************************************************!
 * Function : main
 *****************************************************************************/
int
main
(int argc, char** argv)
{
  FILE*                                 file;

  CanDefinitionsInitialize();
  file = fopen("DeviceDefs1.json", "wb");
  if ( NULL == file ) {
    fprintf(stderr, "Could not open \"Devices.json\"\n");
    return EXIT_FAILURE;
  }
  fprintf(file, "[\n");
  CreateJSON(file, "SMDUE", RegDefList_SMDUE, NumRegDefList_SMDUE, MsgDefList_SMDUE, NumMsgDefList_SMDUE, 0xA6, "Bay", "Bay", 219);
  fprintf(file, ",\n");
  CreateJSON(file, "SMDUH2", RegDefList_SMDUH2, NumRegDefList_SMDUH2, MsgDefList_SMDUH2, NumMsgDefList_SMDUH2, 0xA5, "Panel", "Panel", 211);
  fprintf(file, "\n");
  CreateJSON(file, "REC", RegDefList_Rect, NumRegDefList_Rect, MsgDefList_Rect, NumMsgDefList_Rect, 0xA5, "Rec", "Rec", 211);
  fprintf(file, "\n");
  fprintf(file, "]\n");
  fclose(file);
  return EXIT_SUCCESS;
}

/*****************************************************************************!
 * Function : CreateJSON
 *****************************************************************************/
void
CreateJSON
(FILE* InFile, string InDeviceType, CanRegDef* InRegDefList, int InRegDefListSize, CanMsgDef* InMsgDefList, int InMsgDefListSize, int InProtocol, string InName, string InDescription, int InStartingCANAddress)
{
  int                                   i, n, k;
  fprintf(InFile, "  {\n");
  fprintf(InFile, "    \"name\"               : \"%s\",\n", InDeviceType);
  fprintf(InFile, "    \"protocol\"           : %d,\n", InProtocol);
  fprintf(InFile, "    \"name\"               : \"%s\",\n", InName);
  fprintf(InFile, "    \"description\"        : \"%s\",\n", InDescription);
  fprintf(InFile, "    \"startingcanaddress\" : %d,\n", InStartingCANAddress);
  fprintf(InFile, "    \"regs\"               : \n");
  fprintf(InFile, "    [\n");
  n = InRegDefListSize;
  for (i = 0; i < n; i++) {
    CanRegDef*				r = &(InRegDefList[i]);

    fprintf(InFile, "      {\n");
    fprintf(InFile, "        \"valuetype\"      : %d,\n", r->ValueType);
    fprintf(InFile, "        \"msgtype\"        : %d,\n", r->MsgType);
    fprintf(InFile, "        \"abbrev\"         : \"%s\",\n", r->Abbrev);
    fprintf(InFile, "        \"groupsort\"      : \"0%s\",\n", r->Group);
    fprintf(InFile, "        \"group\"          : \"%s\",\n", r->Group);
    fprintf(InFile, "        \"name\"           : \"%s\",\n", r->Name);
    fprintf(InFile, "        \"initvalue\"      : \"0x%08X\",\n", r->InitialValue.data32);
    fprintf(InFile, "        \"format\"         : 0,\n");
    fprintf(InFile, "        \"displaylabel\"   : \"%s\"\n", r->DisplayLabel);
    fprintf(InFile, "      }");
    if ( i + 1 <  n ) {
      fprintf(InFile, ",\n");
    } else {
      fprintf(InFile, "\n");
    }
  }
  fprintf(InFile, "    ],\n");
  fprintf(InFile, "    \"messages\" : \n");
  fprintf(InFile, "    [\n");  
  for ( i = 0 ; i < InMsgDefListSize ; i++ ) {
    CanMsgDef*                          msgDef;
    msgDef = &(InMsgDefList[i]);
    fprintf(InFile, "      {\n");
    fprintf(InFile, "        \"request\"        : %d,\n", msgDef->MsgType);
    fprintf(InFile, "        \"name\"           : \"\",\n");
    fprintf(InFile, "        \"responses\"      : [ ");
    for ( k = 0 ; k < MAXRESP ; k++ ) {
      if ( msgDef->Responses[k] == 0x00 ) {
        break;
      }
      fprintf(InFile, "\"0x%02x\"", msgDef->Responses[k]);
      if ( msgDef->Responses[k+1] ) {
        fprintf(InFile, ", ");
      }
    }
    fprintf(InFile, " ]\n");
    fprintf(InFile, "      }");
    if ( i + 1 < InMsgDefListSize ) {
      fprintf(InFile, ",\n");
    } else {
      fprintf(InFile, "\n");
    }
  }
  fprintf(InFile, "    ]\n");
  fprintf(InFile, "  }");
}

