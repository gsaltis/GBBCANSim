/*******************************************************************************
 * FILE         : main.c
 * DATE         : April 26 2018
 * COPYRIGHT    : Copyright (c) 2018 Vertv Company
 *******************************************************************************/

/*******************************************************************************!
 * Global Headers
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <mongoose.h>
#include <pthread.h>

/*******************************************************************************!
 * Local Headers
 *******************************************************************************/
#include "jsoncanif.h"
#include "AllCanDefinitions.h"
#include "Devices.h"
#include "ThreadSafePrint.h"
#include "UserInputHandling.h"
#include "DirManagement.h"
#include "DeviceDef.h"
#include "HTTPServerThread.h"
#include "WebSocketIF.h"
#include "Bay.h"
#include "FuseBreakerType.h"
#include "FileUtils.h"
#include "ascii.h"
#include "PanelType.h"
#include "BayType.h"
#include "WebConnection.h"
#include "ANSIColors.h"
#include "MemoryManager.h"
#include "ClientUserInput.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/
#define PROGRAM_NAME				"cansimws"
#define MAIN_DATABASE_NAME			"GBBCANSim.db"
#define VERSION_MAJOR				1
#define VERSION_MINOR				0
/*******************************************************************************!
 * Local Data
 *******************************************************************************/
sig_atomic_t s_signal_received = 0;

char mainDeviceDefsFileName[FILENAME_MAX+1];

DeviceDefList* 
mainDeviceDefs;

char mainDeviceDataFileName[FILENAME_MAX+1];

bool MainVerbose = true;

struct mg_serve_http_opts
s_http_server_opts;

FuseBreakerType*
MainFuseBreakerTypes = NULL;

PanelType*
MainPanelTypes = NULL;

BayType*
MainBayTypes = NULL;

WebConnectionList*
mainWebConnections;

bool
MainMonitorWebRequest;

bool
MainSupportRectifiers = false;

string
MainDefaultHTTPPort = "8000";

string
MainDefaultWebSocketPort = "8001";

string
MainDefaultWebDirectory = NULL;

string
MainDeviceDefsFilenameDefault = "DeviceDefs.json";

string
MainDeviceDefsFilename = NULL;

string
MainBaysFilename = NULL;

string
MainBaysFilenameDefault = "Bays.json";

/*******************************************************************************
 * Local Functions
 *******************************************************************************/
void
FuseBreakerTypesRead
();

void
SystemInitialize();

void
PanelTypesRead();

void
BayTypesRead();

void
DeviceDefsRead
();

void
MainInitialize
();

void
MainCommandLineProcess
(int argc, char** argv);

void
MainDisplayHelp
();

/*******************************************************************************!
 * Function : signal_handler
 *******************************************************************************/
static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);  // Reinstantiate signal handler
  s_signal_received = sig_num;
}

/*******************************************************************************!
 * Function : main
 *******************************************************************************/
int 
main
(int argc, char** argv) {
  string				baysFilename;

  MainInitialize();
  MainCommandLineProcess(argc, argv);

  BaySaveFilename(MainBaysFilename);
  if ( s_http_port == NULL || *s_http_port == 0x00 ) {
    printf("HTTP Server port cannot be empty\n");
    MainDisplayHelp();
    exit(EXIT_FAILURE);
  }

  if ( s_websocket_port == NULL || *s_websocket_port == 0x00 ) {
    printf("Web Socket port cannot be empty\n");
    MainDisplayHelp();
    exit(EXIT_FAILURE);
  }
  if ( StringEqual(s_http_port, s_websocket_port) ) {
    printf("HTTP Server port and Web Socket port cannot be the same\n");
    exit(EXIT_FAILURE);
  }
  CanDefinitionsInitialize();
  InitSafePrint();

  WebSocketIFCreateInfoScript();  
  SystemInitialize();
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);
  setvbuf(stdout, NULL, _IOLBF, 0);
  setvbuf(stderr, NULL, _IOLBF, 0);

  // WebSocketInitialize();

  pthread_t tr, tw;
  s_http_server_opts.document_root = "www";
  s_http_server_opts.enable_directory_listing = "yes";

  DeviceDefsRead();
  baysFilename = MainBaysFilename;
  if ( !FileExists(baysFilename) ) {
    FileCreateEmptyFile(baysFilename);
  }
  BaysInitialize(baysFilename);
  HTTPServerThreadInit();
  WebSocketServerInit();
  printf("Web directory is %s\n", MainDefaultWebDirectory);
  pthread_create(&tr,  NULL, HandleUserInput, NULL);
  pthread_create(&tw,  NULL, HandleCanTraffic, NULL);

  pthread_join(tr, NULL);
  pthread_join(tw, NULL);
  pthread_join(HTTPServerThreadID, NULL);
  pthread_join(WebSocketServerThreadID, NULL);
  return EXIT_SUCCESS;
}

/*******************************************************************************!
 * Function : SystemInitialize
 *******************************************************************************/
void
SystemInitialize
()
{
    FILE*                                                       deviceDefsFile;
   
    mainWebConnections = WebConnectionListCreate();
 
    InitSafePrint();
    FuseBreakerTypesRead();
    PanelTypesRead();
    BayTypesRead();
    printf_safe("%s V%d.%d \n", PROGRAM_NAME, VERSION_MAJOR, VERSION_MINOR);
    GetDeviceDefsFileName(mainDeviceDefsFileName, FILENAME_MAX);

    mainDeviceDefs = NULL;
    deviceDefsFile = fopen(mainDeviceDefsFileName, "r");
    if ( deviceDefsFile == NULL ) {
        printf_safe("Could not open devices definitions file %s\n", mainDeviceDefsFileName);
        return;
    }
     DeviceDefsRead();DeviceDefsRead(); 
    if ( NULL == mainDeviceDefs ) {
        printf_safe("Could not read devices definitions file %s\n", mainDeviceDefsFileName);
        return;
    }
}

/*****************************************************************************!
 * Function : FuseBreakerTypesRead
 *****************************************************************************/
void
FuseBreakerTypesRead
()
{
  FuseBreakerType*                      fuseBreakerType;
  int                                   fuseBreakerSize;
  string                                fuseBreakers;
  json_value*                           obj;
  int                                   i;
  json_value*                           value;
  json_settings                         jsettings;
  char                                  error[1024];
  string                                filename = StringMultiConcat(MainDefaultWebDirectory, "/", "FuseBreakerTypes.json", NULL);

  memset(&jsettings, NUL, sizeof(json_settings));
  GetFileBuffer(filename, &fuseBreakers, &fuseBreakerSize);
  value = json_parse_ex(&jsettings, fuseBreakers, fuseBreakerSize, error);
  if ( NULL == value ) {
    fprintf(stderr, "JSON Error : %s : %s\n", filename, error);
    FreeMemory(filename);
    return;
  }
  if ( value->type != json_array ) {
    json_value_free(value);
  }

  for ( i = 0; i < value->u.array.length; i++ ) {
    obj = value->u.array.values[i];
    if ( obj->type != json_object ) {
      continue;
    }

    fuseBreakerType = FuseBreakerTypeFromJSONString(obj);
    if ( MainFuseBreakerTypes ) {
      FuseBreakerTypeAppend(MainFuseBreakerTypes, fuseBreakerType);
    } else {
      MainFuseBreakerTypes = fuseBreakerType;
    }
  }
  FreeMemory(filename);
}

/*****************************************************************************!
 * Function : PanelTypesRead
 *****************************************************************************/
void
PanelTypesRead
()
{

  PanelType*                            panelType;
  int                                   panelSize;
  string                                panels;
  json_value*                           obj;
  int                                   i;
  json_value*                           value;
  json_settings                         jsettings;
  char                                  error[1024];
  string                                filename = StringMultiConcat(MainDefaultWebDirectory, "/", "PanelTypes.json", NULL);

  memset(&jsettings, NUL, sizeof(json_settings));
  GetFileBuffer(filename, &panels, &panelSize);
  value = json_parse_ex(&jsettings, panels, panelSize, error);
  if ( NULL == value ) {
    fprintf(stderr, "JSON Error : %s : %s\n", filename, error);
    FreeMemory(filename);
    return;
  }
  if ( value->type != json_array ) {
    json_value_free(value);
  }

  for ( i = 0; i < value->u.array.length; i++ ) {
    obj = value->u.array.values[i];
    if ( obj->type != json_object ) {
      continue;
    }

    panelType = PanelTypeFromJSONString(obj);
    if ( MainPanelTypes ) {
      PanelTypeAppend(MainPanelTypes, panelType);
    } else {
      MainPanelTypes = panelType;
    }
  }
  FreeMemory(filename);
}

/*****************************************************************************!
 * Function : BayTypesRead
 *****************************************************************************/
void
BayTypesRead
()
{

  BayType*                              bayType;
  int                                   baySize;
  string                                bays;
  json_value*                           obj;
  int                                   i;
  json_value*                           value;
  json_settings                         jsettings;
  char                                  error[1024];
  string                                filename = StringMultiConcat(MainDefaultWebDirectory, "/", "BayTypes.json", NULL);

  memset(&jsettings, NUL, sizeof(json_settings));
  GetFileBuffer(filename, &bays, &baySize);
  value = json_parse_ex(&jsettings, bays, baySize, error);
  if ( NULL == value ) {
    fprintf(stderr, "JSON Error : %s : %s\n", filename, error);
    FreeMemory(filename);
    return;
  }
  if ( value->type != json_array ) {
    json_value_free(value);
  }

  for ( i = 0; i < value->u.array.length; i++ ) {
    obj = value->u.array.values[i];
    if ( obj->type != json_object ) {
      continue;
    }

    bayType = BayTypeFromJSONString(obj);
    if ( MainBayTypes ) {
      BayTypeAppend(MainBayTypes, bayType);
    } else {
      MainBayTypes = bayType;
    }
  }
  FreeMemory(filename);
}

/*****************************************************************************!
 * Function : DeviceDefsRead
 *****************************************************************************/
void
DeviceDefsRead
()
{
  string				buffer;
  int					bufferLen;
  json_value*				devicejson;
  json_settings                         jsettings;
  char                                  error[1024];
  DeviceDefList*			defs;

  defs = NULL; 
  memset(&jsettings, NUL, sizeof(json_settings));
  if ( GetFileBuffer(MainDeviceDefsFilename, &buffer, &bufferLen) ) {
    devicejson = json_parse_ex(&jsettings, buffer, bufferLen, error);
    if ( NULL == devicejson ) {
      printf("%s%s%s\n", ColorRed, error, ColorWhite);
      return;
    }
    defs = DeviceDefListFromJSON(devicejson);    
  }  
  mainDeviceDefs = defs;
}

/*****************************************************************************!
 * Function : MainInitialize
 *****************************************************************************/
void
MainInitialize
()
{
  MainDefaultWebDirectory = StringCopy("www");
  CanDeviceListInitialize();
  HTTPServerSetPort(MainDefaultHTTPPort);
  WebSocketIFSetPort(MainDefaultWebSocketPort);

  MainMonitorWebRequest 	= false;
  CANMonitorInput       	= false;
  MainDeviceDefsFilename 	= StringCopy(MainDeviceDefsFilenameDefault);
  MainBaysFilename			= StringCopy(MainBaysFilenameDefault);
}

/*****************************************************************************!
 * Function : MainCommandLineProcess
 *****************************************************************************/
void
MainCommandLineProcess
(int argc, char** argv)
{
  int						i;
  string					command;

  for (i = 1 ; i < argc; i++ ) {
    command = argv[i];

    if ( StringEqualsOneOf(command, "-mw", "--monitorweb", NULL) ) {
      MainMonitorWebRequest = true;
      continue;
    }
    if ( StringEqualsOneOf(command, "-h", "--help", NULL) ) {
      MainDisplayHelp();
      exit(EXIT_SUCCESS);
    }

    if ( StringEqualsOneOf(command, "-H", "--httpport", NULL) ) {
      i++;
      if ( i >= argc ) {
  	fprintf(stderr, "%s is missing port number\n", command);
        MainDisplayHelp();
 	exit(EXIT_FAILURE);
      }
      HTTPServerSetPort(argv[i]);
      continue;
    }

    if ( StringEqualsOneOf(command, "-W", "--websocketport", NULL) ) {
      i++;
      if ( i >= argc ) {
  	fprintf(stderr, "%s is missing port number\n", command);
        MainDisplayHelp();
 	exit(EXIT_FAILURE);
      }
      WebSocketIFSetPort(argv[i]);
      continue;
    }
    if ( StringEqualsOneOf(command, "-w", "--webdir", NULL) ) {
      i++;
      if ( i >= argc ) {
        fprintf(stderr, "%s is missing a www directory name\n", command);
        MainDisplayHelp();
        exit(EXIT_FAILURE);
      }
      if ( MainDefaultWebDirectory ) {
        FreeMemory(MainDefaultWebDirectory);
      }
      MainDefaultWebDirectory = StringCopy(argv[i]);
      continue;
    }

    if ( StringEqualsOneOf(command, "-r", "--rectifiers", NULL) ) {
      MainSupportRectifiers = true;
      continue;
    }

    if ( StringEqualsOneOf(command, "-mc", "--monitorcan", NULL) ) {
      CANMonitorInput = true;
      continue;
    }

	//! Handle Device Definitions
	if ( StringEqualsOneOf(command, "-d", "--devicedef", NULL) ) {
	  i++;
	  if ( i >= argc ) {
		fprintf(stderr, "%s is missing a devicedef file\n", command);
		MainDisplayHelp();
		exit(EXIT_FAILURE);
	  }
	  if ( MainDeviceDefsFilename ) {
		FreeMemory(MainDeviceDefsFilename);
	  }
	  MainDeviceDefsFilename = StringCopy(argv[i]);
	  continue;
	}

	//! Handle Bay Definitions
	if ( StringEqualsOneOf(command, "-b", "--baysfile", NULL) ) {
	  i++;
	  if ( i >= argc ) {
		fprintf(stderr, "%s is missing a bays file\n", command);
		MainDisplayHelp();
		exit(EXIT_FAILURE);
	  }
	  if ( MainBaysFilename ) {
		FreeMemory(MainBaysFilename);
	  }
	  MainBaysFilename = StringCopy(argv[i]);
	  continue;
	}

    if ( command[0] == '-' ) {
      fprintf(stderr, "%s is an invalid command\n", command);
      MainDisplayHelp();
      exit(EXIT_FAILURE);
    }
  }
}

/*****************************************************************************!
 * Function : MainDisplayHelp
 *****************************************************************************/
void
MainDisplayHelp
()
{
  int					n;

  n = printf("%s options\n", PROGRAM_NAME);
  printf("options\n");
  printf("%*s%s -b  --baysfile      %s: Specify the bays information file %s(default %s)%s\n", n, " ", ColorGreen, ColorBrightCyan, ColorYellow, MainBaysFilenameDefault, ColorReset);
  printf("%*s%s -d  --devicedef     %s: Specify the device definition file %s(default %s)%s\n", n, " ", ColorGreen, ColorBrightCyan, ColorYellow, MainDeviceDefsFilenameDefault, ColorReset);
  printf("%*s%s -h  --help          %s: Display this information%s\n", n, " ", ColorGreen, ColorBrightCyan, ColorReset);
  printf("%*s%s -H  --httpport      %s: Specify HTTP listening port %s(default %s)%s\n", n, " ", ColorGreen, ColorBrightCyan, ColorYellow, MainDefaultHTTPPort, ColorReset);
  printf("%*s%s -mc --monitorcan    %s: Monitor CAN requests%s\n", n, " ", ColorGreen, ColorBrightCyan, ColorReset);
  printf("%*s%s -mw --monitorweb    %s: Monitor web page requests%s\n", n, " ", ColorGreen, ColorBrightCyan, ColorReset);
  printf("%*s%s -w  --webdir        %s: Specify the base www directory %s(default %s)%s\n", n, " ", ColorGreen, ColorBrightCyan, ColorYellow, MainDefaultWebDirectory, ColorReset);
  printf("%*s%s -W  --websocketport %s: Specify Web Socket listening port %s(default %s)%s\n", n, " ", ColorGreen, ColorBrightCyan, ColorYellow, MainDefaultWebSocketPort, ColorReset);
}

