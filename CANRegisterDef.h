/*****************************************************************************
 * FILE NAME    : CANRegisterDef.h 
 * DATE         : June 25 2020
 * PROJECT      : BDFB
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/
#ifndef _canregisterdef_h_
#define _canregisterdef_h_

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
#include "NumericTypes.h"

/*******************************************************************************!
 * Exported Type : CANRegisterDef
 *******************************************************************************/
typedef const struct  {
    uint16_t 			valueType;
    ufloatbit32_t 		initialValue;
    uint16_t			messageType;
    string 			group;
    string 			abbrev;
    string 			name;
    string 			displayLabel;
} CANRegisterDef;

/*****************************************************************************!
 * Exported Functions
 *****************************************************************************/

#endif /* _canregisterdef_h_*/
