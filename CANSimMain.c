
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
#include "CanMsg.h"
#include "Devices.h"
#include "CANInterface.h"
#include "ThreadSafePrint.h"
#include "UserInputHandling.h"
#include "DirManagement.h"
#include "DeviceDef.h"

/*******************************************************************************!
 * Local Data
 *******************************************************************************/
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond;
char mainDeviceDefsFileName[FILENAME_MAX+1];
DeviceDefList* mainDeviceDefs;

/*******************************************************************************!
 * Local Functions
 *******************************************************************************/
void
SystemInitialize();
 
/*******************************************************************************!
 * Function : main
 *******************************************************************************/
int main()
{
	SystemInitialize();
	
	//AddDevice("BAY", 1);
	//AddDevice("PANEL", 8);
	 AddDevice("REC", 6);

	//SetRegisterValueFloat("SMDUH2", 3, 0x100, 42.42);
	//SetRegisterValueHex("SMDUH2", 3, 0x5A, 0x30313031);
	//DumpDevice("SMDUH2", 3);

	//AddDevice("REC12", 6);
	//AddDevice("BBU", 3);

	pthread_t tr, tw;

	pthread_create(&tr, NULL, HandleUserInput, NULL);
	pthread_create(&tw, NULL, HandleCanTraffic, NULL);

	pthread_join(tr, NULL);
	pthread_join(tw, NULL);

	return 1;

}  //main


/*******************************************************************************!
 * Function : SystemInitialize
 *******************************************************************************/
void
SystemInitialize
()
{
	FILE*							deviceDefsFile;
	
	InitSafePrint();
	printf_safe("CanSim V0.5 \n");
	GetDeviceDefsFileName(mainDeviceDefsFileName, FILENAME_MAX);

	mainDeviceDefs = NULL;
    deviceDefsFile = fopen(mainDeviceDefsFileName, "r");
    if ( deviceDefsFile == NULL ) {
    	printf_safe("Could not open devices definitions file %s\n", mainDeviceDefsFileName);
    	return;
    }
	mainDeviceDefs = ReadDeviceDefList(deviceDefsFile);
	if ( NULL == mainDeviceDefs ) {
		printf_safe("Could not read devices definitions file %s\n", mainDeviceDefsFileName);
		return;
	}
	
	DumpDeviceDefList(mainDeviceDefs);
}