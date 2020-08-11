/*****************************************************************************
 FILE NAME      : UserInputHandling.c
 DATE           : October 01 2018
 PROJECT        : 
 COPYRIGHT      : Copyright (C) 2018 by Gregory R Saltis
******************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "CanMsg.h"
#include "CANInterface.h"
#include "ThreadSafePrint.h"
#include "Devices.h"
#include "MemoryManager.h"
#include "UserInputHandling.h"
#include "main.h"
#include "Bay.h"
#include "WebConnection.h"
#include "WebSocketIF.h"
#include "linenoise.h"
#include "ANSIColors.h"
#include "Rectifier.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/
#define Upcase(s) {int i; for (i = 0; (s[i] != '\0' && s[i] != '\n'); i++) { s[i] = toupper(s[i]); } s[i] = '\0';}

/*******************************************************************************
 * Local Type : TempDevice
 *******************************************************************************/
struct TempDevice {
  string             		name;
  string			description;
  int                     	deviceID;
  int                     	protocolNumber;
  int                     	canaddress;
  time_t			lastContactTime;
};

/*****************************************************************************!
 * Local Functions 
 *****************************************************************************/
int
DeviceRegDefCompareByGroupName
(const void* InP1, const void* InP2);

bool
HandleClearCommand
(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE]);

bool
HandleOptionCommand
(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE]);

bool
HandleOptionSetCommand
(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE]);

bool
HandleOptionSetMonitorCANCommand
(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE]);

bool
HandleOptionSetMonitorWebCommand
(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE]);

void
HandleClearDevices
();

void
HandleClearDevice
(char Params[NUMPARAMS][COMMANDSIZE]);

void
HandleShowMessages
(char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandlePanelsCommand
(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE]);

bool 
HandleAdd
(char* Command, char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleAddBayCommand
(char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleAddPanelCommand
(char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleAddBreakerCommand
(char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleAddFuseCommand
(char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleAddFuseBreakerCommand
(char InParams[NUMPARAMS][COMMANDSIZE], string InType);

void
HandleListAllRegisters
();

void
HandleListRegisters
(char* InName);

bool
HandleDelay
(char* InCommand, char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandlePause
(char* InCommand, char InParams[NUMPARAMS][COMMANDSIZE]);

void
HandleError
();

bool
HandleSetInteractive
();

bool 
HandleUserCommand
(char* Command);

bool
HandleCloseCommand
(char InCommand[COMMANDSIZE], char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleConnectionsCommand
(char InCommand[COMMANDSIZE], char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleRemoveCommand
(string InCommand, char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleRemoveBayCommand
(char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleRemovePanelConnectCommand
(char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleRemovePanelCommand
(char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleRemoveFuseCommand
(char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleRemoveBreakerCommand
(char InParams[NUMPARAMS][COMMANDSIZE]);

bool
HandleRemoveFuseBreakerCommand
(char InParams[NUMPARAMS][COMMANDSIZE], string InType);

bool
HandleAddPanelConnectCommand
(char InParams[NUMPARAMS][COMMANDSIZE]);

void
HandleSetFuse
(char Params[NUMPARAMS][COMMANDSIZE]);

void
HandleListDevices
();

void
HandleListMessages
(char* InName, char* InMessageTypeString);

bool 
HandleSetN
(string Command, char Params[NUMPARAMS][COMMANDSIZE]);

void
HandleSetPanel
(char Params[NUMPARAMS][COMMANDSIZE]);;

void
HandleSetBay
(char Params[NUMPARAMS][COMMANDSIZE]);

void
HandleMonitorWWWCommand
(char Params[NUMPARAMS][COMMANDSIZE]);

bool
HandleAddRectifierCommand
(char InParams[NUMPARAMS][COMMANDSIZE]);

void
HandleShowCAN
(char InParams[NUMPARAMS][COMMANDSIZE]);

/*******************************************************************************
 Function : CompareTempDevice
 *******************************************************************************/
int CompareTempDevice
(
 const void*                    InP1,
 const void*                    InP2
)
{
  struct TempDevice*            d1;
  struct TempDevice*            d2;
  int                           n;

  d1 = (struct TempDevice*)InP1;
  d2 = (struct TempDevice*)InP2;

  n = strcmp(d1->name, d2->name);
  if ( n < 0 ) {
    return -1;
  }
  if ( n > 0 ) {
    return 1;
  }
  return d1->deviceID - d2->deviceID;
}

/*****************************************************************************!
 * Function : HandleBaysCommand
 *****************************************************************************/
bool HandleBaysCommand(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE])
{
  Bay*					bay;
  bool					haveBay;

  haveBay = false;
  
  if ( StringEqualNoCase(Params[1], "full") ) { 
    for (bay = BaysFirstBay(); bay ; bay = BaysNextBay(bay)) {
      if ( haveBay ) {
        printf("-----\n");
      }
      haveBay = true;
      BayDisplay(bay, 0, true);
    }
  } else {
    for ( bay = BaysFirstBay(); bay ; bay = BaysNextBay(bay)) {
      printf("%30s : %03d %d %s\n", bay->name, bay->canDevice->CanAddress, bay->index, bay->type->name);
    }
  } 
  return true;
}

/*****************************************************************************!
 * Function :  HandlePanelsCommand
 *****************************************************************************/
bool
HandlePanelsCommand
(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE])
{
  Bay*					bay;
  Panel*				panel;
  bool					haveBay;
  int					i;
  string				s;
  int					n;

  haveBay = false;
  for ( bay = BaysFirstBay(); bay; bay = BaysNextBay(bay) ) {
    if ( haveBay ) {
      printf("--------------------------------------------------------------------------------\n");
    }
    haveBay = true;
    BayDisplay(bay, 0, false);
    for ( i = 0; i < bay->panelCount; i++ ) {
      panel = bay->panels[i];
      if ( NULL == panel ) {
    	continue;
      }
      n = printf("Panel %d", panel->index);
      s = StringFill('=', 80 - n);
   
      printf("%s\n", s);
      FreeMemory(s);
      PanelDisplay(panel, 2, false);
    } 
  }
  return true;
}

/*****************************************************************************!
 * Function : HandleFuseBreakerCommand
 *****************************************************************************/
bool HandleFuseBreakersCommand(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE])
{
  Bay*					bay;
  Panel*				panel;
  FuseBreaker*				fusebreaker;
  string				stateColor;
  string				s;
  printf_safe("%sBAY PANEL INDEX                           TYPE                     TYPE CLASS  RATING  STATE%s\n", HeadingStyle, ColorWhite);
  for ( bay = BaysFirstBay(); bay; bay = BaysNextBay(bay) ) {
    for ( panel = BayFirstPanel(bay); panel; panel = BayNextPanel(bay, panel) ) {
      for ( fusebreaker = PanelFirstFuseBreaker(panel); fusebreaker; fusebreaker = PanelNextFuseBreaker(panel, fusebreaker) ) {
	s = FuseBreakerStateToString(fusebreaker->state);
        if ( fusebreaker->state == FuseBreakerStateOpen ) {
	  stateColor = ColorRed;
        } else {
	  stateColor = ColorGreen;
        } 
        printf_safe(" %2d    %2d    %2d %30s %30s     %3d %s%6s%s \n", bay->index, panel->index, fusebreaker->index, 
                    fusebreaker->type, fusebreaker->typeClass, fusebreaker->rating, stateColor, s, ColorWhite);

      }
    } 
  }
  return true;
}

/*****************************************************************************!
 * Function : HandleQuit
 *****************************************************************************/
bool HandleQuit(char* Command, char Params[NUMPARAMS][COMMANDSIZE])
{
        exit(EXIT_SUCCESS);
}

/*****************************************************************************!
 * Function : HandleAddPanelConnectCommand
 *****************************************************************************/
bool
HandleAddPanelConnectCommand
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  int						bayIndex, fromIndex, toIndex;
  Bay*						bay;
  Panel*					panel;
  PanelConnection*				connection;

  bayIndex = atoi(InParams[1]);
  fromIndex = atoi(InParams[2]);
  toIndex = atoi(InParams[3]);

  if ( bayIndex < 1 || bayIndex > BAY_MAX_INDEX ) {
    printf_safe("%sThe bay index must be between 1 and %d%s\n", ColorRed, BAY_MAX_INDEX, ColorWhite);
    return true;
  }

  bay = FindBayByIndex(bayIndex);
  if ( NULL == bay ) {
    printf_safe("%sA Bay with an index of %d does not exist%s\n",ColorRed, bayIndex, ColorWhite);
    return true;
  }

  if ( fromIndex == toIndex ) {
    printf_safe("%sCan not create a panel connection to itself%s\n",ColorRed, ColorWhite);
    return true;
  }
    
  panel = BayFindPanelByIndex(bay, fromIndex);
  if ( NULL == panel ) {
    printf_safe("%sA panel in Bay %d with an index of %d does not exist%s\n",ColorRed, bayIndex, fromIndex, ColorWhite);
    return true;
  }

  panel = BayFindPanelByIndex(bay, toIndex);
  if ( NULL == panel ) {
    printf_safe("%sA panel in Bay %d with an index of %d does not exist%s\n",ColorRed, bayIndex, toIndex, ColorWhite);
    return true;
  }

  connection = BayFindPanelConnectionByIndices(bay, fromIndex, toIndex);
  if ( connection ) {
    printf_safe("%sA Connection from panel %d to panel %d in Bay %d exists%s\n", ColorRed, fromIndex, toIndex, bayIndex, ColorWhite); 
    return true;
  }

  if ( !BayVerifyNewPanelConnection(bay, fromIndex, toIndex) ) {
    printf_safe("%sCan not create a connection from panel %d to panel %d in Bay %d%s\n", ColorRed, fromIndex, toIndex, bayIndex, ColorWhite);
    return true;
  }

  connection = CreatePanelConnection(fromIndex, toIndex);
  BayAppendPanelConnection(bay, connection);
  BaysSave(NULL);
  BaysSaveValues(NULL);
  WebSocketIFSendPanelConnect(connection);

  printf_safe("%sA connection from panel %d to panel %d in Bay %d has been created%s\n", ColorGreen, fromIndex, toIndex, bayIndex, ColorWhite);
  return true;
}

/*****************************************************************************!
 * Function : HandleRemoveCommand
 *****************************************************************************/
bool
HandleRemoveCommand
(string InCommand, char InParams[NUMPARAMS][COMMANDSIZE]) {
  if ( StringEqualNoCase(InParams[0], "BAY") ) {
    return HandleRemoveBayCommand(InParams);
  } else if ( StringEqualNoCase(InParams[0], "PANEL") ) {
    return HandleRemovePanelCommand(InParams);
  }
  return false;
}

/*****************************************************************************!
 * Function : HandleRemovePanelConnectCommand
 *****************************************************************************/
bool
HandleRemovePanelConnectCommand
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  int						bayIndex, fromIndex, toIndex;
  Bay*						bay;
  Panel*					panel;
  PanelConnection*				connection;

  bayIndex = atoi(InParams[1]);
  fromIndex = atoi(InParams[2]);
  toIndex   = atoi(InParams[3]);

  if ( bayIndex == 0 ) {
    printf_safe("A bay index is required\n");
    return true;
  }
  if ( fromIndex == 0 ) {
    printf_safe("A from panel index is required\n");
    return true;
  }
  if ( toIndex == 0 ) {
    printf_safe("A to panel index is required\n");
    return true;
  }

  bay = FindBayByIndex(bayIndex);
  if ( NULL == bay ) {
    printf_safe("Could not find a bay with an index of %d\n", bayIndex); 
    return true;
  }

  panel = BayFindPanelByIndex(bay, fromIndex);
  if ( NULL == panel ) {
    printf_safe("Could not find a panel with an index of %d\n", fromIndex);
    return true;
  }

  panel = BayFindPanelByIndex(bay, toIndex);
  if ( NULL == panel ) {
    printf_safe("Could not find a panel with an index of %d\n", toIndex);
    return true;
  }

  connection = BayFindPanelConnectionByIndices(bay, fromIndex, toIndex);
  if ( NULL == connection ) {
    printf_safe("Could not find a panel connection from %d to %d\n", fromIndex, toIndex);
    return true;
  }
  WebSocketIFSendRemovePanelConnect(connection);
  if ( BayRemovePanelConnection(bay, connection) ) {
    PanelConnectionDestroy(connection);
    BaysSave(NULL);
    printf_safe("Removed the panel connection in bay %d from panel %d to panel %d\n", bayIndex, fromIndex, toIndex);
    return true;
  }
  printf_safe("Could not remove the panel connection in bay %d from panel %d to panel %d\n", bayIndex, fromIndex, toIndex); 
  return true;
}

/*****************************************************************************!
 * Function : HandleRemoveBayCommand
 *****************************************************************************/
bool
HandleRemoveBayCommand
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  int					bayindex;
  Bay*					bay;

  bayindex = atoi(InParams[1]);
  if ( bayindex < 1 || bayindex > BAY_MAX_INDEX ) {
    printf_safe("%sThe bay index must be between 1 and %d%s\n", ColorRed, BAY_MAX_INDEX, ColorWhite);
    return true;
  }

  bay = FindBayByIndex(bayindex);
  if ( NULL == bay ) {
    printf_safe("%sA Bay with an index of %d does not exist%s\n",ColorRed, bayindex, ColorWhite);
    return true;
  }

  BayDestroy(bay);
  WebSocketIFRemoveBay(bayindex);
  BaysSave(NULL);
  BaysSaveValues(NULL);
  printf_safe("%sBay index %d has been removed%s\n", ColorGreen, bayindex, ColorWhite);
  return true;
}

/*****************************************************************************!
 * Function : HandleRemovePanelCommand
 *****************************************************************************/
bool
HandleRemovePanelCommand
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  int					bayIndex, panelIndex;
  Bay*					bay;
  Panel*				panel;
  int					i, index;
  bool					panelsRemoved;

  bayIndex = atoi(InParams[1]);
  panelIndex = atoi(InParams[2]);

  //! Validate the index and make sure the panel exists
  bay = FindBayByIndex(bayIndex);
  if ( bay == NULL ) {
    printf_safe("%sA Bay with an index of %d does not exist%s\n",ColorRed, bayIndex, ColorWhite);
    return true;
  }

  //! Remove all panels
  if ( StringEqualNoCase("all", InParams[2]) ) {
    panelsRemoved = false;
    for (i = 0; i < bay->panelCount; i++ ) {
      index = i + 1;
      panel = BayFindPanelByIndex(bay, index);
      if ( panel ) {
	panelsRemoved = true;
        BayRemovePanelByIndex(bay, index);
      }
    }
    if ( panelsRemoved ) {
      printf_safe("%sRemoved all panels from bay %d\n%s", ColorGreen, bayIndex, ColorWhite);
    } else {
      printf_safe("No panels to remove from bay %d\n", ColorRed, bayIndex, ColorWhite);
    }
    return true;
  }

  //! Remove a single panel  
  panel = BayFindPanelByIndex(bay, panelIndex); 
  if ( panel == NULL ) {
    printf_safe("%sA panel in Bay %d with an index of %d does not exist%s\n",ColorRed, bayIndex, panelIndex, ColorWhite);
    return true;
  }
 
  BayRemovePanelByIndex(bay, panelIndex);
  WebSocketIFRemovePanel(bayIndex, panelIndex);
  BaysSave(NULL);
  BaysSaveValues(NULL);
  printf_safe("%sPanel index %d has been removed from Bay %d%s\n", ColorGreen, panelIndex, bayIndex, ColorWhite);

  return true;
}

/*****************************************************************************!
 * Function : HandleRemoveFuseCommand
 *****************************************************************************/
bool
HandleRemoveFuseCommand
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  return HandleRemoveFuseBreakerCommand(InParams, "fuse");
}

/*****************************************************************************!
 * Function : HandleRemoveBreakerCommand
 *****************************************************************************/
bool
HandleRemoveBreakerCommand
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  return HandleRemoveFuseBreakerCommand(InParams, "breaker");
}

/*****************************************************************************!
 * Function : HandleRemoveBreakerCommand
 *****************************************************************************/
bool
HandleRemoveFuseBreakerCommand
(char InParams[NUMPARAMS][COMMANDSIZE], string InType)
{
  int					bayIndex, panelIndex, fuseBreakerIndex;
  Bay*					bay;
  Panel*				panel;
  FuseBreaker*				fuseBreaker;

  bayIndex = atoi(InParams[1]);
  panelIndex = atoi(InParams[2]);
  fuseBreakerIndex = atoi(InParams[3]);

  bay = FindBayByIndex(bayIndex);
  if ( NULL == bay ) {
    printf_safe("%sA Bay with an index of %d does not exist%s\n",ColorRed, bayIndex, ColorWhite);
    return true;
  }
  
  panel = BayFindPanelByIndex(bay, panelIndex);
  if ( NULL == panel ) {
    printf_safe("%sA panel in Bay %d with an index of %d does not exist%s\n",ColorRed, bayIndex, panelIndex, ColorWhite);
    return true;
  }

  fuseBreaker = PanelFindFuseBreaker(panel, fuseBreakerIndex);
  if ( NULL == fuseBreaker ) {
    printf_safe("%sA %s with an index of %d in Bay %d Panel %d does not exist%s\n",ColorRed, InType, fuseBreakerIndex, bayIndex, panelIndex, ColorWhite);
    return true;
  }
  PanelRemoveFuseBreakerByIndex(panel, fuseBreakerIndex);
  BaysSave(NULL);
  BaysSaveValues(NULL);
  printf_safe("%sA %s index %d has been removed from Bay %d Panel %d%s\n", ColorGreen, InType, fuseBreakerIndex, bayIndex, panelIndex, ColorWhite);

  WebSocketIFRemoveFuseBreaker(bayIndex, panelIndex, fuseBreakerIndex);
  return true;
}

/*****************************************************************************!
 * Function : HandleAdd
 *****************************************************************************/
bool 
HandleAdd
(char* Command, char InParams[NUMPARAMS][COMMANDSIZE])
{
  if ( StringEqualNoCase(InParams[0], "DEVICE") ) {
    int ReqParams = 3;
    for (int i = 0; i < ReqParams; i++) {
      if (strlen(InParams[i]) == 0 ) {
        return false;
      }
    }
    int Quan = atoi(InParams[2]);
    AddDevice(&InParams[1][0], Quan);
    return true;
  }
  if ( StringEqualNoCase(InParams[0], "BAY") ) {
    return HandleAddBayCommand(InParams);
  }
  if ( StringEqualNoCase(InParams[0], "PANEL") ) {
    return HandleAddPanelCommand(InParams);
  }
  if ( MainSupportRectifiers ) {
    if ( StringEqualNoCase(InParams[0], "REC") ) {
      return HandleAddRectifierCommand(InParams);
    }
  }
  return false;
}

/*****************************************************************************!
 * Function : HandleAddRectifierCommand
 *****************************************************************************/
bool
HandleAddRectifierCommand
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  Rectifier*				rectifier;
  Bay*					bay;
  uint16_t				index;

  bay = BaysFirstBay();
  index = BayGetNextRectifierIndex(bay); 
  rectifier = CreateRectifier(index);
  BayAddRectifier(bay, rectifier);
  printf("Rectifier %d added to Bay %d\n", rectifier->canDevice->CanAddress, bay->index);
  BaysSave(NULL);
  return true; 
}

/*****************************************************************************!
 * Function : HandleEnable
 *****************************************************************************/
bool HandleEnable(char* Command, char Params[NUMPARAMS][COMMANDSIZE])
{
        int ReqParams = 3;
        for (int i = 0; i < ReqParams; i++) if (strlen(Params[i]) == 0)  return false;

        if (strcasecmp(Params[0], "DEVICE") != 0) return false;
        int DeviceNum = atoi(Params[2]);
        EnableDevice(&Params[1][0], DeviceNum);
        return true;
}

/*****************************************************************************!
 * Function : HandleDisable
 *****************************************************************************/
bool HandleDisable(char* Command, char Params[NUMPARAMS][COMMANDSIZE])
{
        int ReqParams = 3;
        for (int i = 0; i < ReqParams; i++) if (strlen(Params[i]) == 0)  return false;

        if (strcasecmp(Params[0], "DEVICE") != 0) return false;
        int DeviceNum = atoi(Params[2]);
        DisableDevice(&Params[1][0], DeviceNum);
        return true;
}

/*******************************************************************************!
 * Function : HandleStatus
 *******************************************************************************/
bool
HandleStatus()
{
   extern bool PortOpened;
   
   printf_safe("CAN PORT : %s\n", PortOpened ? "Opened" : "Closed");
   return true;
}

/*****************************************************************************!
 * Function : HandleListDeviceTypes
 *****************************************************************************/
void
HandleListDeviceTypes()
{
    printf_safe("DEVICE TYPES :\n");
    for ( DeviceDef* deviceDef = mainDeviceDefs->defs; deviceDef; deviceDef = deviceDef->next ) {
        printf_safe("  %s\n", deviceDef->name);
    }
}

/*****************************************************************************!
 * Function : HandleListDeviceType
 *****************************************************************************/
void
HandleListDeviceType
(
 char*                                  InDeviceTypeName
)
{
    DeviceDef*                          deviceDef;
    
    deviceDef = FindDeviceDef(mainDeviceDefs, InDeviceTypeName);
    if ( NULL == deviceDef ) {
        printf_safe("Device %s not found\n", InDeviceTypeName);
        return;
    }
    printf_safe("    Device Name : %s\n", deviceDef->name);
    printf_safe("    Description : %s\n", deviceDef->desc);
    printf_safe("    CAN Address : 0x%02x\n", deviceDef->canAddressStart);
    printf_safe("Protocol Number : 0x%02x\n", deviceDef->protocolNumber);
    printf_safe("   Message Type : 0x%02x\n", deviceDef->messageType);
    printf_safe("      Registers : %d\n", DeviceDefGetRegCount(deviceDef));
    printf_safe("       Messages : %d\n", DeviceDefGetMessageCount(deviceDef));
}

/*****************************************************************************!
 * Function : HandleList 
 *****************************************************************************/
bool
HandleList(char* Command, char Params[NUMPARAMS][COMMANDSIZE])
{
  if ( strlen(Params[0]) == 0 ) {
    return false;
  }
  if ( StringEqualNoCase(Params[0], "DEVICETYPES") ) {
    if ( strlen(Params[1]) == 0 ) {
      HandleListDeviceTypes();
    } else {
      HandleListDeviceType(Params[1]);
    }
    return true;
  }
  if ( StringEqualNoCase(Params[0], "REGISTERS") ) {
    if ( strlen(Params[1]) == 0 ) {
      HandleListAllRegisters();
    } else {
      HandleListRegisters(Params[1]);
    }
    return true;
  }

  if ( StringEqualNoCase(Params[0], "MESSAGES") ) {
    if ( Params[1] != 0 ) {
      HandleListMessages(Params[1], Params[2]);
    }
    return true;
  }

  if ( StringEqualNoCase(Params[0], "DEVICES") ) {
    HandleListDevices();
    return true;
  }
  return false;
}

/*****************************************************************************!
 * Function : HandleListDevices
 *****************************************************************************/
void
HandleListDevices
()
{
  int					i, k, n, m, nameSize, descSize;
  CanDevice*				devices;
  struct TempDevice*			tDevices;
  string 	 			dividerLine;
  char*                                 lastDevice;
  n = GetNumDevice();
  nameSize = strlen("Name");
  descSize = strlen("Description");
  devices = GetCanDevice();
        
  if ( n > 0 ) {
    tDevices = (struct TempDevice*)malloc(sizeof(struct TempDevice) * n);
    for ( i = 0 ; i < n ; i++ ) {
      k = strlen(devices[i].deviceDefinition->name);
      if ( k > nameSize ) {
        nameSize = k;
      }
      k = strlen(devices[i].deviceDefinition->desc);
      if ( k > descSize ) {
        descSize = k;
      }
      tDevices[i].name = devices[i].deviceDefinition->name;
      tDevices[i].description = devices[i].deviceDefinition->desc;
      tDevices[i].deviceID = devices[i].CanAddress - devices[i].deviceDefinition->canAddressStart + 1;
      tDevices[i].protocolNumber = devices[i].deviceDefinition->protocolNumber;
      tDevices[i].canaddress = devices[i].CanAddress;
      tDevices[i].lastContactTime = devices[i].lastContactTime;
    }
  }
  m = descSize + nameSize + 25;
  dividerLine = StringFill('-', m);
  qsort(tDevices, n, sizeof(struct TempDevice), CompareTempDevice);
  printf_safe("%s\n", dividerLine);
  printf_safe("      %*s %*s  Id Address Proto\n", nameSize, "Name", descSize, "Description");
  printf_safe("%s\n", dividerLine);
  if ( n > 0 ) {
    lastDevice = (char*)tDevices[0].name;
  }
  for (i = 0 ; i < n ; i++) {
    if ( strcmp(lastDevice, tDevices[i].name) != 0 ) {
      lastDevice = (char*)tDevices[i].name;
      printf_safe("%s\n", dividerLine);
    }
    printf_safe("%3d : %*s %*s %4d     %3d  0x%0X %s\n", i+1, nameSize, tDevices[i].name, descSize, tDevices[i].description, tDevices[i].deviceID, tDevices[i].canaddress, tDevices[i].protocolNumber, 

                asctime(localtime(&tDevices[i].lastContactTime)));
  }
  if ( n > 0 ) {
    free(tDevices);
  }
  FreeMemory(dividerLine);
}

/*****************************************************************************!
 * 
 *****************************************************************************/
//
// Handle the command to set the output file for CAN message logging
//  0: filespec 
//

bool HandleOutput(char* Command, char Params[NUMPARAMS][COMMANDSIZE])
{
        int ReqParams = 1;
        for (int i = 0; i < ReqParams; i++) if (strlen(Params[i]) == 0)  return false;
        // OpenCANOutputFile(Params[0]);
        return true;
}

/*****************************************************************************!
 * 
 *****************************************************************************/
//
// Initialize a CAN watch in the CAN interface, to watch for a specific protocol, protocol/register, or source address
//  0: watch type
//  1: varies
//  2: varies

bool HandleWatch(char* Command, char Params[NUMPARAMS][COMMANDSIZE])
{
        if (strlen(Params[0]) == 0)  return false;
        if (strcasecmp(Params[0], "ADDRESS") == 0)
        {
                //if (strlen(Params[1]) == 0)  return false;
                //int Address;
                //if (strcasestr(Params[1], "0X") != NULL) Address = strtol(Params[1], NULL, 16);
                //else Address = atoi(Params[1]);
                //SetCANWatchAddress(Address);
                return true;
        }

        else if (strcasecmp(Params[0], "REGISTER") == 0)
        {
                //if (strlen(Params[1]) == 0)  return false;
                //int Protocol;
                //if (strcasestr(Params[1], "0X") != NULL) Protocol = strtol(Params[1], NULL, 16);
                //else Protocol = atoi(Params[1]);

                //if (strlen(Params[2]) == 0)  return false;
                //int Register;
                //if (strcasestr(Params[2], "0X") != NULL) Register = strtol(Params[2], NULL, 16);
                //else Register = atoi(Params[2]);
                // SetCANWatchRegister(Protocol, Register);
                return true;
        }

        else if (strcasecmp(Params[0], "PROTOCOL") == 0)
        {
                //if (strlen(Params[1]) == 0)  return false;
                //int Protocol;
                //if (strcasestr(Params[1], "0X") != NULL) Protocol = strtol(Params[1], NULL, 16);
                //else Protocol = atoi(Params[1]);
                // SetCANWatchProtocol(Protocol);
                return true;
        }

        else if (strcasecmp(Params[0], "OFF") == 0)
        {
                // SetCANWatchCancel();
                return true;
        }
        else return false;
}

bool HandlePing(char* Command, char Params[NUMPARAMS][COMMANDSIZE])
{
        // frameid Id; Id.data32 = 0x0A4EDF83; // 0x0A6EDF83
        // dataframe Data; Data.data64 = 0x00F0000000000000;
        // CanSend(Id, Data); 
        return true;
}

//
// Handle the command to start CAN message output to the screen (monitoring)
//

//extern bool OutputToFile;
bool HandleMonitor(char* Command, char Params[NUMPARAMS][COMMANDSIZE])
{
//        OutputToFile = false;
        return true;
}

//
// Handle the exit command.  control-C is used to exit, though, to make the thread management simple
//

bool HandleExit()
{
       // CloseOutputFile();
        return true;
}

//
// Handle the command to dump the device information
//  0: watch type
//  1: varies
//  2: varies
//      3: varies
//

/*****************************************************************************!
 * Function : HandleShow
 *****************************************************************************/
bool HandleShow(char* Command, char InParams[NUMPARAMS][COMMANDSIZE])
{
  if (strlen(InParams[0]) == 0)  {
    return false;
  } else if (StringEqualNoCase(InParams[0], "BAY") ) {
    Bay* bay = FindBayByIndex(atoi(InParams[1]));
    if ( bay ) {
      BayDisplay(bay, 0, true);
    } else {
      printf_safe("Could not a bay with the index %d\n", atoi(InParams[1]));
      return true;
    }
    return true;
  }
  if (strcasecmp(InParams[0], "DEVICE") == 0) {
    if (strlen(InParams[1]) == 0)  return false; // device name
    if (strlen(InParams[2]) == 0)  return false; // device number
    int Count = atoi(InParams[2]);

    DumpDevice(&InParams[1][0], Count);
    return true;
  }
  if (strcasecmp(InParams[0], "GROUP") == 0) {
    if (strlen(InParams[1]) == 0)  return false;      // device name
    if (strlen(InParams[2]) == 0)  return false; // device number
    if (strlen(InParams[3]) == 0)  return false; // group name
    int Count = atoi(InParams[2]);
    DumpGroup(&InParams[1][0], Count, &InParams[3][0]);
    return true;
  }
  if (strcasecmp(InParams[0], "ALL") == 0) {
    DumpDefs();
    return true;
  }
  if (strcasecmp(InParams[0], "MESSAGES") == 0) {
    HandleShowMessages(InParams);
    return true;
  }

  if ( StringEqualNoCase(InParams[0], "CAN") ) {
    HandleShowCAN(InParams);
    return true;
  }
  return false;
}

/*****************************************************************************!
 * Function : HandleShowCAN
 *****************************************************************************/
void
HandleShowCAN
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  printf_safe("CAN Status\n");
  printf_safe("  CAN In Messages  : %d\n", CANInterfaceInMessageCount);
  printf_safe("  CAN Out Messages : %d\n", CANInterfaceOutMessageCount);
}
/*****************************************************************************!
 * Function : HandleShowMessages
 *****************************************************************************/
void
HandleShowMessages
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  DeviceDef*                            deviceDef;

  if ( InParams[1] == NULL || InParams[1][0] == 0x00 ) {
    DisplayDeviceDefsMessages();
    return;
  }

  deviceDef = FindDeviceDef(mainDeviceDefs, InParams[1]);
  if ( deviceDef == NULL ) {
    printf_safe("COuld not find a device definition named %s\n", InParams[1]);
    return;
  }
  DisplayDeviceDefMessages(deviceDef);
}
 
/*****************************************************************************!
 * Function : HandleDump
 *****************************************************************************/
bool HandleDump(char* Command, char Params[NUMPARAMS][COMMANDSIZE])
{
        DumpDefs();
        return true;
}


/*****************************************************************************!
 * Function : HandleSetN
 *****************************************************************************/
// Handle the command to set the value of a register in a device using register number (N)
// 0 : device name
// 1 : device number
// 2 : register number
// 3 : new value
bool 
HandleSetN
(string Command, char Params[NUMPARAMS][COMMANDSIZE])
{
  int						bayindex, panelindex;
  ufloatbit32_t					value;
  Bay*						bay;
  Panel*					panel;
  bool						b;
  StringList*					registerStrings;
  int						registernumber;
  int						registerNumberStart, registerNumberEnd;
  
  bayindex = atoi(Params[0]);
  panelindex = atoi(Params[1]);


  // Find Bay 
  bay = FindBayByIndex(bayindex);
  if ( NULL == bay ) {
    return false;
  }

  // Find Panel
  panel = BayFindPanelByIndex(bay, panelindex);
  if ( panel == NULL ) {
    return false;
  }

  // If this panel does not contain an SMDUH2, return
  if ( !panel->panelType->usesSMDUH2 ) {
    return false;
  }  

  // Get the register numbers
  registerStrings = StringSplit(Params[2], ":", true);
 
  // We need either a single or pair of register numbers 
  if ( registerStrings->stringCount != 1 && registerStrings->stringCount != 2 ) {
    StringListDestroy(registerStrings);
    return false;
  }

  // Get the register numbers
  registerNumberStart = GetIntValueFromString(&b, registerStrings->strings[0]);
  if ( registerStrings->stringCount == 2 ) { 
    registerNumberEnd = GetIntValueFromString(&b, registerStrings->strings[1]);
  } else {
    registerNumberEnd = registerNumberStart;
  }

  // Get the values
  if (strcasestr(Params[3], "0X") != NULL)
    value.data32 = strtoul(Params[3], NULL, 16);
  else
    value.fd = atof(Params[3]);

  // Iterate over the requested register numbers
  for ( registernumber = registerNumberStart; registernumber <= registerNumberEnd; registernumber++ ) {
    SetRegisterValue(panel->smduh2->deviceDefinition->name, panel->index, registernumber, value); 
  }
  BaysSaveValues(NULL);
  return true;
}
/*
{
        int ReqParams = 4;
        for (int i = 0; i < ReqParams; i++) if (strlen(Params[i]) == 0)  return false;

        int Count = atoi(Params[1]);

        int RegisterNum = 0;
        if (strcasestr(Params[2], "0X") != NULL) RegisterNum = strtol(Params[2], NULL, 16);
        else RegisterNum = atoi(Params[2]);

        // next word, value, can be in decimal (as a floating number) or in hex
        ufloatbit32_t NewValue; NewValue.data32 = 0;
        if (strcasestr(Params[3], "0X") != NULL)
                NewValue.data32 = strtoul(Params[3], NULL, 16);
        else
                NewValue.fd = atof(Params[3]);

        SetRegisterValue(Params[0], Count, RegisterNum, NewValue);
        return true;
}
*/

//
// Handle the command to set the value of a register in a device using group name and abbreviation
// 0 : device name
// 1 : device number
// 2 : group
// 3 : abbreviation
// 4: new value

bool HandleSet(char* Command, char Params[NUMPARAMS][COMMANDSIZE])
{
  
  if (strlen(Params[0]) == 0) {
    return HandleSetInteractive();
  }

  if ( StringEqualNoCase(Params[0], "bay") ) {
    HandleSetBay(Params);
    return true;
  }

  if ( StringEqualNoCase(Params[0], "panel") ) {
    HandleSetPanel(Params);
    return true;
  }

  if ( StringEqualNoCase(Params[0], "fuse") ) {
    HandleSetFuse(Params);
    return true;
  } 
  int ReqParams = 5;
  for (int i = 0; i < ReqParams; i++) if (strlen(Params[i]) == 0)  return false;

  // convert the device name into a pointer MyDeviceDef
  DeviceDef* deviceDef = FindDeviceDef(mainDeviceDefs, Params[0]);
  if ( deviceDef == NULL ) {
    return false;
  }
  int Count = atoi(Params[1]);

  // next word, value, can be floating point or hex
  ufloatbit32_t NewValue; NewValue.data32 = 0;
  if (strcasestr(Params[4], "0X") != NULL)
    NewValue.data32 = strtoul(Params[4], NULL, 16);
  else
    NewValue.fd = atof(Params[4]);

  // convert the group and abbreviation into a register number
  int MyRegister = GetRegisterByGroup(Params[2], Params[3], deviceDef);

  // set the register value
  SetRegisterValue(Params[0], Count, MyRegister, NewValue);
  return true;
}

/*****************************************************************************!
 * Function : HandleSetPanel 
 *****************************************************************************/
void
HandleSetPanel
(char Params[NUMPARAMS][COMMANDSIZE])
{
  int					bayindex, panelindex, valuetype;
  Bay*					bay;
  Panel*				panel;
  CanReg*				canReg;
 
  bayindex = atoi(Params[1]);
  panelindex = atoi(Params[2]);
  valuetype = atoi(Params[3]);

  bay = FindBayByIndex(bayindex);
  if ( NULL == bay ) {
    fprintf(stderr, "Could not find a bay with an index of %d\n", bayindex);
    return;
  }
  panel = BayFindPanelByIndex(bay, panelindex);
  if ( NULL == panel ) {
    fprintf(stderr, "Could not a find a panel with an index of %d in bay %d\n", bayindex, panelindex);
    return;
  }

  if ( !panel->panelType->usesSMDUH2 ) {
    fprintf(stderr, "Bay %d Panel %d is not an Advanced Panel\n", bayindex, panelindex);
    return;
  }

  canReg = PanelFindCANRegister(panel, valuetype);
  if ( NULL == canReg ) {
    fprintf(stderr, "Bay %d Panel %d does not contain a register %d\n", bayindex, panelindex, valuetype);
    return;
  }
  
  CanRegSetFromString(canReg, Params[4]);
  printf_safe("Set Bay %d Panel %d register %d to %s\n", bayindex, panelindex, valuetype, Params[4]);
  BaysSaveValues(NULL);
}

/*****************************************************************************!
 * Function : HandleSetBay 
 *****************************************************************************/
void
HandleSetBay
(char Params[NUMPARAMS][COMMANDSIZE])
{
  int					bayindex, valuetype;
  Bay*					bay;
  CanReg*				canReg;

  bayindex = atoi(Params[1]);
  valuetype = atoi(Params[2]);

  bay = FindBayByIndex(bayindex);
  if ( NULL == bay ) {
    fprintf(stderr, "Could not find a bay with an index of %d\n", bayindex);
    return;
  }
 
  canReg = BayFindCANRegister(bay, valuetype);
  if ( NULL == canReg ) {
    fprintf(stderr, "Could not find a register with a type of %d in bay %d\n", valuetype, bayindex);
    return;
  }

  CanRegSetFromString(canReg, Params[3]);
  printf_safe("Set Bay %d register %d to %s\n", bayindex, valuetype, Params[3]);
  BaysSaveValues(NULL);
}

/*****************************************************************************!
 * Function : HandleSetFuse
 *****************************************************************************/
void
HandleSetFuse
(char Params[NUMPARAMS][COMMANDSIZE])
{
}

//
// Set the register in a group, each on incremented from previous
//
// 0 : device name
// 1 : device number
// 2 : group
// 3 : starting value
// 4 : increment
// 

bool HandleIncrement(char* Command, char Params[NUMPARAMS][COMMANDSIZE])
{
        int ReqParams = 5;
        for (int i = 0; i < ReqParams; i++) if (strlen(Params[i]) == 0)  return false;

        // convert the device name into a pointer MyDeviceDef
        DeviceDef* deviceDef = FindDeviceDef(mainDeviceDefs, Params[0]);
        if ( NULL == deviceDef ) {
            return false;
        }
        int Count = atoi(Params[1]);
        // next word, value, can be floating point or hex
        ufloatbit32_t NewValue; NewValue.data32 = 0;
        if (strcasestr(Params[3], "0X") != NULL)
                NewValue.data32 = strtoul(Params[3], NULL, 16);
        else
                NewValue.fd = atof(Params[3]);

        float IncValue = atof(Params[4]);

        bool Done = false;
        int AbbrevNum = 0;
        char MyAbbrev[COMMANDSIZE];
        while (!Done)
        {
                AbbrevNum++;
                sprintf(MyAbbrev, "%d", AbbrevNum);

                // convert the group and abbreviation into a register number
                int MyRegister = GetRegisterByGroup(Params[2], MyAbbrev, deviceDef);
                if (MyRegister == 0) Done = true;
                else
                {
                        // set the register value
                        SetRegisterValue(Params[0], Count, MyRegister, NewValue);
                        NewValue.fd = NewValue.fd + IncValue;
                }
        }
        return true;
}

/*****************************************************************************!
 * Function : HandleSetInteractive
 * Purpose  : Prompt user to set the value of a register
 *****************************************************************************/
bool HandleSetInteractive()
{
  int					bayIndex, panelIndex, deviceIndex;
  string				prompt;
  string				bayIndexList;
  string				panelIndexList;
  string				deviceIndexList;
  float					value;
  string				s;
  Bay*					bay;
  Panel*				panel;
  FuseBreaker*				device;
  char					s2[17];
  bool					floatOK;
 
  (void)value;
  bayIndexList = BaysGetIndexList();
  prompt = StringMultiConcat("Select Bay [", bayIndexList, "] :", NULL);
  s = linenoise(prompt);
  FreeMemory(bayIndexList);
  FreeMemory(prompt);
  bayIndex = atoi(s);
  free(s);  

  if ( bayIndex == 0 ) {
    printf_safe("A Bay index is required\n");
    return true;
  }
  bay = FindBayByIndex(bayIndex);
  if ( NULL == bay ) {
    printf_safe("Could not find a bay with index %d\n", bayIndex);
    return true;
  }

  panelIndexList = BayGetPanelIndexList(bay, false);
  prompt = StringMultiConcat("Select Panel [", panelIndexList, "] : ", NULL);
  s = linenoise(prompt);
  panelIndex = atoi(s);
  free(s);
  FreeMemory(prompt);
  FreeMemory(panelIndexList);
  if ( panelIndex == 0 ) {
    printf_safe("A panel index is required\n");
    return true;
  }
  panel = BayFindPanelByIndex(bay, panelIndex);
  if ( NULL == panel ) {
    printf_safe("Could not find a panel with an index of %d in Bay %d\n", panelIndex, bayIndex);
    return true;
  }

  deviceIndexList = PanelGetFuseBreakerList(panel);
  if ( NULL == deviceIndexList ) {
    printf_safe("Bay %d Panel %d contains no devices\n", bayIndex, panelIndex);
    return true;
  }
  prompt = StringMultiConcat("Select Device [", deviceIndexList, "] : ", NULL);
  s = linenoise(prompt);
  deviceIndex = atoi(s);

  FreeMemory(prompt);
  free(s);
  if ( deviceIndex == 0 ) {
    printf_safe("A device requires an index\n");
    return true;
  }
  device = PanelFindFuseBreaker(panel, deviceIndex);
  if ( NULL == device ) {
    printf_safe("Bay %d Panel %d does not have device with an index of %d\n",
                bayIndex, panelIndex, deviceIndex);
    return true;
  }

  prompt = "Enter current value : ";

  s = linenoise(prompt);
  value = GetFloatValueFromString(&floatOK, s);
  
  free(s);
  if ( !floatOK ) {
    printf_safe("Need a value of %s\n", s);
    return true;
  } 
  snprintf(s2, sizeof(s2) - 1, "%d", deviceIndex); 
  SetDeviceRegister(panel->smduh2, s2, "Current", value);
  return true;
}

//
// Display all of the available commands
//
bool HandleHelp()
{
  printf_safe("\n\nCommands: \n\n");
  printf_safe("%sADD DEVICE <device-name> <quantity>                             %sAdd a device to the simulator\n", ColorRed, ColorReset);
  printf_safe("%sADD BAY <bayindex>                                              %sAdd a bay to a system\n", ColorRed, ColorReset);
  printf_safe("%sADD PANEL <bayindex> [all | <panelindex>]                       %sAdd a panel to a bay\n", ColorRed, ColorReset);
  if ( MainSupportRectifiers ) {
    printf_safe("%sADD REC                                                         %sAdd a rectifier\n", ColorRed, ColorReset);
  }
  printf_safe("\n");
  printf_safe("%sREMOVE BAY <bayindex>                                           %sRemove a bay from a system\n", ColorRed, ColorReset);
  printf_safe("%sREMOVE PANEL <bayindex> <panelindex>                            %sRemove a panel from a bay\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sLIST DEVICES                        %s to list devices\n", ColorRed, ColorReset);
  printf_safe("%sLIST DEVICETYPES <device-name>      %s to list all available devices types ('type' display all information for a specific type\n", ColorRed, ColorReset);
  printf_safe("%sLIST REGISTERS <device-name>        %s to list all registers ('type' for registers for specific type\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sCLEAR DEVICES                              %s to clear all regsiters in all devices\n", ColorRed, ColorReset);
  printf_safe("%sCLEAR DEVICE <device-name> <device-number> %s to clear all registers in a specific device\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sSET  %s                                                                          to get prompted for the input to set a register value in a device \n", ColorRed, ColorReset);
  printf_safe("%sSET <device-name> <device-number> <group-name> <abbreviation> <new-value> %s     to set a register value in a device \n", ColorRed, ColorReset);
  printf_safe("%sSETN bayindex panelindex registernumber{:registernumber} value  %s             to set a register value in a device using register number \n", ColorRed, ColorReset);
  printf_safe("%sINCREMENT <device-name> <device-number> <group-name> <new-value> <increment>  %s to set a series of registers, increment can be 0 \n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sSHOW DEVICE <device-name> <device-number>  %s            to show the information for a device\n", ColorRed, ColorReset);
  printf_safe("%sSHOW GROUP <device-name> <device-number> <group-name> %s to show the information for a group\n", ColorRed, ColorReset);
  printf_safe("%sSHOW ALL  %s                                             to show all registers for all devices\n", ColorRed, ColorReset);
  printf_safe("%sSHOW MESSAGES  %s                                        to show all messages for all devices\n", ColorRed, ColorReset);
  printf_safe("%sSHOW BAY <bay-index>	%s				  to show the information for a Bay\n", ColorRed, ColorReset);
  printf_safe("%sSHOW CAN%s                                               to show the CAN Interface status\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sOUT <filespec>  %s to monitor CAN traffic to an output file \n", ColorRed, ColorReset);
  printf_safe("%sWATCH  %s          to watch for REGISTER ADDRESS PROTOCOL or OFF in incoming/outgoing CAN messages\n", ColorRed, ColorReset);
  printf_safe("%sPING  %s           to send message to SMDUEwa\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%s@<filename>  %s    to execute a script file\n", ColorRed, ColorReset);
  printf_safe("%sDELAY <seconds> %s to delay for a number of seconds \n", ColorRed, ColorReset);
  printf_safe("%sPAUSE <string>  %s to wait for user input \n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sBAYS            						%s List Bays\n", ColorRed, ColorReset);
  printf_safe("%sPANELS          						%s List Panels\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sCLOSE <index>   %s Close a WWW connection\n", ColorRed, ColorReset);
  printf_safe("%sCONNECTIONS     %s List WWW Connections\n", ColorRed, ColorReset);
  printf_safe("\n");
  printf_safe("%sOPTION SET {MONITORWEB} {ON|OFF} %s Monitor WEB Input requests\n", ColorRed, ColorReset);
  printf_safe("%s           {MONITORCAN} {ON|OFF} %s Monitor CAN Input requests\n", ColorRed, ColorReset);
  printf_safe("%sSTATUS%s\n", ColorRed, ColorReset);
  printf_safe("%sQUIT%s             to exit\n\n\n", ColorRed, ColorReset);
        return true;
}

bool HandleHelp2()
{
        printf_safe("\n\nCommands: \n\n");
        printf_safe("%sADD DEVICE <device-name> <quantity> %s to add a device to the simulator\n", ColorRed, ColorWhite);
        printf_safe("%sLIST DEVICES                        %s to list devices\n", ColorRed, ColorWhite);
        printf_safe("\n");
        printf_safe("%sSET  %s                                                                          to get prompted for the input to set a register value in a device \n", ColorRed, ColorWhite);
        printf_safe("%sSET <device-name> <device-number> <group-name> <abbreviation> <new-value> %s     to set a register value in a device \n", ColorRed, ColorWhite);
        printf_safe("%sSETN <device-name> <device-number> <register-number> <new-value>  %s             to set a register value in a device using register number \n", ColorRed, ColorWhite);
        printf_safe("%sINCREMENT <device-name> <device-number> <group-name> <new-value> <increment>  %s to set a series of registers, increment can be 0 \n", ColorRed, ColorWhite);
        printf_safe("\n");
        printf_safe("%sSHOW DEVICE <device-name> <device-number>  %s            to show the information for a device\n", ColorRed, ColorWhite);
        printf_safe("%sSHOW GROUP <device-name> <device-number> <group-name> %s to show the information for a group\n", ColorRed, ColorWhite);
        printf_safe("%sSHOW ALL  %s                                             to show all registers for all devices\n", ColorRed, ColorWhite);
        printf_safe("%sSHOW MESSAGES  %s                                        to show all messages for all devices\n", ColorRed, ColorWhite);
        printf_safe("\n");
        printf_safe("%sOUT <filespec>  %s to monitor CAN traffic to an output file \n", ColorRed, ColorWhite);
        printf_safe("%sMONITOR  %s        to turn on CAN monitoring \n", ColorRed, ColorWhite);
        printf_safe("%sWATCH  %s          to watch for REGISTER ADDRESS PROTOCOL or OFF in incoming/outgoing CAN messages\n", ColorRed, ColorWhite);
        printf_safe("%sPING  %s           to send message to SMDUEwa\n", ColorRed, ColorWhite);
        printf_safe("\n");
        printf_safe("%s@<filename>  %s    to execute a script file\n", ColorRed, ColorWhite);
        printf_safe("%sDELAY <seconds> %s to delay for a number of seconds \n", ColorRed, ColorWhite);
        printf_safe("%sPAUSE <string>  %s to wait for user input \n", ColorRed, ColorWhite);
        printf_safe("\n");
        printf_safe("%sQUIT%s             to exit\n\n\n", ColorRed, ColorWhite);
        return true;
}

//
// Handle the command to set the output file spec
//

bool HandleFile(char* Command, char Params[NUMPARAMS][COMMANDSIZE])
{

        size_t n;
        char FileName[COMMANDSIZE];
        char* str;
        // the incoming command has an '@' at the beginning and '\n' at the end.
        str = Command+1;
        n = strlen(str) - 1;
        strncpy(FileName, str, n);
        FileName[n] = 0x00;
        return ReadScriptFile(FileName);
}

/*******************************************************************************
 * Function : ReadScriptFile
 *******************************************************************************/
bool
ReadScriptFile
(
 char*                          InFilename
)
{
        FILE*                   fp;
        ssize_t                 read;
        size_t                  len = 0;
        char*                   line;

        fp = fopen(InFilename, "r");
        if (fp == NULL) { 
          printf_safe("%s - file not found \n", InFilename); 
          return true; 
        }

        while ((read = getline(&line, &len, fp)) != -1) {
                if (len > 0) { 
                  bool Result = HandleUserCommand(line); 
                  if (!Result) return true; 
                }
        }

        fclose(fp);
        if (line)
                free(line);
        return true;
}


//
// The main routine for decoding the user command and calling the appropriate routine
//

bool HandleUserCommand(char* Command)
{
        //printf_safe("Executing: %s \n", Command);
        char InBuf[COMMANDSIZE]; char* InBuffer = &InBuf[0];
        strcpy(InBuffer, Command);
        char* CommandWord;
        CommandWord = strtok(InBuffer, delim);
        bool Result = true;
        if (CommandWord == NULL) { return true; }
        else if (CommandWord[0] == '\n') { return true; }


        char ParamBuf[COMMANDSIZE]; strcpy(ParamBuf, Command);
        char* Param = &ParamBuf[0];
        strtok(ParamBuf, delim);        // skip first word, it is the command
        // the rest of command string are the parameters
        char Params[NUMPARAMS][COMMANDSIZE];
        int Num = 0;
        for (int i = 0; i < NUMPARAMS; i++) strcpy(Params[i], "");
        while ((Param = strtok(NULL, delim)) != NULL)  { strcpy(Params[Num], Param); Num++; if (Num > NUMPARAMS) break; }

        if (strcasecmp(CommandWord, "ADD") == 0)                { Result = HandleAdd(Command, Params); }
        else if (strcasecmp(CommandWord, "REMOVE") == 0)        { Result = HandleRemoveCommand(Command, Params); }
        else if (strcasecmp(CommandWord, "DISABLE") == 0)       { Result = HandleDisable(Command, Params); }
        else if (strcasecmp(CommandWord, "ENABLE") == 0)        { Result = HandleEnable(Command, Params); }
        else if (strcasecmp(CommandWord, "SET") == 0)           { Result = HandleSet(Command, Params); }
        else if (strcasecmp(CommandWord, "SETN") == 0)          { Result = HandleSetN(Command, Params); }
        else if (strcasecmp(CommandWord, "INCREMENT") == 0)     { Result = HandleIncrement(Command, Params); }
        else if (strcasecmp(CommandWord, "SHOW") == 0)          { Result = HandleShow(Command, Params); }
        else if (strcasecmp(CommandWord, "OUT") == 0)           { Result = HandleOutput(Command, Params); }
        else if (strcasecmp(CommandWord, "MONITOR") == 0)       { Result = HandleMonitor(Command, Params); }
        else if (strcasecmp(CommandWord, "WATCH") == 0)         { Result = HandleWatch(Command, Params); }
        else if (strcasecmp(CommandWord, "PING") == 0)          { Result = HandlePing(Command, Params); }
        else if (CommandWord[0] == '@')                                         { Result = HandleFile(Command, Params); }
        else if (strcasecmp(CommandWord, "DELAY") == 0)         { Result = HandleDelay(Command, Params); }
        else if (strcasecmp(CommandWord, "PAUSE") == 0)         { Result = HandlePause(Command, Params); }
        else if (strcasecmp(CommandWord, "HELP") == 0)          { Result = HandleHelp(Command, Params); }
        else if (strcasecmp(CommandWord, "LIST") == 0)          { Result = HandleList(Command, Params); }
        else if (strcasecmp(CommandWord, "CLOSE") == 0)         { Result = HandleCloseCommand(Command, Params); }
        else if (strcasecmp(CommandWord, "CONNECTIONS") == 0)   { Result = HandleConnectionsCommand(Command, Params); }
        else if (strcasecmp(CommandWord, "CLEAR") == 0)         { Result = HandleClearCommand(Command, Params); }
        else if (strcasecmp(CommandWord, "MEM") == 0 )          { Result = HandleMem(Command, Params); }
        else if (strcasecmp(CommandWord, "STATUS") == 0)        { Result = HandleStatus(); }  
        else if (strcasecmp(CommandWord, "QUIT") == 0)          { Result = HandleQuit(Command, Params); }
	else if (strcasecmp(CommandWord, "BAYS") == 0)          { Result = HandleBaysCommand(Command, Params); }
	else if (strcasecmp(CommandWord, "PANELS") == 0)        { Result = HandlePanelsCommand(Command, Params); }
	else if (strcasecmp(CommandWord, "FUSEBREAKERS") == 0)  { Result = HandleFuseBreakersCommand(Command, Params); }
        else if (strcasecmp(CommandWord, "OPTION") == 0)        { Result = HandleOptionCommand(Command, Params); }
        else if (CommandWord[0] == '#')                                         { return true; }
        else if (CommandWord[0] == '!')                                         { return true; }
        else { HandleError(); }
        if (!Result) HandleError();
        return true;
}

/*****************************************************************************!
 * Function : HandleOptionCommand
 *****************************************************************************/

/*****************************************************************************!
 * Function : HandleDelay
 *****************************************************************************/
bool
HandleDelay
(char* InCommand, char InParams[NUMPARAMS][COMMANDSIZE])
{
  if ( InParams[0][0] == 0x00 ) {
    return true; 
  }
  sleep(atoi(InParams[0]));
  return true;
}
    
/*****************************************************************************!
 * Function : HandleError
 *****************************************************************************/
void
HandleError()
{
  printf_safe("Command not recognized.  Use HELP to get a list of commands. \n\n");
}

/*****************************************************************************!
 * Function : HandlePause
 *****************************************************************************/
bool
HandlePause
(char InCommand[COMMANDSIZE], char InParams[NUMPARAMS][COMMANDSIZE])
{
  string			s, s2;
  int				i;

  for (i = 0; InParams[i][0]; i++) {
    if ( s ) {
      s = StringConcatTo(s, " ");
      s = StringConcatTo(s, InParams[i]);
    } else {
      s = StringCopy(InParams[i]);
    }
  }
  s = StringConcatTo(s, " [Press enter to continue] ");
  s2 = linenoise(s);
  free(s2);
  FreeMemory(s);
  return true;
}

//
// Do a forever loop, prompting for user input
//

bool
HandleMem
(
 char*                          Command,
 char                           Params[NUMPARAMS][128]
)
{
  (void)Command;
  (void)Params;
  MMMemoryStats                         stats;
  GetMemoryStats(&stats);
  printf_safe("Memory Allocated : %d\n", stats.BytesAllocated);
  printf_safe("Memory Freed     : %d\n", stats.BytesFreed);
  return true;
}

/*****************************************************************************!
 * Function : HandleUserInput
 *****************************************************************************/
void* HandleUserInput()
{
  char *buffer;
 
  char				prompt[64];
  sprintf(prompt, "Enter Command : " );
  while (true) {
    buffer = linenoise(prompt);
    if ( errno == EAGAIN ) {
      MainMonitorWebRequest = false;
      CANMonitorInput = false;
    }

    if ( NULL == buffer ) {
      continue;
    }
    bool Result = HandleUserCommand(buffer);
    linenoiseHistoryAdd(buffer);
    free(buffer);
    if (!Result) return 0;
  }
  return 0;
}

/*****************************************************************************!
 * Function : HandleListAllRegisters
 *****************************************************************************/
void
HandleListAllRegisters
()
{

}

/*****************************************************************************!
 * Function : NewFunction
 *****************************************************************************/
int
DeviceRegDefCompareByGroupName
(const void* InP1, const void* InP2)
{
  DeviceRegDef*				d1;
  DeviceRegDef*				d2;
  DeviceRegDef**			dp1;
  DeviceRegDef**			dp2;
  int					n;

  dp1 = (DeviceRegDef**)InP1;
  dp2 = (DeviceRegDef**)InP2;

  d1 = *dp1;
  d2 = *dp2;
  n = strcmp(d1->group, d2->group);
  if ( n == 0 ) {
    return strcmp(d1->name, d2->name);
  }
  return n; 
}

/*****************************************************************************!
 * Function : HandleListRegisters
 *****************************************************************************/
void
HandleListRegisters
(char* InName)
{
  DeviceDef*				deviceDef;
  DeviceRegDef*				regDef;
  int					i, defCount;
  DeviceRegDef**			defs;
  
  deviceDef = FindDeviceDef(mainDeviceDefs, InName);
  if ( NULL == deviceDef ) {
    printf_safe("Device %s not found\n", InName);
    return;
  }
  
  printf_safe("Device : %s\n", deviceDef->name); 
  printf_safe("\n");
  printf_safe("Value Type  Initial Value  %*s  %*s  %*s  %*s\n",
              SHORTSTRINGLEN, "Short Name", SHORTSTRINGLEN, "Name", SHORTSTRINGLEN, "Display Label", SHORTSTRINGLEN, "Group");
  printf_safe(" ----------------------------------------------------------------------------------------------------------------\n");

  i = 0; 
  for ( regDef = deviceDef->regDefs->definitions; regDef; regDef = regDef->next ) {
    i++;
  } 
  defCount = i;
  defs = (DeviceRegDef**)GetMemory(i * sizeof(DeviceRegDef*));
  i = 0;
  for ( regDef = deviceDef->regDefs->definitions; regDef; regDef = regDef->next ) {
    defs[i] = regDef;
    i++;
  }

  qsort(defs, defCount, sizeof(DeviceRegDef*), DeviceRegDefCompareByGroupName);

  for ( i = 0 ; i < defCount ; i++ ) {
    regDef = defs[i];
    printf_safe(" %3d (%03x)", regDef->valueType, regDef->valueType);
    printf_safe("       %08x", regDef->initialValue.data32);
    printf_safe("  %*s", SHORTSTRINGLEN, regDef->shortName);
    printf_safe("  %*s", SHORTSTRINGLEN, regDef->name);
    printf_safe("  %*s", SHORTSTRINGLEN, regDef->displayLabel);
    printf_safe("  %*s\n", SHORTSTRINGLEN, regDef->group); 
  }
  FreeMemory(defs);
}

/*****************************************************************************!
 * Function : HandleListMessages 
 *****************************************************************************/
void
HandleListMessages
(char* InName, char* InMessageTypeString)
{
  DeviceDef*				deviceDef;
  DeviceMessageDef*			messageDef;
  int					i, n;
  int					messageType;
  bool					b;
  bool					all;

  if ( StringEqualNoCase(InMessageTypeString, "all")  ) {
    all = true;
    messageType = -1;
  } else if ( StringEqualNoCase(InMessageTypeString, "message") ) {
    all = false;
    messageType = -1;
  } else {
    messageType = -1;
    all = true;
    if ( InMessageTypeString && InMessageTypeString[0] ) {
      n = GetIntValueFromString(&b, InMessageTypeString); 
      if ( b ) {
        messageType = n;
      }
    }
  }
  deviceDef = FindDeviceDef(mainDeviceDefs, InName);
  if ( NULL == deviceDef ) {
    printf_safe("Device %s not found\n", InName);
    return;
  }
  printf_safe("Device : %s\n", deviceDef->name);
  for ( messageDef = deviceDef->messageDefs->definitions; messageDef; messageDef = messageDef->next ) {
    if ( messageType == -1 || messageType == messageDef->msgType ) {
      printf_safe("  %3d (%03x) : %s\n", messageDef->msgType, messageDef->msgType, messageDef->messageName);
      if ( all ) {
        for (i = 0; i < messageDef->responsesCount; i++ ) {
          printf("    %3d (%03x) : %s\n", messageDef->responses[i].responseType,
	         messageDef->responses[i].responseType,
                 messageDef->responses[i].responseDef->displayLabel);
        }
      }
    }
  }
}

/*****************************************************************************!
 * Function : HandleClearCommand
 *****************************************************************************/
bool
HandleClearCommand
(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE])
{
  if ( strlen(Params[0]) == 0 ) {
    HandleClearDevices();
    return true;
  }
  HandleClearDevice(Params);

  return true;
}

/*****************************************************************************!
 * Function : HandleClearDevices
 *****************************************************************************/
void
HandleClearDevices
()
{
  
}

/*****************************************************************************!
 * Function : HandleClearDevice
 *****************************************************************************/
void
HandleClearDevice
(char Params[NUMPARAMS][COMMANDSIZE])
{
  string					deviceName;
  int						deviceID;

  if ( strlen(Params[1]) == 0 ) {
    return;
  }
  deviceName = Params[0];
  deviceID = atoi(Params[1]);

  CanDevice*                   		device;

  device = FindDevice(deviceName, deviceID);
  if ( NULL == device ) {
    printf_safe("Device %s %d could not by found\n", deviceName, deviceID);
    return;
  }
  ClearDeviceRegisters(device);
}


/*****************************************************************************!
 * Function : HandleConnectionsCommand
 *****************************************************************************/
bool
HandleConnectionsCommand
(char InCommand[COMMANDSIZE], char InParams[NUMPARAMS][COMMANDSIZE])
{
  printf_safe("--------------------------------------------------------------------------------\n");
  printf_safe("Web Socket Connections\n");
  WebConnectionListDisplay(mainWebConnections);
  printf_safe("\n");
  return true;
}

/*****************************************************************************!
 * Function : HandleCloseCommand
 *****************************************************************************/
bool
HandleCloseCommand
(char InCommand[COMMANDSIZE], char InParams[NUMPARAMS][COMMANDSIZE])
{
  int 					connectionsCount;
  char					s[64];
  string				s2;
  WebConnection*			connection;
  int					n;

  connectionsCount = WebConnectionListCount(mainWebConnections);
  if ( connectionsCount == 0 ) {
    printf_safe("No open connections\n");
    return true;
  }
  WebConnectionListDisplay(mainWebConnections);
  if ( connectionsCount == 1 ) {
    sprintf(s, "Choose connection [1] : ");
  } else {
    sprintf(s, "Choose connections [1 - %d] : ", connectionsCount);
  }
  s2 = linenoise(s);
  n = atoi(s2);
  free(s2);
  if ( n < 1 || n > connectionsCount ) {
    printf_safe("Invalid connection choice\n");
    return true;
  }
  connection = WebConnectionListGetByIndex(mainWebConnections, n-1);
  if ( NULL == connection ) {
    printf_safe("Could not find the selected connection\n");
    return true;
  }
  WebConnectionClose(connection);
  WebConnectionListRemove(mainWebConnections, connection);
  WebConnectionDestroy(connection);
  printf_safe("Web socket connection closed\n");
  printf_safe("\n");
  return true;
}

/*****************************************************************************!
 * Function : HandleAddFuseBreakerCommand
 *****************************************************************************/
bool
HandleAddFuseBreakerCommand
(char InParams[NUMPARAMS][COMMANDSIZE], string InType)
{
  return false;
}

/*****************************************************************************!
 * Function : HandleAddFuseCommand
 *****************************************************************************/
bool
HandleAddFuseCommand
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  HandleAddFuseBreakerCommand(InParams, "fuse");
  return true;
}

/*****************************************************************************!
 * Function : HandleAddBreakerCommand
 *****************************************************************************/
bool
HandleAddBreakerCommand
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  HandleAddFuseBreakerCommand(InParams, "breaker");
  return true;
}

/*****************************************************************************!
 * Function : HandleAddBayCommand
 *****************************************************************************/
bool
HandleAddBayCommand
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  Bay*					bay;
  char*					s;
  int					bayIndex;
  char					prompt[64];
  StringList*				bayTypeNames;
  int					i;
  int					bayTypeIndex;
  BayType*				bayType;

  // Get a new Bay Index 
  bayIndex = atoi(InParams[1]);

  // Validate the index and make sure the bay doesn't already exist
  if ( bayIndex < 1 || bayIndex > BAY_MAX_INDEX ) {
   printf_safe("%sthe bay index must be between 1 and %d%s\n", ColorRed, BAY_MAX_INDEX, ColorWhite);
   return true;
  }
  bay = FindBayByIndex(bayIndex);
  if ( bay != NULL ) {
    printf_safe("%sA Bay with an index of %d already exists%s\n",ColorRed, bayIndex, ColorWhite);
    return true;
  }

  // Display the available bay types and get one from the user
  bayTypeNames = BayTypeGetNamesList(MainBayTypes);
  for ( i = 0 ; i < bayTypeNames->stringCount; i++ ) {
    printf_safe("%s%d - %s%s\n", ColorYellow, i + 1, bayTypeNames->strings[i], ColorWhite);
  }   
  sprintf(prompt, "Enter an bay type [1 - %d] : ", bayTypeNames->stringCount);
  s = linenoise(prompt);

  bayTypeIndex = atoi(s);

  // Make sure the user entered a valid type index
  if ( bayTypeIndex < 1 || bayTypeIndex > bayTypeNames->stringCount ) {
    printf_safe("%s A bay type must be in the range of 1 - %d%s\n", ColorRed, bayTypeNames->stringCount, ColorWhite);
    StringListDestroy(bayTypeNames);
    return true;
  }

  bayType = BayTypeFindByName(MainBayTypes, bayTypeNames->strings[bayTypeIndex-1]);
  // This should NEVER EVER happen, but in case is does
  if ( bayType == NULL ) {
    StringListDestroy(bayTypeNames);
    printf("%sCould not find a bay type name %s%s\n", ColorRed, bayTypeNames->strings[bayTypeIndex], ColorWhite);
    return true;
  }
  StringListDestroy(bayTypeNames);

  // Create the bay
  bay = BayCreate(bayIndex, bayType);

  // Again this should never happen, but in case it does
  if ( NULL == bay ) {
    printf_safe("%sCould not create the bay%s\n", ColorRed, ColorWhite);
    return true;
  }

  // Save the Bays
  BaysSave(NULL);
  BaysSaveValues(NULL);
 
  WebSocketIFSendBay(bay); 
  // Confirmation message
  printf_safe("%sA %d panel bay with an index of %d was created%s\n", ColorGreen, bayType->panelCount, bayIndex, ColorWhite);
  return true;
}

/*****************************************************************************!
 * Function : HandleAddPanelCommand
 *****************************************************************************/
bool
HandleAddPanelCommand
(char InParams[NUMPARAMS][COMMANDSIZE])
{
  Bay*					bay;
  Panel*				panel;
  char*					s;
  int					bayIndex, panelIndex;
  char					prompt[64];
  StringList*				panelTypeNames;
  int					i;
  int					panelTypeIndex;
  PanelType*				panelType;

  // Get a new Bay Index 
  bayIndex = atoi(InParams[1]);
  panelIndex = atoi(InParams[2]);

  // Validate the index and make sure the bay doesn't already exist
  if ( bayIndex < 1 || bayIndex > BAY_MAX_INDEX ) {
   printf_safe("%sThe bay index must be between 1 and %d%s\n", ColorRed, BAY_MAX_INDEX, ColorWhite);
   return true;
  }
  bay = FindBayByIndex(bayIndex);
  if ( bay == NULL ) {
    printf_safe("%sA Bay with an index of %d does not exist%s\n",ColorRed, bayIndex, ColorWhite);
    return true;
  }

  panel = BayFindPanelByIndex(bay, panelIndex); 
  if ( panel && panel->panelType != NULL ) {
    printf_safe("%sA panel in Bay %d with an index of %d already exists%s\n",ColorRed, bayIndex, panelIndex, ColorWhite);
    return true;
  }

  if ( panel ) {
    BayRemovePanelByIndex(bay, panel->index);
  }
  // Display the available bay types and get one from the user
  panelTypeNames = PanelTypeGetNamesList(MainPanelTypes);
  for ( i = 0 ; i < panelTypeNames->stringCount; i++ ) {
    printf_safe("%s%d - %s%s\n", ColorYellow, i + 1, panelTypeNames->strings[i], ColorWhite);
  }   
  sprintf(prompt, "Enter an bay type [1 - %d] : ", panelTypeNames->stringCount);
  s = linenoise(prompt);

  panelTypeIndex = atoi(s);

  // Make sure the user entered a valid type index
  if ( panelTypeIndex < 1 || panelTypeIndex > panelTypeNames->stringCount ) {
    printf_safe("%s A panel type must be in the range of 1 - %d%s\n", ColorRed, panelTypeNames->stringCount, ColorWhite);
    StringListDestroy(panelTypeNames);
    return true;
  }

  panelType = PanelTypeFindByName(MainPanelTypes, panelTypeNames->strings[panelTypeIndex-1]);
  // This should NEVER EVER happen, but in case is does
  if ( panelType == NULL ) {
    StringListDestroy(panelTypeNames);
    printf("%sCould not find a panel type name %s%s\n", ColorRed, panelTypeNames->strings[panelTypeIndex], ColorWhite);
    return true;
  }
  StringListDestroy(panelTypeNames);

  // Create the bay
  panel = PanelCreate(panelIndex, panelType);

  // Again this should never happen, but in case it does
  if ( NULL == panel ) {
    printf_safe("%sCould not create the panel %s\n", ColorRed, ColorWhite);
    return true;
  }

  // Add the panel to the bay
  BayAddPanel(bay, panel);

  // Save the Bays
  BaysSave(NULL);
  BaysSaveValues(NULL);

  WebSocketIFSendPanel(panel); 
  // Confirmation message
  printf_safe("%sA %s panel(%d) in bay %d was created%s\n", ColorGreen, panelType->name, panelIndex, bayIndex, ColorWhite);
  return true;
}

/*****************************************************************************!
 * Function : HandleOptionCommand
 *****************************************************************************/
bool
HandleOptionCommand
(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE])
{
  string				subcommand;
  subcommand = Params[0];
 
  if ( StringEqualNoCase(subcommand, "SET") ) {
    return HandleOptionSetCommand(InCommand, Params);
  }  
  return false;
} 

/*****************************************************************************!
 * Function : HandleOptionSetCommand
 *****************************************************************************/
bool
HandleOptionSetCommand
(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE])
{
  string				subcommand;
  
  subcommand = Params[1];
  if ( StringEqualNoCase(subcommand, "MONITORWEB") ) {
    return HandleOptionSetMonitorWebCommand(InCommand, Params);
  }
  if ( StringEqualNoCase(subcommand, "MONITORCAN" ) ) {
    return HandleOptionSetMonitorCANCommand(InCommand, Params);
  }
  return false;
}

/*****************************************************************************!
 * Function : HandleOptionSetMonitorWebCommand
 *****************************************************************************/
bool
HandleOptionSetMonitorWebCommand
(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE])
{
  string				subcommand;
  
  subcommand = Params[2];
  if ( StringEqualNoCase(subcommand, "ON") ) {
    MainMonitorWebRequest = true;
    return true;
  } else if ( StringEqualNoCase(subcommand, "OFF") ) {
    MainMonitorWebRequest = false;
    return true;
  }
  return false;
}

/*****************************************************************************!
 * Function : HandleOptionSetMonitorCANCommand
 *****************************************************************************/
bool
HandleOptionSetMonitorCANCommand
(char* InCommand, char Params[NUMPARAMS][COMMANDSIZE])
{
  string				subcommand;
  
  subcommand = Params[2];
  if ( StringEqualNoCase(subcommand, "ON") ) {
    CANMonitorInput = true;
    return true;
  } else if ( StringEqualNoCase(subcommand, "OFF") ) {
    CANMonitorInput = false;
    return true;
  }
  return false;
}

