#ifndef _userinputhandling_h_
#define _userinputhandling_h_

/*******************************************************************************
 * Exported Macors
 *******************************************************************************/
#define NUMPARAMS 			6

void* HandleUserInput			();
bool HandleMem				(char*, char Params[NUMPARAMS][128]);
bool ReadScriptFile			(char* InFilename);

#endif // _userinputhandling_h_
