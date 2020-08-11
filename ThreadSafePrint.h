/*******************************************************************************
 * FILE 	: ThreadSafePrint.h
 *******************************************************************************/
#ifndef _threadsafeprint_h_
#define _threadsafeprint_h_

/*******************************************************************************
 * Global Headers
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdarg.h>

/******************************************************************************
 * Exported Functions
 ******************************************************************************/
void
InitSafePrint();

int
printf_safe(const char *format, ...);

#endif
