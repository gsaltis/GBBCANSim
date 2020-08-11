/*****************************************************************************
 * FILE NAME    : smdup.c
 * DATE         : July 22 2020
 * COPYRIGHT    : Copyright (C) 2020 by 
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
struct 
{
  unsigned short	valueType;
  unsigned short	messageType;
} Messages[] = {
  { 0x0051, 0x41 },
  { 0x0052, 0x41 },
  { 0x0054, 0x41 },
  { 0x0055, 0x41 },
  { 0x0056, 0x41 },
  { 0x005A, 0x41 },
  { 0x005B, 0x41 },
  { 0x005C, 0x41 },
  { 0x005D, 0x41 },
  { 0x0000, 0x41 },
  { 0x0040, 0x42 },
  { 0x1000, 0x41 },
  { 0x0132, 0x41 },
  { 0x0100, 0x41 },
  { 0x0101, 0x41 },
  { 0x0102, 0x41 },
  { 0x0103, 0x41 },
  { 0x0104, 0x41 },
  { 0x0105, 0x41 },
  { 0x0106, 0x41 },
  { 0x0107, 0x41 },
  { 0x0108, 0x41 },
  { 0x0109, 0x41 },
  { 0x010A, 0x41 },
  { 0x010B, 0x41 },
  { 0x010C, 0x41 },
  { 0x010D, 0x41 },
  { 0x010E, 0x41 },
  { 0x010F, 0x41 },
  { 0x0110, 0x41 },
  { 0x0111, 0x41 },
  { 0x0112, 0x41 },
  { 0x0113, 0x41 },
  { 0x0114, 0x41 },
  { 0x0115, 0x41 },
  { 0x0116, 0x41 },
  { 0x0117, 0x41 },
  { 0x0118, 0x41 },
  { 0x0119, 0x41 },
  { 0x011A, 0x41 },
  { 0x011B, 0x41 },
  { 0x011C, 0x41 },
  { 0x011D, 0x41 },
  { 0x011E, 0x41 },
  { 0x011F, 0x41 },
  { 0x0120, 0x41 },
  { 0x0121, 0x41 },
  { 0x0122, 0x41 },
  { 0x0123, 0x41 },
  { 0x0124, 0x41 },
  { 0x0125, 0x41 },
  { 0x0126, 0x41 },
  { 0x0127, 0x41 },
  { 0x0128, 0x41 },
  { 0x0129, 0x41 },
  { 0x012A, 0x41 },
  { 0x012B, 0x41 },
  { 0x012C, 0x41 },
  { 0x012D, 0x41 },
  { 0x012E, 0x41 },
  { 0x012F, 0x41 },
  { 0x0130, 0x41 },
  { 0x0131, 0x41 }
};


struct {
  unsigned short			request;
  unsigned short			responses[16];
} Responses[] = {
  { 0x20, { 0x51, 0x54, 0x56, 0x5A, 0x5B, 0x5C, 0x5D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
  { 0x24, { 0x0100, 0x0101, 0x0102, 0x0103, 0x0104, 0x0105, 0x0106, 0x0107, 0x0108, 0x0109, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
  { 0x25, { 0x010A, 0x010B, 0x010C, 0x010D, 0x010E, 0x010F, 0x0110, 0x0111, 0x0112, 0x0113, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
  { 0x26, { 0x0114, 0x0115, 0x0116, 0x0117, 0x0118, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
  { 0x27, { 0x0119, 0x011A, 0x011B, 0x011C, 0x011D, 0X011E, 0x011F, 0x0120, 0x0121, 0x0122, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
  { 0x28, { 0x0123, 0x0124, 0x0125, 0x0126, 0x0127, 0x0128, 0x0129, 0x012A, 0x012B, 0x012C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
  { 0x29, { 0x012D, 0x012E, 0x012F, 0x0130, 0x0131, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }
};

/*****************************************************************************!
 * Function : main
 *****************************************************************************/
int
main
(int argc, char** argv)
{
  int					n, i, k, m;

  n = sizeof(Messages) / sizeof(Messages[0]);

  printf("  {\n");
  printf("    \"name\"                : \"SMDU\",\n");
  printf("    \"protocol\"            : 98,\n");
  printf("    \"name\"                : \"SMUD\",\n");
  printf("    \"description\"         : \"SMDU\",\n");
  printf("    \"startingcanaddress\"  : 134,\n");
  printf("    \"regs\"                :\n");
  printf("    [\n");
  for ( i = 0 ; i < n ; i++ ) {
    printf("      {\n");
    printf("        \"valuetype\"       : %d,\n", Messages[i].valueType);
    printf("        \"msgtype\"         : %d,\n", Messages[i].messageType);
    printf("        \"abbrev\"          : \"Command %03X\",\n", Messages[i].valueType);
    printf("        \"groupsort\"       : \"0General\",\n");
    printf("        \"group\"           : \"General\",\n");
    printf("        \"name\"            : \"Command %03X\",\n", Messages[i].valueType);
    printf("        \"initvalue\"       : \"00000000\",\n");
    printf("        \"format\"          : 0,\n");
    printf("        \"displaylabel\"    : \"Command %03X\"\n", Messages[i].valueType);
    if ( i + 1 < n ) {
      printf("      },\n");
    } else {
      printf("      }\n");
    }
  }
  printf("    ],\n");
  printf("    \"messages\"            :\n");
  printf("    [\n");
  n = sizeof(Responses) / sizeof(Responses[0]);
  for ( i = 0 ; i < n ; i++ ) {
    printf("      {\n");
    printf("        \"request\"         : %d,\n", Responses[i].request);
    printf("        \"name\"            : \"Request %02x\",\n", Responses[i].request);
    printf("        \"responses\"       : [ ");
    m = sizeof(Responses[i].responses) / sizeof(Responses[i].responses[0]);
    for (k = 0; k < m; k++) {
      if ( Responses[i].responses[k] == 0x00 ) {
        printf(" ]\n");
        break;
      }
      if ( k > 0 ) {
        printf(", ");
      }
      printf("\"%03X\"", Responses[i].responses[k]);
    }
    if ( i + 1 == n ) {
      printf("      }\n");
    } else {
      printf("      },\n");
    }
  }
  printf("    ]\n");
  printf("  }\n");
  return EXIT_SUCCESS;
}

