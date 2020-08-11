/*****************************************************************************
 * FILE NAME    : Panel.c
 * DATE         : March 27 2020
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
#include "Panel.h"
#include "MemoryManager.h"
#include "String.h"
#include "Bay.h"
#include "JSONIF.h"
#include "Devices.h"
#include "SQLStatements.h"
#include "main.h"

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
 * Function : PanelDestroy
 *****************************************************************************/
void
PanelDestroy
(Panel* InPanel)
{
  if ( NULL == InPanel ) {
    return;
  }
  FreeMemory(InPanel); 
}

/*****************************************************************************!
 * Function : PanelCreate
 *****************************************************************************/
Panel*
PanelCreate
(uint32_t InIndex, PanelType* InPanelType)
{
  Panel*                                panel;
  DeviceDef*				deviceDef;

  if ( NULL == InPanelType ) {
    return NULL;
  }
  panel = (Panel*)GetMemory(sizeof(Panel));
  memset(panel, 0x0, sizeof(Panel));
  panel->index = InIndex;
  panel->panelType = InPanelType;
  if ( InPanelType->usesSMDUH2 ) {
    deviceDef = FindDeviceDef(mainDeviceDefs, "SMDUH2");
    panel->smduh2 = AddCANDevice("SMDUH2", deviceDef->canAddressStart + (InIndex - 1));
  }
  return panel;
}

/*****************************************************************************!
 * Function : PanelAddFuseBreaker
 * Purpose  : Add a fuse/breaker to a panel
 *****************************************************************************/
void
PanelAddFuseBreaker
(Panel* InPanel, FuseBreaker* InFuseBreaker)
{
}

/*****************************************************************************!
 * Function : PanelFindFuseBreaker
 *****************************************************************************/
FuseBreaker*
PanelFindFuseBreaker
(Panel* InPanel, uint32_t InFuseBreakerIndex)
{
  return NULL;
}

/*****************************************************************************!
 * Function : PanelRemoveFuseBreakerByIndex
 *****************************************************************************/
bool
PanelRemoveFuseBreakerByIndex
(Panel* InPanel, uint32_t InFuseBreakerIndex)
{
  return false;
}

/*****************************************************************************!
 * Function : PanelToJSONString
 *****************************************************************************/
string
PanelToJSONString
(Panel* InPanel)
{
  char                                  s[15];
  string                                returnString;
  if ( NULL == InPanel ) {
    return NULL;
  }

  returnString = StringCopy("       {\n");

  returnString = StringConcatTo(returnString, "         \"panelindex\" : ");
  sprintf(s, "%d,\n", InPanel->index);
  returnString = StringConcatTo(returnString, s);

  returnString = StringConcatTo(returnString, "         \"bayindex\" : ");
  if ( InPanel->bay ) {
    sprintf(s, "%d,\n", InPanel->bay->index);
  } else {
    sprintf(s, "0,\n");
  }
  returnString = StringConcatTo(returnString, s);

  returnString = StringConcatTo(returnString, "         \"listnumber\" : \"");
  if ( InPanel->panelType ) {
    returnString = StringConcatTo(returnString, InPanel->panelType->listNumber);
  } else {
    returnString = StringConcatTo(returnString, "Empty");
  }
  returnString = StringConcatTo(returnString, "\",\n");

  returnString = StringConcatTo(returnString, "         \"canaddress\" : ");
  if ( InPanel->panelType ) {
    sprintf(s, "%d\n", InPanel->panelType->usesSMDUH2 ? InPanel->smduh2->CanAddress : 0);
  } else {
    sprintf(s, "0\n");
  }
  returnString = StringConcatTo(returnString, s);

  returnString = StringConcatTo(returnString, "       }");
  return returnString;
}

/*****************************************************************************!
 * Function : PanelFromJSONString
 *****************************************************************************/
Panel*
PanelFromJSONString
(json_value* InValue)
{
  Panel*                                panel;
  int                                   index;
  PanelType*				panelType;
  string				listnumber;

  
  if ( NULL == InValue ) {
    return NULL;
  }
  index = JSONIFGetInt(InValue, "panelindex");
  listnumber = JSONIFGetString(InValue, "listnumber");
  panelType = PanelTypeFindByListNumber(MainPanelTypes, listnumber);
  FreeMemory(listnumber);
  if ( NULL == panelType ) {
    return NULL;
  }

  panel = PanelCreate(index, panelType);
  return panel;
}

/*****************************************************************************!
 * Function : PanelFuseBreakerSlotOccupied
 *****************************************************************************/
bool
PanelFuseBreakerSlotOccupied
(Panel* InPanel, int InFuseBreakerIndex)
{
  return true;
}

/*****************************************************************************!
 * Function : PanelValuesFromJSONString
 *****************************************************************************/
void
PanelValuesFromJSONString
(Panel* InPanel, json_value* InPanelValue)
{
  int					i;
  int					valuetype;
  string				valuestring;
  CanReg*				canreg;
  json_value*				regs;

  regs = JSONIFGetArray(InPanelValue, "registers");
  if ( NULL == regs ) {
    return;
  }
  for ( i = 0 ; i < regs->u.array.length; i++ ) {
    valuetype = JSONIFGetInt(regs->u.array.values[i], "valuetype");
    valuestring = JSONIFGetString(regs->u.array.values[i], "value");
   
    canreg = PanelFindCANRegister(InPanel, valuetype);
    if ( canreg ) {
      CanRegSetFromString(canreg, valuestring);
    } 
    FreeMemory(valuestring);
  }
}

/*****************************************************************************!
 * Function : PanelValuesToJSONString
 *****************************************************************************/
string
PanelValuesToJSONString
(Panel* InPanel)
{
  return NULL;
}

/*****************************************************************************!
 * Function : PanelValueFromJSONString
 *****************************************************************************/
void
PanelValueFromJSONString
(Panel* InPanel, json_value* InValue)
{
  FuseBreaker*                          fusebreaker;
  int                                   fusebreakerindex;
  json_value*                           value;
  int                                   i;
  json_value*                           values;
  if ( NULL == InPanel || NULL == InValue ) {
    return;
  }
  values = JSONIFGetArray(InValue, "fusebreakers");

  if ( NULL == values ) {
    return;
  }

  for ( i = 0; i < values->u.array.length; i++) {
    value = values->u.array.values[i];
    fusebreakerindex = JSONIFGetInt(value, "fusebreakerindex");
    fusebreaker = PanelFindFuseBreaker(InPanel, fusebreakerindex);
    if ( NULL == fusebreaker ) {
      continue;
    }
    FuseBreakerValueFromJSONString(fusebreaker, value);
  }
}

/*****************************************************************************!
 * Function : PanelFirstFuseBreaker
 *****************************************************************************/
FuseBreaker*
PanelFirstFuseBreaker
(Panel* InPanel)
{
  return NULL;
}

/*****************************************************************************!
 * Function : PanelNextFuseBreaker
 *****************************************************************************/
FuseBreaker*
PanelNextFuseBreaker
(Panel* InPanel, FuseBreaker* InFuseBreaker)
{
  return NULL;
}

/*****************************************************************************!
 * Function : PanelDisplay
 *****************************************************************************/
void
PanelDisplay
(Panel* InPanel, int InIndent, bool InAllInformation)
{
  string				indentString;
  if ( InIndent ) {
   indentString = StringFill(' ', InIndent);
  } else {
   indentString = "";
  }

  printf("%sPANEL\n", indentString);
  printf("%sIndex             : %d\n", indentString, InPanel->index);
  printf("%sType              : %s\n", indentString, InPanel->panelType ? InPanel->panelType->name : "Empty");
  if ( InPanel->panelType == NULL ) {
    return;
  }
  printf("%sList Number       : %s\n", indentString, InPanel->panelType->listNumber);
  printf("%sUses SMDUH2       : %s\n", indentString, InPanel->panelType->usesSMDUH2 ? "true" : "false");
  if ( InPanel->panelType->usesSMDUH2 ) {
    printf("%sCAN Addresss      : %d\n", indentString, InPanel->smduh2->CanAddress);
  }
}

/*****************************************************************************!
 * Function : PanelGetFuseBreakerList
 *****************************************************************************/
string
PanelGetFuseBreakerList
(Panel* InPanel)
{
  return NULL;
}

/*****************************************************************************!
 * Function : PanelSetEmpty
 *****************************************************************************/
void
PanelSetEmpty
(Panel* InPanel)
{
  if ( NULL == InPanel ) {
    return;
  }
  InPanel->panelType = NULL; 
}

/*****************************************************************************!
 * Function : PanelRegValuesToJSONString
 *****************************************************************************/
string
PanelRegValuesToJSONString
(Panel* InPanel)
{
  string				returnString;
  char					s[24];
  int					i;
  CanReg*				reg;

  returnString = StringCopy("  {\n");

  sprintf(s, "%d,\n", InPanel->index);
  returnString = StringConcatTo(returnString, "    \"panelindex\" : ");
  returnString = StringConcatTo(returnString, s);

  sprintf(s, "%d,\n", InPanel->bay->index);
  returnString = StringConcatTo(returnString, "    \"bayindex\" : ");
  returnString = StringConcatTo(returnString, s);

  returnString = StringConcatTo(returnString, "    \"registers\" :\n");
  returnString = StringConcatTo(returnString, "    [\n");

  for ( i = 0 ; i < InPanel->smduh2->registersCount; i++ ) {
    reg = &InPanel->smduh2->Registers[i];
    returnString = StringConcatTo(returnString, "      {\n");
    returnString = StringConcatTo(returnString, "        \"valuetype\" : ");
    sprintf(s, "%d,\n", reg->registerDef->valueType);
    returnString = StringConcatTo(returnString, s);
    
    returnString = StringConcatTo(returnString, "         \"value\" : ");

    CanRegGetFromString(s, reg);
   
    // PanelSaveCANRegister(InPanel, reg->registerDef->valueType, &reg->Value, NULL); 
    returnString = StringConcatTo(returnString, s);
 
    returnString = StringConcatTo(returnString, "      }");
    if ( i + 1 < InPanel->smduh2->registersCount ) {
      returnString = StringConcatTo(returnString, ",");
    }
    returnString = StringConcatTo(returnString, "\n");
  }
  returnString = StringConcatTo(returnString, "    ]\n");
  
  returnString = StringConcatTo(returnString, "  }");

  return returnString;
}

/*****************************************************************************!
 *  Function : PanelFindCANRegister
 *****************************************************************************/
CanReg*
PanelFindCANRegister
(Panel* InPanel, int InValueType)
{
  int					i;
  CanReg*				canreg;

  if ( InPanel == NULL ) {
    return NULL;
  }

  if ( !InPanel->panelType->usesSMDUH2 ) {
    return NULL;
  }

  if ( !InPanel->smduh2 ) {
    return NULL;
  }

  for ( i = 0 ; i < InPanel->smduh2->registersCount ; i++ ) {
    canreg = &(InPanel->smduh2->Registers[i]);
    if ( canreg->registerDef->valueType == InValueType ) {
      return canreg;
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : PanelSaveCANRegister
 *****************************************************************************/
void
PanelSaveCANRegister
(Panel* InPanel, int InValueType, ufloatbit32_t* InValue, sqlite3* InDatabase)
{
  string				statement;
  char					s[18];
  int					sqliteReturn;
  char*					errorMessage = NULL;

  sprintf(s, "%08X", InValue->data32);
  statement = (string)GetMemory(strlen(SQLStatementUpdateRegister) * 2);
  sprintf(statement, SQLStatementUpdateRegister, InPanel->bay->index, InPanel->index,
          InValueType, s); 
  sqliteReturn = sqlite3_exec(MainDataBase, statement, NULL, NULL, &errorMessage);
  if ( sqliteReturn != SQLITE_OK ) {
    
    fprintf(stderr, "Could not save data base entry : %s\n", errorMessage);
    sqlite3_free(errorMessage);
  }
  FreeMemory(statement); 
}

/*****************************************************************************!
 * Function : PanelSaveValuesSQL
 *****************************************************************************/
void
PanelSaveValuesSQL
(Panel* InPanel)
{
  int					i;
  CanReg*				reg;

  if ( NULL == InPanel ) {
    return;
  }
  if ( InPanel->panelType == NULL ) {
    return;
  }
  if ( !InPanel->panelType->usesSMDUH2 ) {
    return;
  } 

  for ( i = 0 ; i < InPanel->smduh2->registersCount; i++ ) {
    reg = &InPanel->smduh2->Registers[i];
    PanelSaveRegisterValueSQL(InPanel, reg);
  }
}

/*****************************************************************************!
 * Function : PanelSaveRegisterValueSQL 
 *****************************************************************************/
void
PanelSaveRegisterValueSQL
(Panel* InPanel, CanReg* InReg)
{
  string				s;
  char					s2[24];
  char*					error;

  s = GetMemory(strlen(SQLStatementUpdateRegister) * 2);
  sprintf(s, SQLStatementUpdateRegister, InPanel->bay->index, InPanel->index, 
          InReg->registerDef->valueType, CanRegGetFromString(s2, InReg));
  if ( SQLITE_OK != sqlite3_exec(MainDataBase, s, NULL, NULL, &error) ) {
    fprintf(stderr, "SQL Error : Statement : %s  : Error %s\n", s, error);
    sqlite3_free(error);
  }
  FreeMemory(s); 
}

