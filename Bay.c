/*****************************************************************************
 * FILE NAME    : Bay.c
 * DATE         : March 27 2020
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "Bay.h"
#include "MemoryManager.h"
#include "String.h"
#include "JSONIF.h"
#include "FileUtils.h"
#include "BayType.h"
#include "main.h"
#include "Devices.h"
#include "CanMsg.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/
#define BAY_DEFAULT_IP_ADDRESS          "127.0.0.1"
#define BAY_DEFAULT_IP_PORT_NUMBER      8001

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
Bay*
Bays[BAY_MAX_INDEX];

string
BaysFileName = NULL;

/*****************************************************************************!
 * Local Functions
 *****************************************************************************/
BayType*
BayTypeFromString
(string InString);

void
BaysRemoveAll
();

void
BayCreateEmptyPanel
(Bay* InBay, int InIndex);

/*****************************************************************************!
 * Function : BaysInitialize
 *****************************************************************************/
void
BaysInitialize
(string InFilename)
{
  string                                base;
  string                                valuesFilename;
  int                                   i;
  json_value*                           value;
  int                                   bufferLen;
  string                                buffer;

  for ( i = 0 ; i < BAY_MAX_INDEX; i++ ) {
    Bays[i] = NULL;
  }

  if ( NULL == InFilename ) {
    return;
  }

  //! Get the file contents
  if ( ! GetFileBuffer(InFilename, &buffer, &bufferLen) ) {
    BaySaveFilename(InFilename); return;
  }

  //! We know we have a good filename, save it
  BaySaveFilename(InFilename);

  //! Parse the file contents
  value = json_parse(buffer, bufferLen);
  FreeMemory(buffer);
  if ( NULL == value ) {
    return;
  }

  
  //! Create the bays
  BaysFromJSONString(value);
  json_value_free(value);

  base = FilenameExtractBase(InFilename);
  if ( NULL == base ) {
    return;
  }

  valuesFilename = StringConcat(base, "-Values.json");
  FreeMemory(base);

  if ( ! FileExists(valuesFilename) ) {
    BaysSaveValues(NULL);
  } else {
    char                                jsonError[256];
    json_settings                       jsettings;
    string				s;
    int					slen;
    json_value*				jsonvalue;

    memset(&jsettings, 0x00, sizeof(json_settings));

    GetFileBuffer(valuesFilename, &s, &slen);
    jsonvalue = json_parse_ex(&jsettings, s, slen, jsonError);
    if ( jsonvalue == NULL ) {
      fprintf(stderr, "JSON Error %s : %s\n", valuesFilename, jsonError);
    } else {
      BaysValueFromJSONString(jsonvalue);
      json_value_free(jsonvalue);
    }
    FreeMemory(s);
  }
}

/*****************************************************************************!
 * Function : BaySaveFilename
 *****************************************************************************/
void
BaySaveFilename
(string InFilename)
{
  if ( BaysFileName ) {
    FreeMemory(BaysFileName);
  }
  BaysFileName = StringCopy(InFilename);
}

/*****************************************************************************!
 * Function : BayCreate
 *****************************************************************************/
Bay*
BayCreate
(int InIndex, BayType* InBayType)
{
  int                                   i, n;
  Bay*                                  bay;
  char					name[15];

  if ( InIndex < 1 && InIndex > BAY_MAX_INDEX ) {
    return NULL;
  }
  i = InIndex - 1;

  if ( Bays[i] != NULL ) {
    return NULL;
  }
  bay = (Bay*)GetMemory(sizeof(Bay));
  memset(bay, 0x00, sizeof(Bay));
  bay->index = InIndex;
  sprintf(name, "Bay %d", InIndex);
  bay->name = StringCopy(name);
  bay->type = InBayType;
  bay->ipaddress = StringCopy(BAY_DEFAULT_IP_ADDRESS);
  bay->portNumber = BAY_DEFAULT_IP_PORT_NUMBER;
  bay->listNumber = StringCopy(InBayType->listNumber);
  bay->panelCount = InBayType->panelCount;
  bay->panelConnections = NULL;
  n = sizeof(Panel*) * bay->type->panelCount;
  bay->panels = (Panel**)GetMemory(n);
  for ( i = 0; i < bay->panelCount; i++ ) {
    bay->panels[i] = (Panel*)GetMemory(sizeof(Panel));
    memset(bay->panels[i], 0x00, sizeof(Panel));
  }
  bay->canDevice = AddSingleDevice("SMDUE");
  memset(bay->panels, 0x0, n);
  BayCreateEmptyPanels(bay);
  Bays[InIndex-1] = bay; 
  return bay;
}

/*****************************************************************************!
 * Function : BayCreateEmptyPanels
 *****************************************************************************/
void
BayCreateEmptyPanels
(Bay* InBay)
{
  int					i;

  for (i = 0; i < InBay->type->panelCount; i++) {
    BayCreateEmptyPanel(InBay, i + 1);
  }
}

/*****************************************************************************!
 * Function : BayCreateEmptyPanel
 *****************************************************************************/
void
BayCreateEmptyPanel
(Bay* InBay, int InIndex)
{
  int					i = InIndex - 1;

  InBay->panels[i] = (Panel*)GetMemory(sizeof(Panel));
  memset(InBay->panels[i], 0x00, sizeof(Panel));
  InBay->panels[i]->index = InIndex;
  InBay->panels[i]->bay = InBay;
  PanelSetEmpty(InBay->panels[i]);
}

/*****************************************************************************!
 * Function : BayDestroy
 *****************************************************************************/
void
BayDestroy
(Bay* InBay)
{
  int                                   j;
  int                                   i;
  if ( NULL == InBay ) {
    return;
  }
  i = InBay->index - 1;
  
  if ( InBay->name ) {
    FreeMemory(InBay->name);
  }
  if ( InBay->ipaddress ) {
    FreeMemory(InBay->ipaddress);
  }

  if ( InBay->canDevice ) {
    RemoveDevice("SMDUE", InBay->canDevice->CanAddress); 
  }

  if ( InBay->panelConnections ) {
    PanelConnectionsDestroy(InBay->panelConnections);
  }
  for ( j = 0; j < InBay->panelCount; j++ ) {
    if ( InBay->panels[j] ) {
      PanelDestroy(InBay->panels[j]);
    }
  }

  if ( InBay->listNumber ) {
    FreeMemory(InBay->listNumber);
  }
  FreeMemory(InBay->panels);
  FreeMemory(InBay);
  Bays[i] = NULL;
}

/*****************************************************************************!
 * Function : BaySet
 * Purpose  : Set the elements in a bay structure
 *****************************************************************************/
void
BaySet
(Bay* InBay, string InName, string InIPAddress, uint32_t InPort)
{
  if ( NULL == InBay ) {
    return ;
  }
  BaySetIPInfo(InBay, InIPAddress, InPort);
  BaySetName(InBay, InName);
}

/*****************************************************************************!
 * Function : BaySetName
 *****************************************************************************/
void
BaySetName
(Bay* InBay, string InName)
{
  if ( NULL == InBay || NULL == InName ) {
    return;
  }
  if ( InBay->name ) {
    FreeMemory(InBay->name);
  }
  InBay->name = StringCopy(InName);
}

/*****************************************************************************!
 * Function : BaySetIPInfo
 *****************************************************************************/
void
BaySetIPInfo
(Bay* InBay, string InIPAddress, uint32_t InPortNumber)
{
  if ( NULL == InBay ) {
    return;
  }

  if ( InBay->ipaddress ) {
    FreeMemory(InBay->ipaddress);
    InBay->ipaddress = NULL;
  }
  if ( InIPAddress ) {
    InBay->ipaddress = StringCopy(InIPAddress);
  }
  InBay->portNumber = InPortNumber;
}

/*****************************************************************************!
 * Function : BayAddPanel
 *****************************************************************************/
void
BayAddPanel
(Bay* InBay, Panel* InPanel)
{
  int                                   i;

  if ( NULL == InBay || NULL == InPanel ) {
    return;
  }
  i = InPanel->index - 1;
  if ( i < 0 || i >= InBay->panelCount ) {
    return;
  }
  if ( InBay->panels[i] != NULL && InBay->panels[i]->panelType != NULL ) {
    return;
  }
  InBay->panels[i] = InPanel;
  InPanel->bay = InBay;
}

/*****************************************************************************!
 * Function : BayFindPanelByIndex
 * Purpose  : Find a panel by its index number
 *****************************************************************************/
Panel*
BayFindPanelByIndex
(Bay* InBay, int InPanelIndex)
{
  int                                   i;

  if ( NULL == InBay ) {
    return NULL;
  }

  if ( InPanelIndex > InBay->panelCount ) {
    return NULL;
  }
  for ( i = 0; i < InBay->panelCount; i++ ) {
    if ( InBay->panels[i]->index == InPanelIndex ) {
      if ( InBay->panels[i]->panelType != NULL ) {
	return InBay->panels[i];
      }
    }
  } 
  return NULL;
}

/*****************************************************************************!
 * Function : BayGetIPInfo
 * Purpose  : Return the Bay's IP Information
 *****************************************************************************/
void
BayGetIPInfo
(Bay* InBay, string* InIPAddress, uint32_t* InPortNumber)
{
  if ( NULL == InBay || NULL == InIPAddress || NULL == InPortNumber ) {
    return;
  }
  *InIPAddress = InBay->ipaddress;
  *InPortNumber = InBay->portNumber;
}

/*****************************************************************************!
 * Function : FindBayByIndex
 * Purpose  : Find a bay by its index number
 *****************************************************************************/
Bay*
FindBayByIndex
(int InIndex)
{
  int                                   i;

  i = InIndex - 1;

  if ( i < 0 || i >= BAY_MAX_INDEX ) {
    return NULL;
  }

  return Bays[i];
}


/*****************************************************************************!
 * Function : BayTypeToString
 * Purpose  : Return the string form of a BayType
 *****************************************************************************/
string
BayTypeToString
(BayType* InBayType)
{
  return InBayType->name;
}
/*****************************************************************************!
 * Function : BayToJSONString
 * Purpose  : Return JSON Encoded form of a Bay
 *****************************************************************************/
string
BayToJSONString
(Bay* InBay)
{
  int                                   i;
  bool                                  havePanel;
  string                                rString;
  char                                  s1[64];
  string                                s;
  PanelConnection*			connection;

  rString = StringCopy("  {\n");
  rString = StringConcatTo(rString, "    \"name\" : \"");

  if ( InBay->name == NULL ) {
    sprintf(s1, "Bay %d", InBay->index);
    s = s1;
  } else {
    s = InBay->name;
  }
  rString = StringConcatTo(rString, s);
  rString = StringConcatTo(rString, "\",\n");
  rString = StringConcatTo(rString, "    \"bayindex\" : ");
  sprintf(s1, "%d,\n", InBay->index);
  rString = StringConcatTo(rString, s1);
  
  sprintf(s1, "%d,\n", InBay->canDevice->CanAddress);
  rString = StringConcatTo(rString, "    \"canaddress\" : ");
  rString = StringConcatTo(rString, s1);

  rString = StringConcatTo(rString, "    \"type\" : \"");
  rString = StringConcatTo(rString, BayTypeToString(InBay->type));
  rString = StringConcatTo(rString, "\",\n");

  rString = StringConcatTo(rString, "    \"panelcount\" : ");
  sprintf(s1, "%d,\n", InBay->type->panelCount);
  rString = StringConcatTo(rString, s1);

  rString = StringConcatTo(rString, "    \"ipaddress\" : \"");
  rString = StringConcatTo(rString, InBay->ipaddress);
  rString = StringConcatTo(rString, "\",\n");

  rString = StringConcatTo(rString, "    \"portnumber\" : ");
  sprintf(s1, "%d,\n", InBay->portNumber);
  rString = StringConcatTo(rString, s1);

  rString = StringConcatTo(rString, "    \"listnumber\" : \"");
  rString = StringConcatTo(rString, InBay->listNumber);
  rString = StringConcatTo(rString, "\",\n");

  rString = StringConcatTo(rString, "    \"panels\" : [");

  havePanel = false;

  for ( i = 0; i < InBay->panelCount; i++ ) {
    if ( NULL == InBay->panels[i] ) {
      continue;
    }
    if ( havePanel ) {
      rString = StringConcatTo(rString, ",\n");
    } else {
      rString = StringConcatTo(rString, "\n");
    }
    s = PanelToJSONString(InBay->panels[i]);
    rString = StringConcatTo(rString, s);
    FreeMemory(s);
    havePanel = true;
  }
  rString = StringConcatTo(rString, "\n");
  rString = StringConcatTo(rString, "    ],\n");
  rString = StringConcatTo(rString, "    \"rectifiers\" : \n");
  rString = StringConcatTo(rString, "    [\n");
  for ( Rectifier* rectifier = InBay->rectifiers; rectifier; rectifier = rectifier->next ) {
    string				s;
    s = RectifierToJSONString(rectifier);
    rString = StringConcatTo(rString, s);
    FreeMemory(s);
    if ( rectifier->next ) {
      rString = StringConcatTo(rString, ",\n");
    } else {
      rString = StringConcatTo(rString, "\n");
    }
  }
  rString = StringConcatTo(rString, "    ],\n");
  rString = StringConcatTo(rString, "    \"panelconnections\" :\n");
  rString = StringConcatTo(rString, "    [\n");
  connection = InBay->panelConnections;
  while (connection) {
    rString = StringConcatTo(rString, PanelConnectionToJSONString(connection, 6));
    connection = connection->next;
    if (connection) {
      rString = StringConcatTo(rString, ",");
    }
    rString = StringConcatTo(rString, "\n");
  }
  rString = StringConcatTo(rString, "    ]\n");
 
  rString = StringConcatTo(rString, "  }");
  return rString;
}

/*****************************************************************************!
 * Function : BaysToJSONString
 *****************************************************************************/
string
BaysToJSONString
()
{
  string                                s2;
  bool                                  haveBay;
  Bay*                                  bay;
  int                                   i;
  string                                s;

  s = StringCopy("[");
  haveBay = false;
  for ( i = 0; i < BAY_MAX_INDEX; i++ ) {
    bay = Bays[i];
    if ( NULL == bay ) {
      continue;
    }
    if ( haveBay ) {
      s = StringConcatTo(s, ",\n");
    } else {
      s = StringConcatTo(s, "\n");
    }
    haveBay = true;
    s2 = BayToJSONString(bay);
    s = StringConcatTo(s, s2);
    FreeMemory(s2);
  }
  s = StringConcatTo(s, "\n]");
  return s;
}

/*****************************************************************************!
 * Function : BaysFromJSONString
 *****************************************************************************/
bool
BaysFromJSONString
(json_value* InValue)
{
  json_value*                           value;
  int                                   i;
  if ( NULL == InValue ) {
    return NULL;
  }
  BaysRemoveAll();
  if ( InValue->type != json_array ) {
    return false;
  }

  for (i = 0; i < InValue->u.array.length; i++) {
    value = InValue->u.array.values[i];
    (void)BayFromJSONString(value);
  }

  return true;
}

/*****************************************************************************!
 * Function : BayFromJSONString
 *****************************************************************************/
Bay*
BayFromJSONString
(json_value* InValue)
{
  Panel*                                panel;
  json_value*                           panelsValue;
  int                                   m;
  json_value*                           value;
  int                                   k;
  int                                   i;
  int                                   n;
  Bay*                                  bay;
  string                                typeString;
  int                                   portNumber;
  string                                ipaddress;
  int                                   panelCount;
  BayType*                              type;
  int                                   index;
  string                                name;
  string				listNumber;
  Rectifier*				rectifier;
  json_value*				rectifiersValue;
  int					canAddress;

  if ( NULL == InValue ) {
    return NULL;
  }

  if ( InValue->type != json_object ) {
    return NULL;
  }

  index = JSONIFGetInt(InValue, "bayindex");
  if ( index < 1 || index > BAY_MAX_INDEX ) {
    return NULL;
  }

  i = index - 1;
  if ( Bays[i] ) {
    BayDestroy(Bays[i]);
    Bays[i] = NULL;
  }
  name = JSONIFGetString(InValue, "name");
  
  typeString = JSONIFGetString(InValue, "type");
  type = BayTypeFromString(typeString);
  panelCount = JSONIFGetInt(InValue, "panelcount");
  ipaddress = JSONIFGetString(InValue, "ipaddress");
  portNumber = JSONIFGetInt(InValue, "portnumber");
  listNumber = JSONIFGetString(InValue, "listnumber");
  canAddress = JSONIFGetInt(InValue, "canaddress");

  bay = (Bay*)GetMemory(sizeof(Bay));
  memset(bay, 0x00, sizeof(Bay));
  if ( canAddress == 0 ) {
    bay->canDevice = AddSingleDevice("SMDUE");
  } else {
    bay->canDevice = AddCANDevice("SMDUE", canAddress);
  }

  bay->name             = name;
  bay->index            = index;
  bay->ipaddress        = ipaddress;
  bay->portNumber       = portNumber;
  bay->type             = type;
  bay->panelCount       = panelCount;
  bay->listNumber       = listNumber;

  
  n = sizeof(Panel*) * panelCount;
  bay->panels = (Panel**)GetMemory(n);
  memset(bay->panels, 0x00, n);
  for (i = 0 ; i < panelCount; i++) {
     bay->panels[i] = (Panel*)GetMemory(sizeof(Panel));
     memset(bay->panels[i], 0x00, sizeof(Panel));
  }
  BayCreateEmptyPanels(bay); 
  panelsValue = JSONIFGetArray(InValue, "panels");

  //!
  if ( panelsValue ) {
    for ( k = 0; k < panelsValue->u.array.length; k++ ) {
      value = panelsValue->u.array.values[k];
      panel = PanelFromJSONString(value);
      if ( panel ) {
        if ( panel->index < 1 || panel->index > panelCount ) {
          PanelDestroy(panel);
          continue;
        }
        m = panel->index - 1;
        if ( bay->panels[m] ) {
          PanelDestroy(bay->panels[m]);
        }
        bay->panels[m] = panel;
        panel->bay = bay;
      }
    }
  }
  // Find the rectifiers branch   
  rectifiersValue = JSONIFGetArray(InValue, "rectifiers");
  if ( rectifiersValue ) {
    // Walk it, read and store the rectifiers found
    for (k = 0; k < rectifiersValue->u.array.length; k++) {
      value = rectifiersValue->u.array.values[k];
      index = JSONIFGetInt(value, "index");

      // Drop rectifiers with duplicate index values
      rectifier = BayFindRectifierByIndex(bay, index);
      if ( rectifier ) {
        continue;
      }

      // Create, read and add the new rectifier
      rectifier = CreateRectifierNoDevice(index);
      RectifierFromJSONString(rectifier, value);
      BayAddRectifier(bay, rectifier);
    }
  }
  json_value* panelConnectionsValues = JSONIFGetArray(InValue, "panelconnections");
  if ( panelConnectionsValues ) {
    for (k = 0; k < panelConnectionsValues->u.array.length; k++) {
      json_value* value = panelConnectionsValues->u.array.values[k];
      PanelConnection* panelConnection = PanelConnectionFromJSONString(value);
      if ( panelConnection ) {
        BayAppendPanelConnection(bay, panelConnection);
      }
    }
  }
  Bays[bay->index-1] = bay;
  return bay;
}

/*****************************************************************************!
 * Function : BaysRemoveAll
 *****************************************************************************/
void
BaysRemoveAll
()
{
  int                                   i;
  for (i = 0 ; i < BAY_MAX_INDEX; i++) {
    if ( Bays[i] ) {
      BayDestroy(Bays[i]);
      Bays[i] = NULL;
    }
  }
}

/*****************************************************************************!
 * Function : BayTypeFromString
 *****************************************************************************/
BayType*
BayTypeFromString
(string InString)
{
  return BayTypeFindByName(MainBayTypes, InString);
}

/*****************************************************************************!
 * Function : BaysSaveValues
 *****************************************************************************/
bool
BaysSaveValues
(string InFilename)
{
  string                                filename;
  string                                basename;
  string                                s, s2;
  FILE*                                 file;
  bool					haveBay;
  Bay*					bay;
  int					i;

  if ( InFilename ) {
    filename = InFilename;
  } else {
    filename = BaysFileName;
  }
  if ( NULL == filename ) {
    return false;
  }
  basename = FilenameExtractBase(filename);
  basename = StringConcatTo(basename, "-Values.json");
  file = fopen(basename, "wb");
  FreeMemory(basename);
  if ( NULL == file ) {
    return false;
  }
  
  s = StringCopy("[");
  haveBay = false;
  for ( i = 0; i < BAY_MAX_INDEX; i++ ) {
    bay = Bays[i];
    if ( NULL == bay ) {
      continue;
    }
    if ( haveBay ) {
      s = StringConcatTo(s, ",\n");
    } else {
      s = StringConcatTo(s, "\n");
    }
    haveBay = true;
    s2 = BayRegValuesToJSONString(bay, true); 
    s = StringConcatTo(s, s2);
    FreeMemory(s2);
  }
  s = StringConcatTo(s, "\n]\n");

  fprintf(file, "%s", s);
  fclose(file);
  FreeMemory(s);
  return true;
}

/*****************************************************************************!
 * Function : BaysSave
 *****************************************************************************/
bool
BaysSave
(string InFilename)
{
  string                                s;
  FILE*                                 file;
  string                        filename;

  if ( InFilename ) {
    filename = InFilename;
  } else {
    filename = BaysFileName;
  }

  if ( NULL == filename ) {
    return false;
  }

  if ( BaysFileName ) {
    if ( ! StringEqual(filename, BaysFileName) ) {
      FreeMemory(BaysFileName);
    }
  }
  BaysFileName = StringCopy(filename);

  file = fopen(BaysFileName, "wb");
  if ( NULL == file ) {
    return false;
  }

  s = BaysToJSONString();
  fprintf(file, s);

  fclose(file);
  FreeMemory(s);
  return true;
}

/*****************************************************************************!
 * Function : BayPanelSlotOccupied
 *****************************************************************************/
bool
BayPanelSlotOccupied
(Bay* InBay, int InPanelIndex)
{
  Panel*                                panel;
  if ( NULL == InBay ) {
    return false;
  }
  panel = BayFindPanelByIndex(InBay, InPanelIndex);
  return panel != NULL;
}

/*****************************************************************************!
 * Function : BayUpdateFromJSONString
 *****************************************************************************/
bool
BayUpdateFromJSONString
(Bay* InBay, json_value* InValue)
{
  bool                                  changed;
  int                                   portnumber;
  string                                ipaddress;

  if ( NULL == InBay || NULL == InValue ) {
    return false;
  }
  
  ipaddress = JSONIFGetString(InValue, "ipaddress");
  portnumber = JSONIFGetInt(InValue, "portnumber");
  
  if ( ipaddress ) {
    if ( InBay->ipaddress ) {
      FreeMemory(InBay->ipaddress);
    }
    InBay->ipaddress = StringCopy(ipaddress);
    changed = true;
  }

  if ( portnumber != 0 ) {
    if ( portnumber != InBay->portNumber ) {
      InBay->portNumber = portnumber;
      changed = true;
    }
  }
  return changed;
}

/*****************************************************************************!
 * Function : BaysRemoveBayByIndex
 *****************************************************************************/
bool
BaysRemoveBayByIndex
(int InIndex)
{
  int                                   i;
  if ( InIndex < 1 || InIndex > BAY_MAX_INDEX ) {
    return false;
  }
  i = InIndex - 1;

  BayDestroy(Bays[i]);
  Bays[i] = NULL;
  return true;
}

/*****************************************************************************!
 * Function : BayRemovePanelByIndex
 *****************************************************************************/
bool
BayRemovePanelByIndex
(Bay* InBay, int InPanelIndex)
{
  int                                   i;
  if ( NULL == InBay ) {
    return false;
  }

  if ( InPanelIndex < 1 || InPanelIndex > InBay->panelCount ) {
    return false;
  }

  i = InPanelIndex - 1;
  if ( NULL == InBay->panels[i] ) {
    return false;
  }

  PanelDestroy(InBay->panels[i]);
  InBay->panels[i] = NULL;
  BayCreateEmptyPanel(InBay, InPanelIndex);
  return true;
}

/*****************************************************************************!
 * Function : BayRegValuesFromJSONString
 *****************************************************************************/
void
BayRegValuesFromJSONString
(Bay* InBay, json_value* InValue)
{
  int					i;
  int					valueType;
  string				valueString;
  json_value*				value;
  CanReg*				canReg;
  bool					b;

  if ( NULL == InBay || NULL == InValue ) {
    return;
  }

  if ( InValue->type != json_array ) {
    return;
  }
  
  for ( i = 0 ; i < InValue->u.array.length; i++ ) {
    value = InValue->u.array.values[i];
    valueType = JSONIFGetInt(value, "valuetype");
    valueString = JSONIFGetString(value, "value");
    canReg = BayFindCANRegister(InBay, valueType);    
    if ( canReg->registerDef->formatType == 0 ) {
      canReg->Value.fd = GetFloatValueFromString(&b, valueString);
    } else if ( canReg->registerDef->formatType == 1 ) {
      memcpy(&(canReg->Value.data32), valueString, 3);
    } else if ( canReg->registerDef->formatType == 2 ) {
      canReg->Value.data32 = GetIntValueFromString(&b, valueString);
    } else if ( canReg->registerDef->formatType == 3 ) {
      memcpy(&(canReg->Value.data32), valueString, 4);
    } else if ( canReg->registerDef->formatType == 4 ) {
      canReg->Value.data32 = GetHex32ValueFromString(NULL, valueString);
    }
    FreeMemory(valueString);        
  } 
}

/*****************************************************************************!
 * Function : BayFindCANRegister
 *****************************************************************************/
CanReg*
BayFindCANRegister
(Bay* InBay, int InValueType)
{
  CanReg*				canReg;
  int					i;

  if ( InBay == NULL ) {
    return NULL;
  }

  for ( i = 0 ; i < InBay->canDevice->registersCount ; i++ ) {
    canReg = &(InBay->canDevice->Registers[i]);
    if ( canReg->registerDef->valueType == InValueType ) {
      return canReg;
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : BayRegValuesToJSONString
 *****************************************************************************/
string
BayRegValuesToJSONString
(Bay* InBay, bool InSavePanelRegs)
{
  string				returnString;
  char					s[24];
  int					i;
  CanReg*				reg;

  returnString = StringCopy("  {\n");
  sprintf(s, "%d,\n", InBay->index);
  returnString = StringConcatTo(returnString, "    \"bayindex\" : ");
  returnString = StringConcatTo(returnString, s);


  returnString = StringConcatTo(returnString, "    \"registers\" :\n");
  returnString = StringConcatTo(returnString, "    [\n");

  for ( i = 0 ; i < InBay->canDevice->registersCount; i++ ) {
    reg = &InBay->canDevice->Registers[i];
    returnString = StringConcatTo(returnString, "      {\n");
    returnString = StringConcatTo(returnString, "        \"valuetype\" : ");
    sprintf(s, "%d,\n", reg->registerDef->valueType);
    returnString = StringConcatTo(returnString, s);
    
    returnString = StringConcatTo(returnString, "        \"value\" : \"");

    // Normal floating point
    if ( reg->registerDef->formatType == 0 ) {
      sprintf(s, "%.03f\"\n", reg->Value.fd);

    // Hardware Version
    } else if ( reg->registerDef->formatType == 1 ) {
      sprintf(s, "%c%c%c\"\n", reg->Value.data8[3], reg->Value.data8[2], reg->Value.data8[1]);

    // Serial Number
    } else if ( reg->registerDef->formatType == 2 ) {
      sprintf(s, "%u\"\n", reg->Value.data32);      

    // 4 ASCII characters
    } else if ( reg->registerDef->formatType == 3 ) {
      sprintf(s, "%c%c%c%c\"\n", reg->Value.data8[3], reg->Value.data8[2], reg->Value.data8[1], reg->Value.data8[0]);

    // Hex characters
    } else if ( reg->registerDef->formatType == 4 ) {
      sprintf(s, "%08X\"\n", reg->Value.data32);
    }
     
    returnString = StringConcatTo(returnString, s);
 
    returnString = StringConcatTo(returnString, "      }");
    if ( i + 1 < InBay->canDevice->registersCount ) {
      returnString = StringConcatTo(returnString, ",");
    }
    returnString = StringConcatTo(returnString, "\n");
  }
  returnString = StringConcatTo(returnString, "    ]");

   
  if ( InSavePanelRegs ) {
    Panel*				panel;
    string				s;
    bool				havePanel;

    havePanel = false;
    returnString = StringConcatTo(returnString, ",\n");
    returnString = StringConcatTo(returnString, "    \"panels\" : \n");
    returnString = StringConcatTo(returnString, "    [");
    panel = BayFirstPanel(InBay);
    while ( panel ) {
      if ( panel->panelType == NULL || !panel->panelType->usesSMDUH2 ) {
        panel = BayNextPanel(InBay, panel);
	continue;
      }
      if ( havePanel ) {
        returnString = StringConcatTo(returnString, ",\n");
      } else {
	returnString = StringConcatTo(returnString, "\n");
      }
      s = PanelRegValuesToJSONString(panel);
      returnString = StringConcatTo(returnString, s);
      FreeMemory(s);
      panel = BayNextPanel(InBay, panel);
      havePanel = true;
    }
    if ( !havePanel ) {
      returnString = StringConcatTo(returnString, "\n");
    }
    returnString = StringConcatTo(returnString, "    ]\n");
  } else {
    returnString = StringConcatTo(returnString, "\n");
  }
  returnString = StringConcatTo(returnString, "  }");

  return returnString;
}

/*****************************************************************************!
 * Function : BayValueToJSONString
 *****************************************************************************/
string
BayValueToJSONString
(Bay* InBay)
{
  char                                  s[16];
  Panel*                                panel;
  int                                   i;
  bool                                  haveone;
  string                                returnString, s2;

  if ( NULL == InBay ) {
    return NULL;
  }
  
  returnString = StringCopy("  {\n    \"bayindex\" : ");
  sprintf(s, "%d,\n", InBay->index);
  returnString = StringConcatTo(returnString, s);

  returnString = StringConcatTo(returnString, "    \"panels\" :\n");
  returnString = StringConcatTo(returnString, "      [\n");
  haveone = false;

  for ( i = 0; i < InBay->panelCount ; i++ ) {
    panel = InBay->panels[i];
    if ( NULL == panel ) {
      continue;
    }
    if ( haveone ) {
      returnString = StringConcatTo(returnString, ",\n");
    }
    haveone = true;
    s2 = PanelValuesToJSONString(panel);
    returnString = StringConcatTo(returnString, s2);
    FreeMemory(s);
  }
  returnString = StringConcatTo(returnString, "\n      ]\n");
  returnString = StringConcatTo(returnString, "  }");
  return returnString;
}

/*****************************************************************************!
 * Function : BaysValueToJSONString
 *****************************************************************************/
string
BaysValueToJSONString
()
{
  int                                   i;
  bool                                  haveone;
  string                                returnString;
  
  returnString = StringCopy("[\n");
  haveone = false;

  for ( i = 0; i < BAY_MAX_INDEX; i++ ) {
    if ( NULL == Bays[i] ) {
      continue;
    }
    if ( haveone ) {
      returnString = StringConcatTo(returnString, ",\n");
    }
    haveone = true;
    returnString = StringConcatTo(returnString, BayValueToJSONString(Bays[i]));
  }
  returnString = StringConcatTo(returnString, "\n]\n");
  return returnString;
}

/*****************************************************************************!
 * Function : BayValueFromJSONString
 *****************************************************************************/
void
BayValueFromJSONString
(Bay* InBay, json_value* InValue)
{
  json_value*                           regs;
  json_value*				panelValue;
  json_value*				panelValues;
  Panel*				panel;
  int					panelindex;
  int					i;

  if ( NULL == InBay || NULL == InValue ) {
    return;
  }
  if ( InValue->type != json_object ) {
    return;
  }

  regs = JSONIFGetArray(InValue, "registers");
  if ( NULL == regs ) {
    return;
  }  

  
  BayRegValuesFromJSONString(InBay, regs);
  panelValues = JSONIFGetArray(InValue, "panels");

  if ( NULL == panelValues ) {
    return;
  }

  for (i = 0; i < panelValues->u.array.length; i++ ) {
    panelValue = panelValues->u.array.values[i];
    panelindex = JSONIFGetInt(panelValue, "panelindex");
    panel = BayFindPanelByIndex(InBay, panelindex);
    if ( NULL == panel ) {
      continue;
    }
    PanelValuesFromJSONString(panel, panelValue);     
  }
}

/*****************************************************************************!
 * Function : BaysValueFromJSONString
 *****************************************************************************/
void
BaysValueFromJSONString
(json_value* InValue)
{
  int                                   k;
  int                                   bayIndex;
  int                                   i;
  json_value*                           value;
  if ( NULL == InValue ) {
    return;
  }

  if ( InValue->type != json_array ) {
    return;
  }

  for ( i = 0; i < InValue->u.array.length ; i++ ) {
    value = InValue->u.array.values[i];
    bayIndex = JSONIFGetInt(value, "bayindex");
    k = bayIndex - 1;
    if ( NULL == Bays[k] ) {
      continue;
    }
    BayValueFromJSONString(Bays[i], value);
  }
}

/*****************************************************************************!
 * Function : BaysFirstBay
 *****************************************************************************/
Bay*
BaysFirstBay
()
{
  int                                   i;

  for ( i = 0 ; i < BAY_MAX_INDEX ; i++ ) {
    if ( Bays[i] ) {
      return Bays[i];
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : BaysNextBay
 *****************************************************************************/
Bay*
BaysNextBay
(Bay* InBay)
{
  int                                   i;

  if ( InBay == NULL ) {
    return NULL;
  }
  for ( i = 0 ; i < BAY_MAX_INDEX  && Bays[i] != InBay; i++ ) {
  }

  for ( i++; i < BAY_MAX_INDEX && Bays[i] == NULL; i++ ) {
  }
  return (i < BAY_MAX_INDEX) ? Bays[i] : NULL;
}

/*****************************************************************************!
 * Function : BayFirstPanel
 *****************************************************************************/
Panel*
BayFirstPanel
(Bay* InBay)
{
  int                                   i;

  if ( NULL == InBay ) {
    return NULL;
  }
  for ( i = 0; i < InBay->panelCount; i++ ) {
    if ( InBay->panels[i] == NULL ) {
      continue;
    }
    return InBay->panels[i];
  }
  return NULL;
}

/*****************************************************************************!
 * Function : BayNextPanel
 *****************************************************************************/
Panel*
BayNextPanel
(Bay* InBay, Panel* InPanel)
{
  int                                   i;

  for ( i = 0 ; i < InBay->panelCount; i++ ) {
    if ( InBay->panels[i] == InPanel ) {
      break;
    }
  }
  if ( i == InBay->panelCount ) {
    return NULL;
  }
  for ( i++; i < InBay->panelCount && InBay->panels[i] == NULL; i++ ) {
  }
  return i < InBay->panelCount ? InBay->panels[i] : NULL;
}

/*****************************************************************************!
 * Function : BayDisplay
 *****************************************************************************/
void
BayDisplay
(Bay* InBay, int InIndent, bool InAllInformation)
{
  string				indentString;
  Panel*				panel;
  PanelConnection*			connection;
  int					i;

  if ( InIndent > 0 ) {
    indentString = StringFill(' ', InIndent);
  } else {
    indentString = "";
  }

  printf("%sBAY\n", indentString);
  printf("%sIndex               : %d\n", indentString, InBay->index);
  printf("%sName                : %s\n", indentString, InBay->name);
  printf("%sType                : %s\n", indentString, BayTypeToString(InBay->type));
  printf("%sPanel Count         : %d\n", indentString, InBay->panelCount);
  printf("%sIP Address          : %s\n", indentString, InBay->ipaddress);
  printf("%sPort Number         : %d\n", indentString, InBay->portNumber); 
  printf("%sList Number         : %s\n", indentString, InBay->listNumber);
  printf("%sPanel Connections   : \n", indentString);
  for ( i = 1, connection = InBay->panelConnections; connection; connection = connection->next, i++ ) {
    printf("%s  Connection        : %d\n", indentString, i);
    printf("%s    From            : %d\n", indentString, connection->fromPanel);
    printf("%s    To              : %d\n", indentString, connection->toPanel);
    printf("%s--------------------\n", indentString);
  }
  if ( !InAllInformation ) {
    if ( InIndent > 0 ) {
      FreeMemory(indentString);
    }
    return;
  }

  for ( panel = BayFirstPanel(InBay); panel; panel = BayNextPanel(InBay, panel) ) {
    PanelDisplay(panel, InIndent + 2, InAllInformation);
  }

  if ( MainSupportRectifiers ) { 
    printf("%sRectifiers\n", indentString);
    for ( Rectifier* rectifier = InBay->rectifiers; rectifier; rectifier = rectifier->next ) {
      RectifierDisplay(rectifier, InIndent + 4, false);
    }
  }
  if ( InIndent > 0 ) {
    FreeMemory(indentString);
  }
 
}

/*****************************************************************************!
 * Function : BaysGetIndexList
 *****************************************************************************/
string
BaysGetIndexList
()
{
  string				returnString;
  Bay*					bay;
  char					s[7];
  
  returnString = NULL;

  for (bay = BaysFirstBay(); bay; bay = BaysNextBay(bay)) {
    sprintf(s, "%d", bay->index);
    if ( returnString ) {
      returnString = StringConcatTo(returnString, ", ");
      returnString = StringConcatTo(returnString, s);
    } else {
      returnString = StringCopy(s);
    }
  }
  return returnString;
}

/*****************************************************************************!
 * Function : BaysGetPanelIndexList
 *****************************************************************************/
string
BayGetPanelIndexList
(Bay* InBay, bool InIncludePlainPanels)
{
  string				returnString;
  char					s[7];
  Panel*				panel;

  returnString = NULL;

  for ( panel = BayFirstPanel(InBay); panel; panel = BayNextPanel(InBay, panel) ) {
    if ( InIncludePlainPanels || panel->panelType->usesSMDUH2 ) {
      sprintf(s, "%d", panel->index);
      if ( returnString ) {
        returnString = StringConcatTo(returnString, ", ");
        returnString = StringConcatTo(returnString, s);
      } else {
        returnString = StringCopy(s);
      }
    }
  }
  return returnString;
}

/*****************************************************************************!
 * Function : BayFindPanelConnectionByIndices
 *****************************************************************************/
PanelConnection*
BayFindPanelConnectionByIndices
(Bay* InBay, int InFromIndex, int InToIndex)
{
  PanelConnection*				connection;

  if ( InBay == NULL || 0 == InFromIndex || 0 == InToIndex ) {
    return NULL;
  }
  
  for ( connection = InBay->panelConnections; connection; connection = connection->next ) {
    if ( connection->fromPanel == InFromIndex && connection->toPanel == InToIndex ) {
      return connection;
    }
  }
  return NULL;
}

/*****************************************************************************!
 * Function : BayVerifyNewPanelConnection
 *****************************************************************************/
bool
BayVerifyNewPanelConnection
(Bay* InBay, int InFromIndex, int InToIndex)
{
  if ( InBay == NULL ) {
    return false;
  }
  if ( StringEqual(InBay->type->name, "6 Panel") ) {
    switch (InFromIndex) {
      case 1 :
        if ( InToIndex == 2 || InToIndex == 3 ) {
  	   return true;
 	}
	return false;	

      case 2 :
	if ( InToIndex == 4 ) {
  	   return true;
 	}
	return false;

      case 3 :
	if ( InToIndex == 4 || InToIndex == 5 ) {
  	   return true;
 	}
	return false;

      case 4 :
	 if ( InToIndex == 6 ) {
  	   return true;
 	}
	return false;

      case 5 :
	if ( InToIndex == 6 ) {
  	   return true;
 	}
	return false;

      case 6 :
	return false;
    }
    return false;
  } else {
     switch (InFromIndex) {
      case 1 :
        if ( InToIndex == 2 || InToIndex == 3 ) {
  	   return true;
 	}
	return false;	

      case 2 :
	if ( InToIndex == 4 ) {
  	   return true;
 	}
	return false;

      case 3 :
	if ( InToIndex == 4 || InToIndex == 5 ) {
  	   return true;
 	}
	return false;

      case 4 :
	 if ( InToIndex == 6 ) {
  	   return true;
 	}
	return false;

      case 5 :
	if ( InToIndex == 6 || InToIndex == 7 ) {
  	   return true;
 	}
	return false;

      case 6 :
	if ( InToIndex == 8) {
  	   return true;
 	}
	return false;
  
      case 7 :
	if ( InToIndex == 8 ) {
  	   return true;
 	}
	return false;
  
      case 8 :
	return false;
    }
    return false;
  } 
  return false;
}

/*****************************************************************************!
 * Function : BayAppendPanelConnection
 *****************************************************************************/
void
BayAppendPanelConnection
(Bay* InBay, PanelConnection* InPanelConnection)
{
  if ( NULL == InBay || NULL == InPanelConnection ) {
    return;
  }
  if ( InBay->panelConnections ){
    PanelConnectionAppend(InBay->panelConnections, InPanelConnection);
  } else {
    InBay->panelConnections = InPanelConnection;
  }
  InPanelConnection->bay = InBay;
}

/*****************************************************************************!
 * Function : BayRemovePanelConnection
 *****************************************************************************/
bool
BayRemovePanelConnection
(Bay* InBay, PanelConnection* InConnection)
{
  PanelConnection*			connection;
  PanelConnection*			connectionPrev;

  if ( NULL == InBay || NULL == InConnection ) {
    return false;
  }

  connection = InBay->panelConnections;
  if ( connection->fromPanel == InConnection->fromPanel &&
       connection->toPanel == InConnection->toPanel ) {
    InBay->panelConnections = InBay->panelConnections->next;
    return true; 
  }

  connectionPrev = connection;
  connection = connection->next;
  while (connection) {
    if ( connection->toPanel == InConnection->toPanel &&
 	 connection->fromPanel == InConnection->fromPanel ) {
      connectionPrev->next = connection->next;
      return true;
    }
    connectionPrev = connection;
    connection = connection->next;
  }
  return false;
}

/*****************************************************************************!
 * Function : BayRemovePanelConnectionsByIndex
 *****************************************************************************/
bool
BayRemovePanelConnectionsByIndex
(Bay* InBay, int InPanelIndex)
{
  PanelConnection*			connection;
  bool					found;

  if ( NULL == InBay ) {
    return false;
  }

  while (true) {
    found = false;
    for ( connection = InBay->panelConnections; connection && !found ; connection = connection->next ) {
      if ( connection->toPanel == InPanelIndex || connection->fromPanel == InPanelIndex ) {
	BayRemovePanelConnection(InBay, connection);
	found = true;
      }
    } 
    if ( !found ) {
      break;
    }
  }
  return true;
}

/*****************************************************************************!
 * Function : BayAddRectifier
 *****************************************************************************/
void
BayAddRectifier
(Bay* InBay, Rectifier* InRectifier)
{
  Rectifier*				rectifier;

  if ( NULL == InBay || NULL == InRectifier ) {
    return;
  }

  if ( InBay->rectifiers ) {
    for ( rectifier = InBay->rectifiers; rectifier->next; rectifier = rectifier->next ) {
    }
    rectifier->next = InRectifier;
  } else {
    InBay->rectifiers = InRectifier;
  }
}

/*****************************************************************************!
 * Function : BayGetNextRectifierIndex
 * Purpose  : Find the lowest unused index
 *****************************************************************************/
uint16_t
BayGetNextRectifierIndex
(Bay* InBay)
{
  uint16_t				index;
  bool					found;
  Rectifier*				rectifier;

  if ( NULL == InBay ) {
    return 0;
  }
  index = 1;

  do { 
    found = true;
    for ( rectifier = InBay->rectifiers; rectifier; rectifier = rectifier->next ) {
      if ( rectifier->index == index ) {
	break;
      }
    }
    if ( rectifier ) {
      found = false;
      index++;
    }
  } while (!found);
  return index;
}

/*****************************************************************************!
 * Function : BayFindRectifierByIndex
 *****************************************************************************/
Rectifier*
BayFindRectifierByIndex
(Bay* InBay, uint16_t InIndex)
{
  Rectifier*				rectifier;

  if ( NULL == InBay ) {
    return NULL;
  }

  for ( rectifier = InBay->rectifiers; rectifier; rectifier = rectifier->next ) {
    if ( rectifier->index == InIndex ) {
      return rectifier;
    }
  }
  return NULL;
}

