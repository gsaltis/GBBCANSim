/*******************************************************************************!
 * FILE         : NumericTypes.h
 * DATE         : September 24 2018
 * PROJECT      : CanSim
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/
#ifndef _numerictypes_h_
#define _numerictypes_h_

/*******************************************************************************!
 * Global Headers
 *******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************!
 * Exported Type : uint16bit_t
 *******************************************************************************/
typedef	union
{
    uint16_t                            id1;
    struct {
	unsigned                        highchar1 : 8;
	unsigned                        lowchar1  : 8;
    } bitdata1;
} uint16bit_t;

/*******************************************************************************!
 * Exported Type : ufloatbit32_t
 *******************************************************************************/
typedef	union
{
    float                               fd;
    uint16bit_t                         intdata[2];
    uint8_t				data8[4];
    uint32_t                            data32;
} ufloatbit32_t;

/*******************************************************************************!
 * Exported Functions
 *******************************************************************************/
int
GetIntValueFromString
(bool* InStatus, char* InString);

uint32_t
GetHex32ValueFromString
(bool* InStatus, char* InString);

float
GetFloatValueFromString
(bool* InStatus, char* InString);

#endif // _numerictypes_h_

