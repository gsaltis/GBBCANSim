/*******************************************************************************
 * FILE         : DeviceRegDef.c
 * DATE         : September 24 2018
 * PROJECT      : CanSim
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/

/*******************************************************************************!
 * Global Headers
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************!
 * Local Headers
 *******************************************************************************/
#include "DeviceRegDef.h"
#include "DefFileToken.h"
#include "NumericTypes.h"
#include "MemoryManager.h"
#include "ThreadSafePrint.h"
#include "String.h"

/*******************************************************************************!
 * Local Macros
 *******************************************************************************/
#define REG_DEF_READ_END_STATE      7

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
static int
DeviceRegDefCompareByName
(const void* InPointer1, const void* InPointer2);

static int
DeviceRegDefCompareByGroupName
(const void* InPointer1, const void* InPointer2);

static int
DeviceRegDefCompareByNumber
(const void* InPointer1, const void* InPointer2);

/*******************************************************************************!
 * Function : ReadDeviceRegsList
 * Note     : We got here because we read an opening "RegDefs" token
 *            We will continue to read tokens until we run out of tokens
 *            (which is an error) or read a RegDefsEnd token
 *******************************************************************************/
DeviceRegDefList*
ReadDeviceRegList
(
 FILE*                                  InFile
)
{
    char                                token[DEF_FILE_TOKEN_MAX_LENGTH+1];
    DeviceRegDefList*                   regDefs;
    
    regDefs = CreateDeviceRegDefList();
    
    while (1) {
        if ( NULL == ReadDefTokenFromFile(token, InFile, DEF_FILE_TOKEN_MAX_LENGTH) )  {
            break;
        }
        if ( *token == 0x00 ) {
            continue;
        }
        if ( strcmp(token, "RegDefsEnd") == 0 ) {
            // We are at the end and done
            break;
        }
        if ( strcmp(token, "RegDef") == 0 ) {
            DeviceRegDef* regDef = ReadDeviceReg(InFile);
            AddDeviceRegDef(regDefs, regDef);
        }
    }
    return regDefs;
}

/*******************************************************************************!
 * Function : DestroyDeviceRegList
 *******************************************************************************/
void
DestroyDeviceRegList
(
 DeviceRegDefList*                      InRegDefs
)
{
    DeviceRegDef*                       regDef;
    if ( NULL == InRegDefs ) {
        return;
    }
    for (regDef = InRegDefs->definitions; regDef; regDef = regDef->next) {
        DestroyDeviceRegDef(regDef);
    }      
    FreeMemory(InRegDefs);
}

/*******************************************************************************!
 * Function : ReadDeviceReg
 *******************************************************************************/
DeviceRegDef*
ReadDeviceReg
(
 FILE*                                  InFile  
)
{
    char                                token[DEF_FILE_TOKEN_MAX_LENGTH+1];
    int                                 state = 0;
    DeviceRegDef*                       regDef;
    bool                                b;
    int                                 n;
    float                               f;
    
    regDef = CreateDeviceRegDef();

    while (1) {
        if ( NULL == ReadDefTokenFromFile(token, InFile, DEF_FILE_TOKEN_MAX_LENGTH) ) {
            FreeMemory(regDef);
            // This is an error
            break;
        }    
        if ( *token == 0x00 ) {
            // This is ok just a blank line or just a comment line
            continue;
        }
        if ( strcmp(token, "RegDefEnd") == 0 ) {
            if ( state == REG_DEF_READ_END_STATE ) {
                return regDef;
            } 
            FreeMemory(regDef);
            return NULL;
        }
        switch (state) {
            case 0 : {
                strcpy(regDef->name, token);
                state++;
                break;
            }
            case 1 : {
                strcpy(regDef->shortName, token);
                state++;
                break;
            }
            case 2 : {
                strcpy(regDef->displayLabel, token);
                state++;
                break;
            }
            case 3 : {
                strcpy(regDef->group, token);
                state++;
                break;
            }
            case 4 : {
                n = GetIntValueFromString(&b, token);
                if (b) {
                    regDef->valueType = n;
                    state++;
                } else {
                    FreeMemory(regDef);
                    return NULL;
                }
                break;
            }
            case 5 : {
                f = GetFloatValueFromString(&b, token);
                if ( b ) {
                    regDef->initialValue.fd = f;
                    state++;
                } else {
                    FreeMemory(regDef);
                    return NULL;
                }
                break;
            }
            case 6 : {
                n = GetIntValueFromString(&b, token);
                if ( b ) {
                    regDef->messageType = n;
                    state++;
                } else {
                    FreeMemory(regDef);
                    return NULL;
                }
                break;
            }
        }
    }
    return regDef;
}

/*****************************************************************************!
 * Function : DumpDeviceRegDefListShort
 *****************************************************************************/
void
DumpDeviceRegDefListShort
(DeviceRegDefList* InDeviceDefRegList, int InIndent, int InSortOrder)
{
  DeviceRegDef*                         regDef;
  int					maxGroupLen, maxShortNameLen;
  int					maxNameLen, maxDisplayLabelLen;
  int					k, n, i;
  DeviceRegDef**			regs;
 
  n = GetDeviceRegCount(InDeviceDefRegList);

  maxGroupLen        = 5;
  maxNameLen         = 4;
  maxShortNameLen    = 10;
  maxDisplayLabelLen = 13;

  for ( regDef = InDeviceDefRegList->definitions; 
        regDef;
        regDef = regDef->next ) {
    if ( maxNameLen < (k = strlen(regDef->name)) ) {
      maxNameLen = k;
    }
    if ( maxShortNameLen < (k = strlen(regDef->shortName)) ) {
      maxShortNameLen = k;
    }
    if ( maxDisplayLabelLen < (k = strlen(regDef->displayLabel)) ) {
      maxDisplayLabelLen = k;
    }
    if ( maxGroupLen < (k = strlen(regDef->group)) ) {
      maxGroupLen = k;
    }
  }
  regs = GetMemory(n * sizeof(DeviceRegDef*));
  for ( i = 0, regDef = InDeviceDefRegList->definitions; 
        regDef;
        regDef = regDef->next, i++ ) {
    regs[i] = regDef;
  }
  if ( InSortOrder == DEVICE_REG_SORT_NAME ) {
    qsort(regs, n, sizeof(DeviceRegDef*), DeviceRegDefCompareByName);
  } else  if ( InSortOrder == DEVICE_REG_SORT_GROUP ) {
    qsort(regs, n, sizeof(DeviceRegDef*), DeviceRegDefCompareByGroupName);
  } else if ( InSortOrder == DEVICE_REG_SORT_NUMBER ) {
    qsort(regs, n, sizeof(DeviceRegDef*), DeviceRegDefCompareByNumber);
  }  
  for (i = 0; i < n; i++ ) {
    DumpDeviceRegDefShort(regs[i], InIndent, maxNameLen, maxShortNameLen, maxGroupLen, maxDisplayLabelLen);
  }
  FreeMemory(regs);
}

/*****************************************************************************!
 * Function : DumpDeviceRegDefList
 *****************************************************************************/
void
DumpDeviceRegDefList
(
 DeviceRegDefList*                      InRegDefs,
 int                                    InIndent
)
{
    DeviceRegDef* regDef;
    if ( NULL == InRegDefs ) {
        return;
    }
    printf_safe("%*s        REGISTER DEFS :\n", InIndent, InIndent > 0 ? " " : "");
    for ( regDef = InRegDefs->definitions; regDef; regDef = regDef->next ) {
        DumpDeviceRegDef(regDef, InIndent+4);
        if ( regDef->next ) {
            printf_safe("\n");
        } 
    }
}

/*******************************************************************************!
 * Function : DumpDeviceRegDef
 *******************************************************************************/
void
DumpDeviceRegDef
(
 DeviceRegDef*                          InRegDef,
 int                                    InIndent
)
{
    if ( NULL == InRegDef ) {
        return;
    }
    printf_safe("%*s         REGISTER DEF :\n", InIndent, InIndent > 0 ? " " : "");
    printf_safe("%*s                 Name : %s\n", InIndent, InIndent > 0 ? " " : "", InRegDef->name);
    printf_safe("%*s           Short Name : %s\n", InIndent, InIndent > 0 ? " " : "", InRegDef->shortName);
    printf_safe("%*s        Display Label : %s\n", InIndent, InIndent > 0 ? " " : "", InRegDef->displayLabel);
    printf_safe("%*s                Group : %s\n", InIndent, InIndent > 0 ? " " : "", InRegDef->group);
    printf_safe("%*s           Value Type : %04x\n", InIndent, InIndent > 0 ? " " : "", InRegDef->valueType);
    printf_safe("%*s         Message Type : %04x\n", InIndent, InIndent > 0 ? " " : "", InRegDef->messageType);
    printf_safe("%*s        Initial Value : %0.04f\n", InIndent, InIndent > 0 ? " " : "", InRegDef->initialValue.fd);
}

/*****************************************************************************!
 * Function : DumpDeviceRegDefShort
 *****************************************************************************/
void
DumpDeviceRegDefShort
(DeviceRegDef* InRegDef, int InIndent, int InNameIndent, int InShortNameIndent, 
 int InGroupIndent, int InDisplayLabelIndent)
{
  string				indentString;

  if ( InIndent > 0 )  {
    indentString = StringFill(' ', InIndent);
  } else {
    indentString = (string)GetMemory(1);
    *indentString = 0x00;
  }
  printf("%s%*s  %*s  %*s  %*s  %04d  %04x  %9.04f\n",
         indentString,
         InNameIndent,
         InRegDef->name,

	 InShortNameIndent,
         InRegDef->shortName,

	 InDisplayLabelIndent,
         InRegDef->displayLabel,

	 InGroupIndent,
         InRegDef->group,

         InRegDef->valueType,
         InRegDef->messageType,
         InRegDef->initialValue.fd);
  FreeMemory(indentString);
}

/*******************************************************************************!
 * Function : DestroyDeviceRegDef
 *******************************************************************************/
void
DestroyDeviceRegDef
(
 DeviceRegDef*                       InRegDef
)
{
    if ( NULL == InRegDef ) {
        return;
    }
    FreeMemory(InRegDef);
}

/*******************************************************************************!
 * Function : CreateDeviceRegDef
 *******************************************************************************/
DeviceRegDef*
CreateDeviceRegDef
()
{
    DeviceRegDef*                       regDef;
    
    regDef = (DeviceRegDef*)GetMemory(sizeof(DeviceRegDef));
    memset(regDef, 0x00, sizeof(DeviceRegDef));
    return regDef;
}

/*******************************************************************************!
 * Function : CreateDeviceRegDefList
 *******************************************************************************/
DeviceRegDefList*
CreateDeviceRegDefList
()
{
    DeviceRegDefList*                   regDefs;
    
    regDefs = (DeviceRegDefList*)GetMemory(sizeof(DeviceRegDefList));
    memset(regDefs, 0x00, sizeof(DeviceRegDefList));
    return regDefs;
}

/*******************************************************************************!
 * Function : FindRegDefByName
 *******************************************************************************/
DeviceRegDef*
FindRegDefByName
(
 DeviceRegDefList*                      InDefinitions, 
 char*                                  InName
)
{
    DeviceRegDef*                       regDef;
    
    if ( NULL == InDefinitions || NULL == InName ) {
        return NULL;
    }
    
    if ( *InName == 0x00 ) {
        return NULL;
    }

    for (regDef = InDefinitions->definitions; regDef; regDef = regDef->next) {
        if ( strcmp(regDef->name, InName) == 0 ) {
            return regDef;
        }
    }
    return NULL;
}

/*******************************************************************************!
 * Function : FindRegDefByValue
 *******************************************************************************/
DeviceRegDef*
FindRegDefByValue
(DeviceRegDefList* InDefinitions, int InValue)
{
  DeviceRegDef*                       regDef;
    
  if ( InDefinitions == NULL ) {
    return NULL;
  }
    
  for (regDef = InDefinitions->definitions; regDef; regDef = regDef->next) {
    if ( regDef->valueType == InValue ) {
      return regDef;
    }
  }
  return NULL;
}

/*******************************************************************************!
 * Function : AddDeviceRegDef
 *******************************************************************************/
void
AddDeviceRegDef
(
 DeviceRegDefList*                      InDeviceRegDefs,
 DeviceRegDef*                          InDeviceRegDef
)
{
    if ( NULL == InDeviceRegDefs || NULL == InDeviceRegDef ) {
        return;
    }
    if ( InDeviceRegDefs->definitions ) {
        InDeviceRegDefs->lastDefinition->next = InDeviceRegDef;
        InDeviceRegDefs->lastDefinition = InDeviceRegDef;
    } else {
        InDeviceRegDefs->definitions = InDeviceRegDef;
        InDeviceRegDefs->lastDefinition = InDeviceRegDef;
    }
}

/*******************************************************************************!
 * Function : GetDeviceRegCount
 *******************************************************************************/
int
GetDeviceRegCount
(
 DeviceRegDefList*                      InDeviceRegDefs
)
{
    int                                 i;
    DeviceRegDef*                       regDef;
    
    if ( NULL == InDeviceRegDefs ) {
        return 0;
    }
    for (i = 0, regDef = InDeviceRegDefs->definitions; 
         regDef;
         regDef = regDef->next, i++) {}
    return i;
}

/*****************************************************************************!
 * Function : DeviceRegDefCompareByName
 *****************************************************************************/
static int
DeviceRegDefCompareByName
(const void* InPointer1, const void* InPointer2)
{
  DeviceRegDef**			reg1;
  DeviceRegDef**			reg2;

  reg1 = (DeviceRegDef**)InPointer1;
  reg2 = (DeviceRegDef**)InPointer2;

  return strcmp((*reg1)->name, (*reg2)->name);
}

/*****************************************************************************!
 * Function : DeviceRegDefCompareByGroupName
 *****************************************************************************/
static int
DeviceRegDefCompareByGroupName
(const void* InPointer1, const void* InPointer2)
{
  DeviceRegDef**			reg1;
  DeviceRegDef**			reg2;
  int					n;

  reg1 = (DeviceRegDef**)InPointer1;
  reg2 = (DeviceRegDef**)InPointer2;

  n = strcmp((*reg1)->group, (*reg2)->group);
  if ( n != 0 ) {
    return n;
  }
  return strcmp((*reg1)->name, (*reg2)->name);
} 

/*****************************************************************************!
 * Function : DeviceRegDefCompareByNumber
 *****************************************************************************/
static int
DeviceRegDefCompareByNumber
(const void* InPointer1, const void* InPointer2)
{
  DeviceRegDef**			reg1;
  DeviceRegDef**			reg2;

  reg1 = (DeviceRegDef**)InPointer1;
  reg2 = (DeviceRegDef**)InPointer2;

  return (*reg1)->valueType - (*reg2)->valueType;
}
 
/*****************************************************************************!
 * Function :  DeviceRegDefFromJSON
 *****************************************************************************/
DeviceRegDef*
DeviceRegDefFromJSON
(json_value* InValue)
{
  DeviceRegDef*				def;
  string				s;
  bool					b;

  def = (DeviceRegDef*)GetMemory(sizeof(DeviceRegDef));
  memset(def, 0x00, sizeof(DeviceRegDef));

  def->valueType = JSONIFGetInt(InValue, "valuetype");
  def->messageType = JSONIFGetInt(InValue, "msgtype");

  s = JSONIFGetString(InValue, "group");
  strcpy(def->group, s);
  strcpy(def->groupsort, s);
  FreeMemory(s);

  s = JSONIFGetString(InValue, "groupsort");
  strcpy(def->groupsort, s);
  FreeMemory(s);
 
  s = JSONIFGetString(InValue, "name");
  strcpy(def->name, s);
  FreeMemory(s);

  s = JSONIFGetString(InValue, "abbrev");
  strcpy(def->shortName, s);
  FreeMemory(s);
 
  s = JSONIFGetString(InValue, "initvalue");
  def->initialValue.data32 = GetIntValueFromString(&b, s);
  FreeMemory(s);
    
  s = JSONIFGetString(InValue, "displaylabel");
  strcpy(def->displayLabel, s);
  FreeMemory(s);
  def->formatType = JSONIFGetInt(InValue, "format");

  return def;
}

/*****************************************************************************!
 * Function : DeviceRegDefListToJSON
 *****************************************************************************/
string
DeviceRegDefListToJSON
(DeviceRegDefList* InDefinitions)
{
  string				defsString;
  string				defString;
  DeviceRegDef*				regDef;

  defsString = StringCopy("  [\n");
  for ( regDef = InDefinitions->definitions; regDef; regDef = regDef->next ) {
    defString = DeviceRegDefToJSON(regDef);
    defsString = StringConcatTo(defsString, defString);
    FreeMemory(defString);
    if ( regDef->next ) {
      defsString = StringConcatTo(defsString, ",\n");
    } else {
      defsString = StringConcatTo(defsString, "\n");
    } 
  }
  
  defsString = StringConcatTo(defsString, "  ]");
  return defsString;
}

/*****************************************************************************!
 * Function : DeviceRegDefToJSON
 *****************************************************************************/
string
DeviceRegDefToJSON
(DeviceRegDef* InDefinition)
{
  string				defString;
  char					s[15];

  defString = StringCopy("  {\n");
  sprintf(s, "%d, ",  InDefinition->valueType);
  defString = StringConcatTo(defString, "\"valuetype\" : ");
  defString = StringConcatTo(defString, s);
  
  sprintf(s, "%d, ", InDefinition->messageType);
  defString = StringConcatTo(defString, "\"msgtype\" : ");
  defString = StringConcatTo(defString, s);

  defString = StringConcatTo(defString, "\"group\" : \"");
  defString = StringConcatTo(defString, InDefinition->group);
  defString = StringConcatTo(defString, "\",\n");

  defString = StringConcatTo(defString, "\"groupsort\" : \"");
  defString = StringConcatTo(defString, InDefinition->groupsort);
  defString = StringConcatTo(defString, "\",\n");
 
  defString = StringConcatTo(defString, "\"name\" : \"");
  defString = StringConcatTo(defString, InDefinition->name);
  defString = StringConcatTo(defString, "\",\n");

  defString = StringConcatTo(defString, "\"abbrev\" : \"");
  defString = StringConcatTo(defString, InDefinition->shortName);
  defString = StringConcatTo(defString, "\",\n");

  defString = StringConcatTo(defString, "\"displaylabel\" : \"");
  defString = StringConcatTo(defString, InDefinition->displayLabel);
  defString = StringConcatTo(defString, "\",\n");

  defString = StringConcatTo(defString, "\"format\" : ");
  sprintf(s, "%d\n", InDefinition->formatType);
  defString = StringConcatTo(defString, s);
  defString = StringConcatTo(defString, "  }");
  return defString;
}

