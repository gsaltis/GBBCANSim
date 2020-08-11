/*****************************************************************************
 * FILE NAME    : bundle.c
 * DATE         : June 22 2020
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <dirent.h>
#include <errno.h>

/*****************************************************************************!
 * Local Headers
 *****************************************************************************/
#include "../String.h"
#include "../FileUtils.h"
#include "../MemoryManager.h"
#include "../ANSIColors.h"

/*****************************************************************************!
 * Local Macros
 *****************************************************************************/
#define DEFAULT_FILE_NAME		"scripts.js"
#define DEFAULT_DIR_NAME		"./"
#define DIRECTORY_SEPARATOR_CHAR	'/'
#define DIRECTORY_SEPARATOR_STRING	"/"
#define DEFAULT_JAVASCRIPT_SUFFIX	".js"

/*****************************************************************************!
 * Local Functions 
 *****************************************************************************/
void
Initialize
();

void
ParseCommandLine
(int argc, char** argv);

void
VerifyCommandLine
();

void
ReadDirectoryFileNames
(string InBaseDirectoryName);

void
BundleFiles
();

void
DisplayHelp
();

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
FILE*
MainFile;

string
MainFileName;

string
MainDirName;

StringList*
MainFileNames;

/*****************************************************************************!
 * Function : main
 *****************************************************************************/
int
main
(int argc, char** argv)
{
  Initialize();
  ParseCommandLine(argc, argv);
  VerifyCommandLine();
  ReadDirectoryFileNames(MainDirName);
  BundleFiles();

  exit(EXIT_SUCCESS);
}

/*****************************************************************************!
 * Function : Initialize
 *****************************************************************************/
void
Initialize
()
{
  MainFileNames = StringListCreate();
  MainFile = NULL;
  MainFileName = StringCopy(DEFAULT_FILE_NAME);
  MainDirName = StringCopy(DEFAULT_DIR_NAME);
}

/*****************************************************************************!
 * Function : ParseCommandLine
 *****************************************************************************/
void
ParseCommandLine
(int argc, char** argv)
{
  int					i;
  string				command;

  for (i = 1; i < argc; i++) {
    command = argv[i];
    if ( StringEqualsOneOf(command, "-h", "--help", NULL) ) {
      DisplayHelp();
      exit(EXIT_SUCCESS);
    }
    if ( StringEqualsOneOf(command, "-f", "--file", NULL) ) {
      i++;
      if ( i == argc ) {
	fprintf(stderr, "%s requires a filename\n", command);
	DisplayHelp();
	exit(EXIT_FAILURE);
      }
      if ( MainFileName ) {
        FreeMemory(MainFileName);
      }
      MainFileName = StringCopy(argv[i]);
      continue;
    }
    if ( StringEqualsOneOf(command, "-d", "--dir", NULL) ) {
      i++;
      if ( i == argc ) {
	fprintf(stderr, "%s requires a directory name\n", command);
	DisplayHelp();
	exit(EXIT_FAILURE);
      }
      if ( MainDirName ) {
	FreeMemory(MainDirName);
      }
      MainDirName = StringCopy(argv[i]);
      continue;
    }
    fprintf(stderr, "%s is not a valid command\n", command);
    DisplayHelp();
    exit(EXIT_FAILURE);	    
  }
}

/*****************************************************************************!
 * Function : VerifyCommandLine
 *****************************************************************************/
void
VerifyCommandLine
()
{
  int					n;

  if ( NULL == MainFileName ) {
    fprintf(stderr, "Missing filename\n");
    DisplayHelp();
    exit(EXIT_FAILURE);
  }

  if ( NULL == MainDirName ) {
    fprintf(stderr, "Missing directory name\n");
    DisplayHelp();
    exit(EXIT_FAILURE);
  }

  if ( !FileExists(MainDirName) ) {
    fprintf(stderr, "The directory %s does not exist\n", MainDirName);
    exit(EXIT_FAILURE);
  } 

  n = strlen(MainDirName);
  n--;
  if ( MainDirName[n] != DIRECTORY_SEPARATOR_CHAR ) {
    MainDirName = StringConcatTo(MainDirName, DIRECTORY_SEPARATOR_STRING);
  }
}

/*****************************************************************************!
 * Function : ReadDirectoryFileNames
 *****************************************************************************/
void
ReadDirectoryFileNames
(string InBaseDirectoryName)
{
  DIR*					d;
  struct dirent*			de;
  string				td;
  int					p, m, n;
  string				s;

  p = strlen(DEFAULT_JAVASCRIPT_SUFFIX);
  d = opendir(InBaseDirectoryName);
  if ( d == NULL ) {
    fprintf(stderr, "Could not open directory %s : %s\n", MainDirName, strerror(errno));
    exit(EXIT_FAILURE);
  }

  for ( de = readdir(d); de; de = readdir(d) ) {
    // Skip the directory entries
    if ( StringEqualsOneOf(de->d_name, ".", "..", NULL) ) {
      continue;
    }
    // Don't include->the file we are creating if a previous version exists
    // Of course, this exclude-> any file named the same as MainFileName in 
    //   any subdirectory.  For now so be it.
    if ( StringEqual(de->d_name, MainFileName) ) {
      continue;
    }
    if ( de->d_type & DT_DIR ) {
      // We have a subdirectory, recurse into and collect files
      td = StringMultiConcat(InBaseDirectoryName, de->d_name, "/", NULL);
      ReadDirectoryFileNames(td);
      FreeMemory(td);
      continue;
    }
    n = strlen(de->d_name);
    if ( n < 3 ) {
      continue;
    }
    m = n - p;
    s = de->d_name + m;
    if ( !StringEqual(s, DEFAULT_JAVASCRIPT_SUFFIX) ) {
      continue;
    }
    StringListAppend(MainFileNames, StringConcat(InBaseDirectoryName, de->d_name));    
  }
  closedir(d);  
}

/*****************************************************************************!
 * Function : BundleFiles 
 *****************************************************************************/
void
BundleFiles
()
{
  int					i;
  FILE*					file;
  string				buffer;
  int					bufferSize;

  file = fopen(MainFileName, "wb");
  if ( NULL == file ) {
    fprintf(stderr, "Could not open file %s : %s\n", MainFileName, strerror(errno));
    exit(EXIT_FAILURE);
  }
  
   
  for (i = 0; i < MainFileNames->stringCount; i++) {
    GetFileBuffer(MainFileNames->strings[i], &buffer, &bufferSize);
    fprintf(file, "// FILE: %s\n", MainFileNames->strings[i]), 
    fprintf(file, buffer);
    FreeMemory(buffer);
  }

  fclose(file);
}

/*****************************************************************************!
 * Function : DisplayHelp
 *****************************************************************************/
void
DisplayHelp
()
{

}

