/*******************************************************************************
 * FILE         : AllCanDefinitions.c
 * DATE         : April 26 2018
 * PROJECT      :
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *******************************************************************************/

/*******************************************************************************
 * Global Headers
 *******************************************************************************/
#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * Local Headers
 *******************************************************************************/
#include "CanMsg.h"
#include "AllCanDefinitions.h"

/*******************************************************************************!
 * Local Data
 *******************************************************************************/    
int NumRegDefList_SMDUE;
int NumMsgDefList_SMDUE;
int NumRegDefList_SMDUH2;
int NumMsgDefList_SMDUH2;
int NumRegDefList_SMTEMP;
int NumMsgDefList_SMTEMP;
int NumRegDefList_Rect;
int NumMsgDefList_Rect;
int NumRegDefList_Rect12;
int NumMsgDefList_Rect12;
int NumRegDefList_BBU;
int NumMsgDefList_BBU;
int NumDeviceDefs;

/*******************************************************************************!
 * Exported Data : RegDefList_SMDUE
 *******************************************************************************/
const CanRegDef RegDefList_SMDUE[] =
{
    // ValueType, InitialValue; MsgType; Group[SHORTSTRINGLEN]; Abbrev[SHORTSTRINGLEN]; Name[STRINGLEN];
    { 0x0001, { 48.0F }, 0x41, "General", "Bus voltage", "Get the bus voltdata",  "Get the bus voltdata" },
    { 0x0002, { 48.1F }, 0x41, "Voltage", "1", "Get the volt data of cabinet 1.", "Cabinet 1." },
    { 0x0003, { 49.2F }, 0x41, "Voltage", "2", "Get the volt data of cabinet 2.", "Cabinet 2." },
    { 0x0004, { 48.3F }, 0x41, "Voltage", "3", "Get the volt data of cabinet 3.", "Cabinet 3." },
    { 0x0005, { 49.4F }, 0x41, "Voltage", "4", "Get the volt data of cabinet 4.", "Cabinet 4." },
    { 0x0006, { 48.5F }, 0x41, "Voltage", "5", "Get the volt data of cabinet 5.", "Cabinet 5." },
    { 0x0007, { 49.6F }, 0x41, "Voltage", "6", "Get the volt data of cabinet 6.", "Cabinet 6." },
    { 0x0008, { 48.7F }, 0x41, "Voltage", "7", "Get the volt data of cabinet 7.", "Cabinet 7." },
    { 0x0009, { 48.8F }, 0x41, "Voltage", "8", "Get the volt data of cabinet 8.", "Cabinet 8." },
    { 0x000A, { 48.9F }, 0x41, "Voltage", "9", "Get the volt data of cabinet 9.", "Cabinet 9." },
    { 0x000B, { 49.1F }, 0x41, "Voltage", "10", "Get the volt data of cabinet 10.", "Cabinet 10." },
    { 0x000C, { .data32 = 0x0 }, 0x41, "General", "Alarm", "Read active alarm / status", "Alarm" },

    /* 00000998 03 / 28 14:55 : 42.945 0x0A6F86DF 0x41F00044D00700DD DUE Unknown        044                0xD00700DD  DB F0
       00000999 03 / 28 14 : 55 : 42.991 0x0A6F86DF 0x41F000463FF00065 DUE Unknown        046 Bar Code  02     0x3FF00065 1.875 DB F0
       00001000 03 / 28 14 : 55 : 43.023 0x0A6F86DF 0x41F0004A30333153 DUE Unknown        04A Bar Code  06     0x30333153 .000 DB F0
       00001001 03 / 28 14 : 55 : 43.054 0x0A6F86DF 0x41F0004B4D445545 DUE Unknown        04B Bar Code  07     0x4D445545  DB F0
       00001002 03 / 28 14 : 55 : 43.069 0x0A6F86DF 0x41F0004C41303100 DUE Unknown        04C SW Version     0x41303100 11.012 DB F0
       00001003 03 / 28 14 : 55 : 43.101 0x0A6F86DB 0x41F0004D00000000 DUE Unknown        04D                0x00000000 .000 DB F0
    */

    { 0x0044, { .data32 = 0xD00700DD }, 0x41, "General", "Bar0", "BarCode starter", "BarCode starter" },
    { 0x0045, { .data32 = 0x31323334 }, 0x41, "General", "Bar1", "BarCode 0 - 3", "BarCode 0 - 3" },
    { 0x0046, { .data32 = 0x3FF00065 }, 0x41, "General", "Bar2", "BarCode 4 - 7", "BarCode 4 - 7" },
    { 0x0047, { .data32 = 0x31323336 }, 0x41, "General", "Bar3", "BarCode 8 - 11", "BarCode 8 - 11" },
    { 0x0048, { .data32 = 0x31323337 }, 0x41, "General", "Bar4", "BarCode 12 - 15", "BarCode 12 - 15" },
    { 0x0049, { .data32 = 0x31323338 }, 0x41, "General", "Bar5", "BarCode 16 - 19", "BarCode 16 - 19" },
    { 0x004A, { .data32 = 0x30333153 }, 0x41, "General", "Bar6", "BarCode 20 - 23", "BarCode 20 - 23" },
    { 0x004B, { .data32 = 0x4D445545 }, 0x41, "General", "Bar7", "BarCode 24 - 27", "BarCode 24 - 27" },
    { 0x004C, { .data32 = 0x41303100 }, 0x41, "General", "Version", "Software Version", "Software Version" },
    { 0x004D, { .data32 = 0x00000000 }, 0x41, "General", "Unknown", "Unknown", "Unknown" },
    { 0x0050, { 0.0 }, 0x41, "General", "Def", "Set / read chan 1 def", "Set / read chan 1 def" },
    { 0x0051, { 100.0 }, 0x41, "Current", "1", "Get the current signal of shunt 1", "Current shunt 1" },
    { 0x0052, { 110.0 }, 0x41, "Current", "2", "Get the current signal of shunt 2", "Current shunt 2" },
    { 0x0053, { 120.0 }, 0x41, "Current", "3", "Get the current signal of shunt 3", "Current shunt 3" },
    { 0x0054, { 130.0 }, 0x41, "Current", "4", "Get the current signal of shunt 4", "Current shunt 4" },
    { 0x0055, { 140.0 }, 0x41, "Current", "5", "Get the current signal of shunt 5", "Current shunt 5" },
    { 0x0056, { 150.0 }, 0x41, "Current", "6", "Get the current signal of shunt 6", "Current shunt 6" },
    { 0x0057, { 160.0 }, 0x41, "Current", "7", "Get the current signal of shunt 7", "Current shunt 7" },
    { 0x0058, { 170.0 }, 0x41, "Current", "8", "Get the current signal of shunt 8", "Current shunt 8" },
    { 0x0059, { 180.0 }, 0x41, "Current", "9", "Get the current signal of shunt 9", "Current shunt 9" },
    { 0x005A, { 190.0 }, 0x41, "Current", "10", "Get the current signal of shunt 10", "Current shunt 10" },
    { 0x0061, { 1.0 }, 0x41, "HallSig", "1", "Get the current signal of hall sensor 1", "Signal Hall 1" },
    { 0x0062, { 1.0 }, 0x41, "HallSig", "2", "Get the current signal of hall sensor 2", "Signal Hall 2" },
    { 0x0063, { 1.0 }, 0x41, "HallSig", "3", "Get the current signal of hall sensor 3", "Signal Hall 3" },
    { 0x0064, { 1.0 }, 0x41, "HallSig", "4", "Get the current signal of hall sensor 4", "Signal Hall 4" },
    { 0x0065, { 1.0 }, 0x41, "HallSig", "5", "Get the current signal of hall sensor 5", "Signal Hall 5" },
    { 0x0066, { 1.0 }, 0x41, "HallSig", "6", "Get the current signal of hall sensor 6", "Signal Hall 6" },
    { 0x0067, { 1.0 }, 0x41, "HallSig", "7", "Get the current signal of hall sensor 7", "Signal Hall 7" },
    { 0x0068, { 1.0 }, 0x41, "HallSig", "8", "Get the current signal of hall sensor 8", "Signal Hall 8" },
    { 0x0069, { 1.0 }, 0x41, "HallSig", "9", "Get the current signal of hall sensor 9", "Signal Hall 9" },
    { 0x006A, { 1.0 }, 0x41, "HallSig", "10", "Get the current signal of hall sensor 10", "Signal Hall 10" },
    { 0x0071, { 1.0 }, 0x41, "VoltageSig", "1", "Get the voltage signal 1", "Voltage Signal 1" },
    { 0x0072, { 1.0 }, 0x41, "VoltageSig", "2", "Get the voltage signal 2", "Voltage Signal 2" },
    { 0x0073, { 1.0 }, 0x41, "VoltageSig", "3", "Get the voltage signal 3", "Voltage Signal 3" },
    { 0x0074, { 1.0 }, 0x41, "VoltageSig", "4", "Get the voltage signal 4", "Voltage Signal 4" },
    { 0x0075, { 1.0 }, 0x41, "VoltageSig", "5", "Get the voltage signal 5", "Voltage Signal 5" },
    { 0x0076, { 1.0 }, 0x41, "VoltageSig", "6", "Get the voltage signal 6", "Voltage Signal 6" },
    { 0x0077, { 1.0 }, 0x41, "VoltageSig", "7", "Get the voltage signal 7", "Voltage Signal 7" },
    { 0x0078, { 1.0 }, 0x41, "VoltageSig", "8", "Get the voltage signal 8", "Voltage Signal 8" },
    { 0x0079, { 1.0 }, 0x41, "VoltageSig", "9", "Get the voltage signal 9", "Voltage Signal 9" },
    { 0x007A, { 1.0 }, 0x41, "VoltageSig", "10", "Get the voltage signal 10", "Voltage Signal 10" },
    { 0x0081, { 1.0 }, 0x41, "TempSig", "1", "Get the temperature signal 1", "Temp Signal 1" },
    { 0x0082, { 1.0 }, 0x41, "TempSig", "2", "Get the temperature signal 2", "Temp Signal 2" },
    { 0x0083, { 1.0 }, 0x41, "TempSig", "3", "Get the temperature signal 3", "Temp Signal 3" },
    { 0x0084, { 1.0 }, 0x41, "TempSig", "4", "Get the temperature signal 4", "Temp Signal 4" },
    { 0x0085, { 1.0 }, 0x41, "TempSig", "5", "Get the temperature signal 5", "Temp Signal 5" },
    { 0x0086, { 1.0 }, 0x41, "TempSig", "6", "Get the temperature signal 6", "Temp Signal 6" },
    { 0x0087, { 1.0 }, 0x41, "TempSig", "7", "Get the temperature signal 7", "Temp Signal 7" },
    { 0x0088, { 1.0 }, 0x41, "TempSig", "8", "Get the temperature signal 8", "Temp Signal 8" },
    { 0x0089, { 1.0 }, 0x41, "TempSig", "9", "Get the temperature signal 9", "Temp Signal 9" },
    { 0x008A, { 1.0 }, 0x41, "TempSig", "10", "Get the temperature signal 10", "Temp Signal 10" },
    { 0x0100, { 301.0 }, 0x41, "VCoeff", "1", "Set / read the voltage Coeff of shunt 1 (mV)", "Voltage Coeff 1" },
    { 0x0101, { 101.0 }, 0x41, "CCoeff", "1", "Set / read the current Coeff of shunt 1 (A)", "Current Coeff 1" },
    { 0x0102, { 302.0 }, 0x41, "VCoeff", "2", "voltage Coeff 2(mV)", "Voltage Coeff 2" },
    { 0x0103, { 102.0 }, 0x41, "CCoeff", "2", "current Coeff 2 (A)", "Current Coeff 2" },
    { 0x0104, { 303.0 }, 0x41, "VCoeff", "3", "voltage Coeff 3 (mV)", "Voltage Coeff 3" },
    { 0x0105, { 103.0 }, 0x41, "CCoeff", "3", "current Coeff 3 (A)", "Current Coeff 3" },
    { 0x0106, { 304.0 }, 0x41, "VCoeff", "4", "voltage Coeff 4(mV)", "Voltage Coeff 4" },
    { 0x0107, { 104.0 }, 0x41, "CCoeff", "4", "current Coeff 4 (A)", "Current Coeff 4" },
    { 0x0108, { 305.0 }, 0x41, "VCoeff", "5", "voltage Coeff 5 (mV)", "Voltage Coeff 5" },
    { 0x0109, { 105.0 }, 0x41, "CCoeff", "5", "current Coeff 5 (A)", "Current Coeff 5" },
    { 0x010A, { 306.0 }, 0x41, "VCoeff", "6", "voltage Coeff 6(mV)", "Voltage Coeff 6" },
    { 0x010B, { 106.0 }, 0x41, "CCoeff", "6", "current Coeff 6 (A)", "Current Coeff 6" },
    { 0x010C, { 307.0 }, 0x41, "VCoeff", "7", "voltage Coeff 7(mV)", "Voltage Coeff 7" },
    { 0x010D, { 107.0 }, 0x41, "CCoeff", "7", "current Coeff 7(A)", "Current Coeff 7" },
    { 0x010E, { 308.0 }, 0x41, "VCoeff", "8", "voltage Coeff 8(mV)", "Voltage Coeff 8" },
    { 0x010F, { 108.0 }, 0x41, "CCoeff", "8", "current Coeff 8 (A)", "Current Coeff 8" },
    { 0x0110, { 309.0 }, 0x41, "VCoeff", "9", "voltage Coeff 9(mV)", "Voltage Coeff 9" },
    { 0x0111, { 109.0 }, 0x41, "CCoeff", "9", "current Coeff 9(A)", "Current Coeff 9" },
    { 0x0112, { 310.0 }, 0x41, "VCoeff", "10", "voltage Coeff 10(mV)", "Voltage Coeff 10" },
    { 0x0113, { 110.0 }, 0x41, "CCoeff", "10", "current Coeff 10 (A)", "Current Coeff 10" },
    { 0x0120, { .data32 = 0x00249249 }, 0x41, "General", "Unknown2", "Unknown2", "Unknown2" }

};

CanMsgDef MsgDefList_SMDUE[] =
{
    //  MsgType; Responses[MAXRESP];
    { 0x00,  { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0b, 0x0c, 0x00, 0x00, 0x00, 0x00 } },
    { 0x20,  { 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x30,  { 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x31,  { 0x100, 0x101, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
};

CanRegDef RegDefList_SMDUH2[] =
{
    // ValueType, InitialValue; MsgType; Name[STRINGLEN];
    { 0x0005, { 100.0 }, 0x41, "General", "Volt", "bus volt", "bus volt" },
    { 0x0040, { 100.0 }, 0x41, "General", "Alarm", "Read active alarm / status", "Read active alarm / status" },
    { 0x0041, { 100.0 }, 0x41, "General", "Status1", "Read current status", "Read current status" },
    { 0x0042, { 100.0 }, 0x41, "General", "Status2", "Read current status", "Read current status" },
    { 0x0051, { .data32 = 0x00000000 }, 0x41, "General", "Status", "Set the characteristic data of this type", "Set the characteristic data of this type" },
    /*
      00001061 03/28 14:55:45.394 0x0A5E9F83 0x20F0008000000000 *SM Req setup data 080 unknown        0x00000000 .000 F0 D3 
      00001062 03/28 14:55:45.425 0x0A5F869F 0x41F0005100000000 *SM Data Response  051 unknown        0x00000000 .000 D3 F0 
      00001063 03/28 14:55:45.487 0x0A5F869F 0x41F00054D00700C7 *SM Data Response  054 SN Low         0xD00700C7  D3 F0 
      00001064 03/28 14:55:45.519 0x0A5F869F 0x41F0005605020064 *SM Data Response  056 Version        0x05020064 .000 D3 F0 
      00001065 03/28 14:55:45.534 0x0A5F869F 0x41F0005A30333153 *SM Data Response  05A Bar Code 1     0x30333153 .000 D3 F0 
      00001066 03/28 14:55:45.565 0x0A5F869F 0x41F0005B4D445548 *SM Data Response  05B Bar Code 2     0x4D445548  D3 F0 
      00001067 03/28 14:55:45.597 0x0A5F869F 0x41F0005C32413031 *SM Data Response  05C Bar Code 3     0x32413031 .000 D3 F0 
      00001068 03/28 14:55:45.628 0x0A5F869B 0x41F0005D46464646 *SM Data Response  05D Bar Code 4     0x46464646 12689.570 D3 F0 
    */
    { 0x0054, { 0xD00700C7 }, 0x41, "General", "SNL", "Read the lower bits of Serial No of node (ID number)", "Read the lower bits of Serial No of node (ID number)" },
    { 0x0055, { 100.0 }, 0x41, "General", "SNH", "Read the higher bits of Searial No of node", "Read the higher bits of Searial No of node" },
    { 0x0056, { .data32 = 0x05020064 }, 0x41, "General", "Version", "Read version number", "Read version number" },
    { 0x005A, { .data32 = 0x30333153 }, 0x41, "General", "Bar1", "Read barcode content 1", "Read barcode content 1" },
    { 0x005B, { .data32 = 0x4D445548 }, 0x41, "General", "Bar2", "Read barcode content 2", "Read barcode content 2" },
    { 0x005C, { .data32 = 0x32413031 }, 0x41, "General", "Bar3", "Read barcode content 3", "Read barcode content 3" },
    { 0x005D, { .data32 = 0x20202046 }, 0x41, "General", "Bar4", "Read barcode content 4", "Read barcode content 4" },
    { 0x00A2, { .data32 = 0x05020064 }, 0x41, "General", "Version", "Set version number", "Set version number" },
    { 0x00AA, { 100.0 }, 0x41, "General", "Type", "Set the characteristic data of this type", "Set the characteristic data of this type" },
    { 0x0100, { 75.0 }, 0x41, "Current", "1", "the current signal of channel 1", "the current signal of channel 1" },
    { 0x0101, { 76.0 }, 0x41, "Current", "2", "the current signal of channel 2", "the current signal of channel 2" },
    { 0x0102, { 77.0 }, 0x41, "Current", "3", "the current signal of channel 3", "the current signal of channel 3" },
    { 0x0103, { 78.0 }, 0x41, "Current", "4", "the current signal of channel 4", "the current signal of channel 4" },
    { 0x0104, { 79.0 }, 0x41, "Current", "5", "the current signal of channel 5", "the current signal of channel 5" },
    { 0x0105, { 80.0 }, 0x41, "Current", "6", "the current signal of channel 6", "the current signal of channel 6" },
    { 0x0106, { 81.0 }, 0x41, "Current", "7", "the current signal of channel 7", "the current signal of channel 7" },
    { 0x0107, { 82.0 }, 0x41, "Current", "8", "the current signal of channel 8", "the current signal of channel 8" },
    { 0x0108, { 81.0 }, 0x41, "Current", "9", "the current signal of channel 9", "the current signal of channel 9" },
    { 0x0109, { 80.0 }, 0x41, "Current", "10", "the current signal of channel 10", "the current signal of channel 10" },
    { 0x010A, { 79.0 }, 0x41, "Current", "11", "the current signal of channel 11", "the current signal of channel 11" },
    { 0x010B, { 78.0 }, 0x41, "Current", "12", "the current signal of channel 12", "the current signal of channel 12" },
    { 0x010C, { 76.0 }, 0x41, "Current", "13", "the current signal of channel 13", "the current signal of channel 13" },
    { 0x010D, { 75.0 }, 0x41, "Current", "14", "the current signal of channel 14", "the current signal of channel 14" },
    { 0x010E, { 74.0 }, 0x41, "Current", "15", "the current signal of channel 15", "the current signal of channel 15" },
    { 0x010F, { 73.0 }, 0x41, "Current", "16", "the current signal of channel 16", "the current signal of channel 16" },
    { 0x0110, { 72.0 }, 0x41, "Current", "17", "the current signal of channel 17", "the current signal of channel 17" },
    { 0x0111, { 71.0 }, 0x41, "Current", "18", "the current signal of channel 18", "the current signal of channel 18" },
    { 0x0112, { 72.0 }, 0x41, "Current", "19", "the current signal of channel 19", "the current signal of channel 19" },
    { 0x0113, { 73.0 }, 0x41, "Current", "20", "the current signal of channel 20", "the current signal of channel 20" },
    { 0x0114, { 74.0 }, 0x41, "Current", "21", "the current signal of channel 21", "the current signal of channel 21" },
    { 0x0115, { 75.0 }, 0x41, "Current", "22", "the current signal of channel 22", "the current signal of channel 22" },
    { 0x0116, { 75.0 }, 0x41, "Current", "23", "the current signal of channel 23", "the current signal of channel 23" },
    { 0x0117, { 75.0 }, 0x41, "Current", "24", "the current signal of channel 24", "the current signal of channel 24" },
    { 0x0118, { 75.0 }, 0x41, "Current", "25", "the current signal of channel 25", "the current signal of channel 25" },
    { 0x0119, { 100.0 }, 0x41, "General", "Voltage1", "voltage 1", "voltage 1" },
    { 0x0132, { 100.0 }, 0x41, "General", "Fuses", "Fuse status and status of bit 1 and bit 3 of DIP switch", "Fuse status and status of bit 1 and bit 3 of DIP switch" },
    { 0x0133, { 100.0 }, 0x41, "Parameter", "1", "Set / read parameters of shunt 1", "Set / read parameters of shunt 1" },
    { 0x0134, { 100.0 }, 0x41, "Parameter", "2", "Set / read parameters of shunt 2", "Set / read parameters of shunt 2" },
    { 0x0135, { 100.0 }, 0x41, "Parameter", "3", "Set / read parameters of shunt 3", "Set / read parameters of shunt 3" },
    { 0x0136, { 100.0 }, 0x41, "Parameter", "4", "Set / read parameters of shunt 4", "Set / read parameters of shunt 4" },
    { 0x0137, { 100.0 }, 0x41, "Parameter", "5", "Set / read parameters of shunt 5", "Set / read parameters of shunt 5" },
    { 0x0138, { 100.0 }, 0x41, "Parameter", "6", "Set / read parameters of shunt 6", "Set / read parameters of shunt 6" },
    { 0x0139, { 100.0 }, 0x41, "Parameter", "7", "Set / read parameters of shunt 7", "Set / read parameters of shunt 7" },
    { 0x013A, { 100.0 }, 0x41, "Parameter", "8", "Set / read parameters of shunt 8", "Set / read parameters of shunt 8" },
    { 0x013B, { 100.0 }, 0x41, "Parameter", "9", "Set / read parameters of shunt 9", "Set / read parameters of shunt 9" },
    { 0x013C, { 100.0 }, 0x41, "Parameter", "10", "Set / read parameters of shunt 10", "Set / read parameters of shunt 10" },
    { 0x013D, { 100.0 }, 0x41, "Parameter", "11", "Set / read parameters of shunt 11", "Set / read parameters of shunt 11" },
    { 0x013E, { 100.0 }, 0x41, "Parameter", "12", "Set / read parameters of shunt 12", "Set / read parameters of shunt 12" },
    { 0x013F, { 100.0 }, 0x41, "Parameter", "13", "Set / read parameters of shunt 13", "Set / read parameters of shunt 13" },
    { 0x0140, { 100.0 }, 0x41, "Parameter", "14", "Set / read parameters of shunt 14", "Set / read parameters of shunt 14" },
    { 0x0141, { 100.0 }, 0x41, "Parameter", "15", "Set / read parameters of shunt 15", "Set / read parameters of shunt 15" },
    { 0x0142, { 100.0 }, 0x41, "Parameter", "16", "Set / read parameters of shunt 16", "Set / read parameters of shunt 16" },
    { 0x0143, { 100.0 }, 0x41, "Parameter", "17", "Set / read parameters of shunt 17", "Set / read parameters of shunt 17" },
    { 0x0144, { 100.0 }, 0x41, "Parameter", "18", "Set / read parameters of shunt 18", "Set / read parameters of shunt 18" },
    { 0x0145, { 100.0 }, 0x41, "Parameter", "19", "Set / read parameters of shunt 19", "Set / read parameters of shunt 19" },
    { 0x0146, { 100.0 }, 0x41, "Parameter", "20", "Set / read parameters of shunt 20", "Set / read parameters of shunt 20" },
    { 0x0147, { 100.0 }, 0x41, "Parameter", "21", "Set / read parameters of shunt 21", "Set / read parameters of shunt 21" },
    { 0x0148, { 100.0 }, 0x41, "Parameter", "22", "Set / read parameters of shunt 22", "Set / read parameters of shunt 22" },
    { 0x0149, { 100.0 }, 0x41, "Parameter", "23", "Set / read parameters of shunt 23", "Set / read parameters of shunt 23" },
    { 0x014A, { 100.0 }, 0x41, "Parameter", "24", "Set / read parameters of shunt 24", "Set / read parameters of shunt 24" },
    { 0x014B, { 100.0 }, 0x41, "Parameter", "25", "Set / read parameters of shunt 25", "Set / read parameters of shunt 25" },
    { 0x014C, { 100.0 }, 0x41, "Power", "1", "power of channel 1", "power of channel 1" },
    { 0x014D, { 100.0 }, 0x41, "Power", "2", "power of channel 2", "power of channel 2" },
    { 0x014E, { 300.3 }, 0x41, "Power", "3", "power of channel 3", "power of channel 3" },
    { 0x014F, { 300.4 }, 0x41, "Power", "4", "power of channel 4", "power of channel 4" },
    { 0x0150, { 300.5 }, 0x41, "Power", "5", "power of channel 5", "power of channel 5" },
    { 0x0151, { 300.6 }, 0x41, "Power", "6", "power of channel 6", "power of channel 6" },
    { 0x0152, { 300.7 }, 0x41, "Power", "7", "power of channel 7", "power of channel 7" },
    { 0x0153, { 300.8 }, 0x41, "Power", "8", "power of channel 8", "power of channel 8" },
    { 0x0154, { 300.9 }, 0x41, "Power", "9", "power of channel 9", "power of channel 9" },
    { 0x0155, { 301.0 }, 0x41, "Power", "10", "power of channel 10", "power of channel 10" },
    { 0x0156, { 301.1 }, 0x41, "Power", "11", "power of channel 11", "power of channel 11" },
    { 0x0157, { 301.2 }, 0x41, "Power", "12", "power of channel 12", "power of channel 12" },
    { 0x0158, { 301.3 }, 0x41, "Power", "13", "power of channel 13", "power of channel 13" },
    { 0x0159, { 301.4 }, 0x41, "Power", "14", "power of channel 14", "power of channel 14" },
    { 0x015A, { 301.5 }, 0x41, "Power", "15", "power of channel 15", "power of channel 15" },
    { 0x015B, { 301.6 }, 0x41, "Power", "16", "power of channel 16", "power of channel 16" },
    { 0x015C, { 301.7 }, 0x41, "Power", "17", "power of channel 17", "power of channel 17" },
    { 0x015D, { 301.8 }, 0x41, "Power", "18", "power of channel 18", "power of channel 18" },
    { 0x015E, { 301.9 }, 0x41, "Power", "19", "power of channel 19", "power of channel 19" },
    { 0x015F, { 302.0 }, 0x41, "Power", "20", "power of channel 20", "power of channel 20" },
    { 0x0160, { 302.1 }, 0x41, "Power", "21", "power of channel 21", "power of channel 21" },
    { 0x0161, { 302.2 }, 0x41, "Power", "22", "power of channel 22", "power of channel 22" },
    { 0x0162, { 302.3 }, 0x41, "Power", "23", "power of channel 23", "power of channel 23" },
    { 0x0163, { 302.4 }, 0x41, "Power", "24", "power of channel 24", "power of channel 24" },
    { 0x0164, { 302.5 }, 0x41, "Power", "25", "power of channel 25", "power of channel 25" },
    { 0x0165, { 500.1 }, 0x41, "DayPower", "1", "total power consumption of Day X of channel 1", "total power consumption of Day X of channel 1" },
    { 0x0166, { 500.2 }, 0x41, "DayPower", "2", "total power consumption of Day X of channel 2", "total power consumption of Day X of channel 2" },
    { 0x0167, { 500.3 }, 0x41, "DayPower", "3", "total power consumption of Day X of channel 3", "total power consumption of Day X of channel 3" },
    { 0x0168, { 500.4 }, 0x41, "DayPower", "4", "total power consumption of Day X of channel 4", "total power consumption of Day X of channel 4" },
    { 0x0169, { 500.5 }, 0x41, "DayPower", "5", "total power consumption of Day X of channel 5", "total power consumption of Day X of channel 5" },
    { 0x016A, { 500.6 }, 0x41, "DayPower", "6", "total power consumption of Day X of channel 6", "total power consumption of Day X of channel 6" },
    { 0x016B, { 500.7 }, 0x41, "DayPower", "7", "total power consumption of Day X of channel 7", "total power consumption of Day X of channel 7" },
    { 0x016C, { 500.8 }, 0x41, "DayPower", "8", "total power consumption of Day X of channel 8", "total power consumption of Day X of channel 8" },
    { 0x016D, { 500.9 }, 0x41, "DayPower", "9", "total power consumption of Day X of channel 9", "total power consumption of Day X of channel 9" },
    { 0x016E, { 501.0 }, 0x41, "DayPower", "10", "total power consumption of Day X of channel 10", "total power consumption of Day X of channel 10" },
    { 0x016F, { 501.1 }, 0x41, "DayPower", "11", "total power consumption of Day X of channel 11", "total power consumption of Day X of channel 11" },
    { 0x0170, { 501.2 }, 0x41, "DayPower", "12", "total power consumption of Day X of channel 12", "total power consumption of Day X of channel 12" },
    { 0x0171, { 501.3 }, 0x41, "DayPower", "13", "total power consumption of Day X of channel 13", "total power consumption of Day X of channel 13" },
    { 0x0172, { 501.4 }, 0x41, "DayPower", "14", "total power consumption of Day X of channel 14", "total power consumption of Day X of channel 14" },
    { 0x0173, { 501.5 }, 0x41, "DayPower", "15", "total power consumption of Day X of channel 15", "total power consumption of Day X of channel 15" },
    { 0x0174, { 501.6 }, 0x41, "DayPower", "16", "total power consumption of Day X of channel 16", "total power consumption of Day X of channel 16" },
    { 0x0175, { 501.7 }, 0x41, "DayPower", "17", "total power consumption of Day X of channel 17", "total power consumption of Day X of channel 17" },
    { 0x0176, { 501.8 }, 0x41, "DayPower", "18", "total power consumption of Day X of channel 18", "total power consumption of Day X of channel 18" },
    { 0x0177, { 501.9 }, 0x41, "DayPower", "19", "total power consumption of Day X of channel 19", "total power consumption of Day X of channel 19" },
    { 0x0178, { 502.0 }, 0x41, "DayPower", "20", "total power consumption of Day X of channel 20", "total power consumption of Day X of channel 20" },
    { 0x0179, { 502.1 }, 0x41, "DayPower", "21", "total power consumption of Day X of channel 21", "total power consumption of Day X of channel 21" },
    { 0x017A, { 502.2 }, 0x41, "DayPower", "22", "total power consumption of Day X of channel 22", "total power consumption of Day X of channel 22" },
    { 0x017B, { 502.3 }, 0x41, "DayPower", "23", "total power consumption of Day X of channel 23", "total power consumption of Day X of channel 23" },
    { 0x017C, { 502.4 }, 0x41, "DayPower", "24", "total power consumption of Day X of channel 24", "total power consumption of Day X of channel 24" },
    { 0x017D, { 502.5 }, 0x41, "DayPower", "25", "total power consumption of Day X of channel 25", "total power consumption of Day X of channel 25" },
    { 0x017E, { 600.0 }, 0x41, "TotalPower", "1", "total power consumption of channel 1", "total power consumption of channel 1" },
    { 0x017F, { 600.1 }, 0x41, "TotalPower", "2", "total power consumption of channel 2", "total power consumption of channel 2" },
    { 0x0180, { 600.2 }, 0x41, "TotalPower", "3", "total power consumption of channel 3", "total power consumption of channel 3" },
    { 0x0181, { 600.3 }, 0x41, "TotalPower", "4", "total power consumption of channel 4", "total power consumption of channel 4" },
    { 0x0182, { 600.4 }, 0x41, "TotalPower", "5", "total power consumption of channel 5", "total power consumption of channel 5" },
    { 0x0183, { 600.5 }, 0x41, "TotalPower", "6", "total power consumption of channel 6", "total power consumption of channel 6" },
    { 0x0184, { 600.6 }, 0x41, "TotalPower", "7", "total power consumption of channel 7", "total power consumption of channel 7" },
    { 0x0185, { 600.7 }, 0x41, "TotalPower", "8", "total power consumption of channel 8", "total power consumption of channel 8" },
    { 0x0186, { 600.8 }, 0x41, "TotalPower", "9", "total power consumption of channel 9", "total power consumption of channel 9" },
    { 0x0187, { 600.9 }, 0x41, "TotalPower", "10", "total power consumption of channel 10", "total power consumption of channel 10" },
    { 0x0188, { 601.0 }, 0x41, "TotalPower", "11", "total power consumption of channel 11", "total power consumption of channel 11" },
    { 0x0189, { 601.1 }, 0x41, "TotalPower", "12", "total power consumption of channel 12", "total power consumption of channel 12" },
    { 0x018A, { 601.2 }, 0x41, "TotalPower", "13", "total power consumption of channel 13", "total power consumption of channel 13" },
    { 0x018B, { 601.3 }, 0x41, "TotalPower", "14", "total power consumption of channel 14", "total power consumption of channel 14" },
    { 0x018C, { 601.4 }, 0x41, "TotalPower", "15", "total power consumption of channel 15", "total power consumption of channel 15" },
    { 0x018D, { 601.5 }, 0x41, "TotalPower", "16", "total power consumption of channel 16", "total power consumption of channel 16" },
    { 0x018E, { 601.6 }, 0x41, "TotalPower", "17", "total power consumption of channel 17", "total power consumption of channel 17" },
    { 0x018F, { 601.7 }, 0x41, "TotalPower", "18", "total power consumption of channel 18", "total power consumption of channel 18" },
    { 0x0190, { 601.8 }, 0x41, "TotalPower", "19", "total power consumption of channel 19", "total power consumption of channel 19" },
    { 0x0191, { 601.9 }, 0x41, "TotalPower", "20", "total power consumption of channel 20", "total power consumption of channel 20" },
    { 0x0192, { 602.0 }, 0x41, "TotalPower", "21", "total power consumption of channel 21", "total power consumption of channel 21" },
    { 0x0193, { 602.1 }, 0x41, "TotalPower", "22", "total power consumption of channel 22", "total power consumption of channel 22" },
    { 0x0194, { 602.2 }, 0x41, "TotalPower", "23", "total power consumption of channel 23", "total power consumption of channel 23" },
    { 0x0195, { 602.3 }, 0x41, "TotalPower", "24", "total power consumption of channel 24", "total power consumption of channel 24" },
    { 0x0196, { 602.4 }, 0x41, "TotalPower", "25", "total power consumption of channel 25", "total power consumption of channel 25" },
    { 0x0197, { 3200.1 }, 0x41, "HallParam", "1", "the Hall parameter (mA/V) of channel 1", "the Hall parameter (mA/V) of channel 1" },
    { 0x0198, { 3200.2 }, 0x41, "HallParam", "2", "the Hall parameter (mA/V) of channel 2", "the Hall parameter (mA/V) of channel 2" },
    { 0x0199, { 3200.3 }, 0x41, "HallParam", "3", "the Hall parameter (mA/V) of channel 3", "the Hall parameter (mA/V) of channel 3" },
    { 0x019A, { 3200.4 }, 0x41, "HallParam", "4", "the Hall parameter (mA/V) of channel 4", "the Hall parameter (mA/V) of channel 4" },
    { 0x019B, { 3200.5 }, 0x41, "HallParam", "5", "the Hall parameter (mA/V) of channel 5", "the Hall parameter (mA/V) of channel 5" },
    { 0x019C, { 3200.6 }, 0x41, "HallParam", "6", "the Hall parameter (mA/V) of channel 6", "the Hall parameter (mA/V) of channel 6" },
    { 0x019D, { 3200.7 }, 0x41, "HallParam", "7", "the Hall parameter (mA/V) of channel 7", "the Hall parameter (mA/V) of channel 7" },
    { 0x019E, { 3200.8 }, 0x41, "HallParam", "8", "the Hall parameter (mA/V) of channel 8", "the Hall parameter (mA/V) of channel 8" },
    { 0x019F, { 3200.9 }, 0x41, "HallParam", "9", "the Hall parameter (mA/V) of channel 9", "the Hall parameter (mA/V) of channel 9" },
    { 0x01A0, { 3201.0 }, 0x41, "HallParam", "10", "the Hall parameter (mA/V) of channel 10", "the Hall parameter (mA/V) of channel 10" },
    { 0x01A1, { 3201.1 }, 0x41, "HallParam", "11", "the Hall parameter (mA/V) of channel 11", "the Hall parameter (mA/V) of channel 11" },
    { 0x01A2, { 3201.2 }, 0x41, "HallParam", "12", "the Hall parameter (mA/V) of channel 12", "the Hall parameter (mA/V) of channel 12" },
    { 0x01A3, { 3201.3 }, 0x41, "HallParam", "13", "the Hall parameter (mA/V) of channel 13", "the Hall parameter (mA/V) of channel 13" },
    { 0x01A4, { 3201.4 }, 0x41, "HallParam", "14", "the Hall parameter (mA/V) of channel 14", "the Hall parameter (mA/V) of channel 14" },
    { 0x01A5, { 3201.5 }, 0x41, "HallParam", "15", "the Hall parameter (mA/V) of channel 15", "the Hall parameter (mA/V) of channel 15" },
    { 0x01A6, { 3201.6 }, 0x41, "HallParam", "16", "the Hall parameter (mA/V) of channel 16", "the Hall parameter (mA/V) of channel 16" },
    { 0x01A7, { 3201.7 }, 0x41, "HallParam", "17", "the Hall parameter (mA/V) of channel 17", "the Hall parameter (mA/V) of channel 17" },
    { 0x01A8, { 3201.8 }, 0x41, "HallParam", "18", "the Hall parameter (mA/V) of channel 18", "the Hall parameter (mA/V) of channel 18" },
    { 0x01A9, { 3201.9 }, 0x41, "HallParam", "19", "the Hall parameter (mA/V) of channel 19", "the Hall parameter (mA/V) of channel 19" },
    { 0x01AA, { 3202.0 }, 0x41, "HallParam", "20", "the Hall parameter (mA/V) of channel 20", "the Hall parameter (mA/V) of channel 20" },
    { 0x01AB, { 3202.1 }, 0x41, "HallParam", "21", "the Hall parameter (mA/V) of channel 21", "the Hall parameter (mA/V) of channel 21" },
    { 0x01AC, { 3202.2 }, 0x41, "HallParam", "22", "the Hall parameter (mA/V) of channel 22", "the Hall parameter (mA/V) of channel 22" },
    { 0x01AD, { 3202.3 }, 0x41, "HallParam", "23", "the Hall parameter (mA/V) of channel 23", "the Hall parameter (mA/V) of channel 23" },
    { 0x01AE, { 3202.4 }, 0x41, "HallParam", "24", "the Hall parameter (mA/V) of channel 24", "the Hall parameter (mA/V) of channel 24" },
    { 0x01AF, { 3202.5 }, 0x41, "HallParam", "25", "the Hall parameter (mA/V) of channel 25", "the Hall parameter (mA/V) of channel 25" },
    { 0x01B0, { 110.1 }, 0x41, "Current", "26", "current signal of channel 26", "current signal of channel 26" },
    { 0x01B1, { 110.2 }, 0x41, "Current", "27", "current signal of channel 27", "current signal of channel 27" },
    { 0x01B2, { 110.3 }, 0x41, "Current", "28", "current signal of channel 28", "current signal of channel 28" },
    { 0x01B3, { 110.4 }, 0x41, "Current", "29", "current signal of channel 29", "current signal of channel 29" },
    { 0x01B4, { 110.5 }, 0x41, "Current", "30", "current signal of channel 30", "current signal of channel 30" },
    { 0x01B5, { 110.6 }, 0x41, "Current", "31", "current signal of channel 31", "current signal of channel 31" },
    { 0x01B6, { 110.7 }, 0x41, "Current", "32", "current signal of channel 32", "current signal of channel 32" },
    { 0x01B7, { 110.8 }, 0x41, "Current", "33", "current signal of channel 33", "current signal of channel 33" },
    { 0x01B8, { 110.9 }, 0x41, "Current", "34", "current signal of channel 34", "current signal of channel 34" },
    { 0x01B9, { 111.0 }, 0x41, "Current", "35", "current signal of channel 35", "current signal of channel 35" },
    { 0x01BA, { 111.1 }, 0x41, "Current", "36", "current signal of channel 36", "current signal of channel 36" },
    { 0x01BB, { 111.2 }, 0x41, "Current", "37", "current signal of channel 37", "current signal of channel 37" },
    { 0x01BC, { 111.3 }, 0x41, "Current", "38", "current signal of channel 38", "current signal of channel 38" },
    { 0x01BD, { 111.4 }, 0x41, "Current", "39", "current signal of channel 39", "current signal of channel 39" },
    { 0x01BE, { 111.5 }, 0x41, "Current", "40", "current signal of channel 40", "current signal of channel 40" },
    { 0x01BF, { 200.0 }, 0x41, "Power", "26", "power of channel 26", "power of channel 26" },
    { 0x01C0, { 200.1 }, 0x41, "Power", "27", "power of channel 27", "power of channel 27" },
    { 0x01C1, { 200.2 }, 0x41, "Power", "28", "power of channel 28", "power of channel 28" },
    { 0x01C2, { 200.3 }, 0x41, "Power", "29", "power of channel 29", "power of channel 29" },
    { 0x01C3, { 200.4 }, 0x41, "Power", "30", "power of channel 30", "power of channel 30" },
    { 0x01C4, { 200.5 }, 0x41, "Power", "31", "power of channel 31", "power of channel 31" },
    { 0x01C5, { 200.6 }, 0x41, "Power", "32", "power of channel 32", "power of channel 32" },
    { 0x01C6, { 200.7 }, 0x41, "Power", "33", "power of channel 33", "power of channel 33" },
    { 0x01C7, { 200.8 }, 0x41, "Power", "34", "power of channel 34", "power of channel 34" },
    { 0x01C8, { 200.9 }, 0x41, "Power", "35", "power of channel 35", "power of channel 35" },
    { 0x01C9, { 201.0 }, 0x41, "Power", "36", "power of channel 36", "power of channel 36" },
    { 0x01CA, { 201.1 }, 0x41, "Power", "37", "power of channel 37", "power of channel 37" },
    { 0x01CB, { 201.2 }, 0x41, "Power", "38", "power of channel 38", "power of channel 38" },
    { 0x01CC, { 201.3 }, 0x41, "Power", "39", "power of channel 39", "power of channel 39" },
    { 0x01CD, { 201.4 }, 0x41, "Power", "40", "power of channel 40", "power of channel 40" },
    { 0x01CE, { 210.1 }, 0x41, "DayPower", "26", "total power consumption of Day X of channel 26", "total power consumption of Day X of channel 26" },
    { 0x01CF, { 210.2 }, 0x41, "DayPower", "27", "total power consumption of Day X of channel 27", "total power consumption of Day X of channel 27" },
    { 0x01D0, { 210.3 }, 0x41, "DayPower", "28", "total power consumption of Day X of channel 28", "total power consumption of Day X of channel 28" },
    { 0x01D1, { 210.4 }, 0x41, "DayPower", "29", "total power consumption of Day X of channel 29", "total power consumption of Day X of channel 29" },
    { 0x01D2, { 210.5 }, 0x41, "DayPower", "30", "total power consumption of Day X of channel 30", "total power consumption of Day X of channel 30" },
    { 0x01D3, { 210.6 }, 0x41, "DayPower", "31", "total power consumption of Day X of channel 31", "total power consumption of Day X of channel 31" },
    { 0x01D4, { 210.7 }, 0x41, "DayPower", "32", "total power consumption of Day X of channel 32", "total power consumption of Day X of channel 32" },
    { 0x01D5, { 210.8 }, 0x41, "DayPower", "33", "total power consumption of Day X of channel 33", "total power consumption of Day X of channel 33" },
    { 0x01D6, { 210.9 }, 0x41, "DayPower", "34", "total power consumption of Day X of channel 34", "total power consumption of Day X of channel 34" },
    { 0x01D7, { 211.0 }, 0x41, "DayPower", "35", "total power consumption of Day X of channel 35", "total power consumption of Day X of channel 35" },
    { 0x01D8, { 211.1 }, 0x41, "DayPower", "36", "total power consumption of Day X of channel 36", "total power consumption of Day X of channel 36" },
    { 0x01D9, { 211.2 }, 0x41, "DayPower", "37", "total power consumption of Day X of channel 37", "total power consumption of Day X of channel 37" },
    { 0x01DA, { 211.3 }, 0x41, "DayPower", "38", "total power consumption of Day X of channel 38", "total power consumption of Day X of channel 38" },
    { 0x01DB, { 211.4 }, 0x41, "DayPower", "39", "total power consumption of Day X of channel 39", "total power consumption of Day X of channel 39" },
    { 0x01DC, { 211.5 }, 0x41, "DayPower", "40", "total power consumption of Day X of channel 40", "total power consumption of Day X of channel 40" },
    { 0x01DD, { 300.1 }, 0x41, "TotalPower", "26", "total power consumption of channel 26", "total power consumption of channel 26" },
    { 0x01DE, { 300.2 }, 0x41, "TotalPower", "27", "total power consumption of channel 27", "total power consumption of channel 27" },
    { 0x01DF, { 300.3 }, 0x41, "TotalPower", "28", "total power consumption of channel 28", "total power consumption of channel 28" },
    { 0x01E0, { 300.4 }, 0x41, "TotalPower", "29", "total power consumption of channel 29", "total power consumption of channel 29" },
    { 0x01E1, { 300.5 }, 0x41, "TotalPower", "30", "total power consumption of channel 30", "total power consumption of channel 30" },
    { 0x01E2, { 300.6 }, 0x41, "TotalPower", "31", "total power consumption of channel 31", "total power consumption of channel 31" },
    { 0x01E3, { 300.7 }, 0x41, "TotalPower", "32", "total power consumption of channel 32", "total power consumption of channel 32" },
    { 0x01E4, { 300.8 }, 0x41, "TotalPower", "33", "total power consumption of channel 33", "total power consumption of channel 33" },
    { 0x01E5, { 300.9 }, 0x41, "TotalPower", "34", "total power consumption of channel 34", "total power consumption of channel 34" },
    { 0x01E6, { 301.0 }, 0x41, "TotalPower", "35", "total power consumption of channel 35", "total power consumption of channel 35" },
    { 0x01E7, { 301.1 }, 0x41, "TotalPower", "36", "total power consumption of channel 36", "total power consumption of channel 36" },
    { 0x01E8, { 301.2 }, 0x41, "TotalPower", "37", "total power consumption of channel 37", "total power consumption of channel 37" },
    { 0x01E9, { 301.3 }, 0x41, "TotalPower", "38", "total power consumption of channel 38", "total power consumption of channel 38" },
    { 0x01EA, { 301.4 }, 0x41, "TotalPower", "39", "total power consumption of channel 39", "total power consumption of channel 39" },
    { 0x01EB, { 301.5 }, 0x41, "TotalPower", "40", "total power consumption of channel 40", "total power consumption of channel 40" },
    { 0x01EC, { 3210.1 }, 0x41, "HallParam", "26", "the Hall parameter (mA/V) of channel 26", "the Hall parameter (mA/V) of channel 26" },
    { 0x01ED, { 3210.2 }, 0x41, "HallParam", "27", "the Hall parameter (mA/V) of channel 27", "the Hall parameter (mA/V) of channel 27" },
    { 0x01EE, { 3210.3 }, 0x41, "HallParam", "28", "the Hall parameter (mA/V) of channel 28", "the Hall parameter (mA/V) of channel 28" },
    { 0x01EF, { 3210.4 }, 0x41, "HallParam", "29", "the Hall parameter (mA/V) of channel 29", "the Hall parameter (mA/V) of channel 29" },
    { 0x01F0, { 3210.5 }, 0x41, "HallParam", "30", "the Hall parameter (mA/V) of channel 30", "the Hall parameter (mA/V) of channel 30" },
    { 0x01F1, { 3210.6 }, 0x41, "HallParam", "31", "the Hall parameter (mA/V) of channel 31", "the Hall parameter (mA/V) of channel 31" },
    { 0x01F2, { 3210.7 }, 0x41, "HallParam", "32", "the Hall parameter (mA/V) of channel 32", "the Hall parameter (mA/V) of channel 32" },
    { 0x01F3, { 3210.8 }, 0x41, "HallParam", "33", "the Hall parameter (mA/V) of channel 33", "the Hall parameter (mA/V) of channel 33" },
    { 0x01F4, { 3210.9 }, 0x41, "HallParam", "34", "the Hall parameter (mA/V) of channel 34", "the Hall parameter (mA/V) of channel 34" },
    { 0x01F5, { 3211.0 }, 0x41, "HallParam", "35", "the Hall parameter (mA/V) of channel 35", "the Hall parameter (mA/V) of channel 35" },
    { 0x01F6, { 3211.1 }, 0x41, "HallParam", "36", "the Hall parameter (mA/V) of channel 36", "the Hall parameter (mA/V) of channel 36" },
    { 0x01F7, { 3211.2 }, 0x41, "HallParam", "37", "the Hall parameter (mA/V) of channel 37", "the Hall parameter (mA/V) of channel 37" },
    { 0x01F8, { 3211.3 }, 0x41, "HallParam", "38", "the Hall parameter (mA/V) of channel 38", "the Hall parameter (mA/V) of channel 38" },
    { 0x01F9, { 3211.4 }, 0x41, "HallParam", "39", "the Hall parameter (mA/V) of channel 39", "the Hall parameter (mA/V) of channel 39" },
    { 0x01FA, { 3211.5 }, 0x41, "HallParam", "40", "the Hall parameter (mA/V) of channel 40", "the Hall parameter (mA/V) of channel 40" },
};

CanMsgDef MsgDefList_SMDUH2[] =
{
    //  MsgType; Responses[MAXRESP];
    { 0x20, { 0x51, 0x54, 0x56, 0x5A, 0x5B, 0x5C, 0x5D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x24, { 0x0100, 0x0101, 0x0102, 0x0103, 0x0104, 0x0105, 0x0106, 0x0107, 0x0108, 0x0109, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x25, { 0x010A, 0x010B, 0x010C, 0x010D, 0x010E, 0x010F, 0x0110, 0x0111, 0x0112, 0x0113, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x26, { 0x0114, 0x0115, 0x0116, 0x0117, 0x0118, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x27, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x30, { 0x014C, 0x014D, 0x014E, 0x014F, 0x0150, 0x0151, 0x0152, 0x0153, 0x0154, 0x0155, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x31, { 0x0156, 0x0157, 0x0158, 0x0159, 0x015A, 0x015B, 0x015C, 0x015D, 0x015E, 0x015F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x32, { 0x0165, 0x0166, 0x0167, 0x0168, 0x0169, 0x016A, 0x016B, 0x016C, 0x016D, 0x016E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }},
    { 0x33, { 0x016F, 0x0170, 0x0171, 0x0172, 0x0173, 0x0174, 0x0175, 0x0176, 0x0177, 0x0178, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x34, { 0x017E, 0x017F, 0x0180, 0x0181, 0x0182, 0x0183, 0x0184, 0x0185, 0x0186, 0x0187, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x35, { 0x0188, 0x0189, 0x018A, 0x018B, 0x018C, 0x018D, 0x018E, 0x018F, 0x0190, 0x0191, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x36, { 0x0197, 0x0198, 0x0199, 0x019A, 0x019B, 0x019C, 0x019D, 0x019E, 0x019F, 0x01A0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x000 } },
    { 0x37, { 0x01A1, 0x01A2, 0x01A3, 0x01A4, 0x01A5, 0x01A6, 0x01A7, 0x01A8, 0x01A9, 0x01AA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x38, { 0x119, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x39, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x3A, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x50, { 0x01B0, 0x01B1, 0x01B2, 0x01B3, 0x01B4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x51, { 0x01B5, 0x01B6, 0x01B7, 0x01B8, 0x01B9, 0x01BA, 0x01BB, 0x01BC, 0x01BD, 0x01BE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x52, { 0x0160, 0x0161, 0x0162, 0x0163, 0x0164, 0x01BF, 0x01C0, 0x01C1, 0x01C2, 0x01C3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x53, { 0x01C4, 0x01C5, 0x01C6, 0x01C7, 0x01C8, 0x01C9, 0x01CA, 0x01CB, 0x01CC, 0x01CD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x54, { 0x0179, 0x017A, 0x017B, 0x017C, 0x017D, 0x01CE, 0x01CF, 0x01D0, 0x01D1, 0x01D2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x55, { 0x01D3, 0x01D4, 0x01D5, 0x01D6, 0x01D7, 0x01D8, 0x01D9, 0x01DA, 0x01DB, 0x01DC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x56, { 0x0192, 0x0193, 0x0194, 0x0195, 0x0196, 0x01DD, 0x01DE, 0x01DF, 0x01E0, 0x01E1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x57, { 0x01E2, 0x01E3, 0x01E4, 0x01E5, 0x01E6, 0x01E7, 0x01E8, 0x01E9, 0x01EA, 0x01EB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x58, { 0x01AB, 0x01AC, 0x01AD, 0x01AE, 0x01AF, 0x01EC, 0x01ED, 0x01EE, 0x01EF, 0x01F0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0x59, { 0x01F1, 0x01F2, 0x01F3, 0x01F4, 0x01F5, 0x01F6, 0x01F7, 0x01F8, 0x01F9, 0x01FA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
};

CanRegDef RegDefList_SMTEMP[] =
{
    // ValueType, InitialValue; MsgType; Name[STRINGLEN];
    { 0x01, { 20.00 }, 0xC1, "Temp", "Temp1", "Input Temp 1", "Input Temp 1" },
    { 0x02, { 20.00 }, 0xC1, "Temp", "Temp2", "Input Temp 1", "Input Temp 1" },
    { 0x03, { 20.00 }, 0xC1, "Temp", "Temp3", "Input Temp 1", "Input Temp 1" },
    { 0x04, { 20.00 }, 0xC1, "Temp", "Temp4", "Input Temp 1", "Input Temp 1" },
    { 0x05, { 20.00 }, 0xC1, "Temp", "Temp5", "Input Temp 1", "Input Temp 1" },
    { 0x06, { 20.00 }, 0xC1, "Temp", "Temp6", "Input Temp 1", "Input Temp 1" },
    { 0x07, { 20.00 }, 0xC1, "Temp", "Temp7", "Input Temp 1", "Input Temp 1" },
    { 0x08, { 20.00 }, 0xC1, "Temp", "Temp8", "Input Temp 1", "Input Temp 1" },
    { 0x40, { 0 }, 0x42, "General", "Alarm", "Read Status", "Read Status" },
    { 0x51, { .data32 = 0x20202020 }, 0x41, "Special", "CharData", "Character Data", "Character Data" },
    { 0x54, { 0 }, 0x41, "General", "SNLow", "SN Low", "SN Low" },
    { 0x55, { .data32 = 0x00001011 }, 0x41, "General", "SNHigh", "SN High", "SN High" },
    { 0x56, { .data32 = 0x000100C8 }, 0x41, "General", "Version", "Version", "Version" },
    { 0x5A, { .data32 = 0x30333153 }, 0x41, "General", "Bar1", "BarCode 1", "BarCode 1" },
    { 0x5B, { .data32 = 0x4D54454D }, 0x41, "General", "Bar2", "BarCode 2", "BarCode 2" },
    { 0x5C, { .data32 = 0x50202020 }, 0x41, "General", "Bar3", "BarCode 3", "BarCode 3" },
    { 0x5D, { .data32 = 0x20202020 }, 0x41, "General", "Bar4", "BarCode 4", "BarCode 4" },
};

CanMsgDef MsgDefList_SMTEMP[] =
{
    //  MsgType; Responses[MAXRESP];
    { 0x00, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x07, 0x40, 0x54, 0x58, 0, 0, 0, 0, 0, 0, 0 } },
    { 0x01, { 0x41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },

};

CanRegDef RegDefList_Rect[] =
{
    // ValueType, InitialValue; MsgType; Name[STRINGLEN];
    { 0x01, { 48.50 }, 0x41, "General", "OutVolt", "Output Volt", "Output Volt" },
    { 0x02, { 5.00 }, 0x41, "General", "OutAmpReal", "Output Amp", "Output Amp" },
    { 0x03, { 1.070 }, 0x41, "General", "CurrLim", "Current Limit", "Current Limit" },
    { 0x04, { 21.00 }, 0x41, "General", "Temp", "M1 Temp", "M1 Temp" },
    { 0x05, { 230.00 }, 0x41, "General", "ACVolt", "AC Voltage", "AC Voltage" },
    { 0x06, { 390.00 }, 0x41, "Special", "HVSDVolt", "HVSD Volt", "HVSD Volt" },
    { 0x07, { 5.00 }, 0x41, "General", "OutAmp", "Output Amps", "Output Amps" },
    { 0x08, { 50.00 }, 0x41, "Special", "PFC0Volt", "PFCO Voltage", "PFCO Voltage" },
    { 0x09, { 65.00 }, 0x41, "Special", "OverTemp", "Overtemp set", "Overtemp set" },
    { 0x0A, { 100.00 }, 0x41, "Special", "PFC1Volt", "PFC1 Voltage", "PFC1 Voltage" },
    { 0x0B, { 25.00 }, 0x41, "Temp", "U1", "U1 Temp", "U1 Temp" },
    { 0x0C, { 230.00 }, 0x41, "Voltage", "ACVoltA", "AC-A Voltage", "AC-A Voltage" },
    { 0x0D, { 231.00 }, 0x41, "Voltage", "ACVoltB", "AC-B Voltage", "AC-B Voltage" },
    { 0x0E, { 232.00 }, 0x41, "Voltage", "ACVoltC", "AC-C Voltage", "AC-C Voltage" },
    { 0x0F, { 12.50 }, 0x41, "General", "DefVolt", "Default Volt", "Default Volt" },
    { 0x10, { 25.00 }, 0x41, "Temp", "PFC", "PFC Temp", "PFC Temp" },
    { 0x11, { 1250.00 }, 0x41, "General", "OutPower", "Output Power", "Output Power" },
    { 0x12, { 250.00 }, 0x41, "Special", "DefAmps", "Default Amps", "Default Amps" },
    { 0x13, { 12.00 }, 0x41, "Special", "DefInVolts", "Def Input V", "Def Input V" },
    { 0x16, { 0.00 }, 0x41, "Special", "estop", "ESTOP", "ESTOP" },
    { 0x18, { 42.00 }, 0x41, "Setting", "DefPowerLim", "Def Power Lim", "Def Power Lim" },
    { 0x19, { 1350.00 }, 0x41, "Setting", "DefCurrLim", "Def Curr Lim", "Def Curr Lim" },
    { 0x1A, { 30.00 }, 0x41, "Setting", "DefACIn", "Def AC Input", "Def AC Input" },
    { 0x20, { 3300.00 }, 0x41, "Setting", "PowerLim", "Power Limit", "Power Limit" },
    { 0x21, { 12.510 }, 0x41, "Setting", "Voltage", "Voltage", "Voltage" },
    { 0x22, { 121.00 }, 0x41, "Setting", "CurrLim", "Current Limit", "Current Limit" },
    { 0x23, { 13.50 }, 0x41, "Setting", "HVLim", "HV Limit", "HV Limit" },
    { 0x24, { 12.50 }, 0x41, "Setting", "DefVolt", "Def Voltage", "Def Voltage" },
    { 0x25, { 1.00 }, 0x41, "Setting", "OverTemp", "OverTemp", "OverTemp" },
    { 0x26, { 1.00 }, 0x41, "Setting", "CurrShareVolt", "Curr Share V", "Curr Share V" },
    { 0x27, { 1.00 }, 0x41, "Setting", "PFCOutVolt", "PFC Out Volt", "PFC Out Volt" },
    { 0x28, { 12.00 }, 0x41, "Setting", "RestartTime", "Restart Time", "Restart Time" },
    { 0x29, { 8.00 }, 0x41, "Setting", "WalkTime", "Walk-in Time", "Walk-in Time" },
    { 0x2A, { 0.00 }, 0x41, "Setting", "StartInt", "Start Intervl", "Start Intervl" },
    { 0x2B, { 0.00 }, 0x41, "Setting", "CANBaud", "CAN baud rate", "CAN baud rate" },
    { 0x30, { 1.00 }, 0x41, "Setting", "On", "On/Off", "On/Off" },
    { 0x31, { 0.00 }, 0x41, "Setting", "OverVoltReset", "OverVlt Reset", "OverVlt Reset" },
    { 0x32, { 0.00 }, 0x41, "Setting", "Walk", "Walk-in", "Walk-in" },
    { 0x33, { 1.00 }, 0x41, "Setting", "FanSpeed", "Fan Speed", "Fan Speed" },
    { 0x34, { 1.00 }, 0x41, "Setting", "GreenLED", "Green LED", "Green LED" },
    { 0x35, { 0.00 }, 0x41, "Setting", "OverVoltRelay", "OverVolt Rely", "OverVolt Rely" },
    //{ 0x36, { 0.00 }, 0x41, "", "", "CAN init", "CAN init" },
    //{ 0x37, { 0.00 }, 0x41, "", "", "Calibration", "Calibration" },
    { 0x38, { 1.00 }, 0x41, "General", "Version", "Version", "Version" },
    //{ 0x39, { 42.00 } 0x41, "", "", "OV Prot Time", "OV Prot Time" },
    //{ 0x3a, { 42.00 }, 0x41, "", "", "OV Disconnect", "OV Disconnect" },
    //{ 0x3b, { 42.00 }, 0x41, "", "", "Measure Mem", "Measure Mem" },
    //{ 0x3c, { 0.00 }, 0x41, "", "", "Addr re-id", "Addr re-id" },
    //{ 0x3d, { 0.00 }, 0x41, "", "", "After AC Lost", "After AC Lost" },
    //{ 0x3e, { 0.00 }, 0x41, "", "", "Volt Correl", "Volt Correl" },
    //{ 0x3f, { 0.00 }, 0x41, "", "", "Start SelfDiag", "Start SelfDiag" },
    { 0x40, { 0 }, 0x42, "General", "Alarm", "Read Status", "Read Status" },
    //{ 0x50, { 0.00 }, 0x41, "", "", "Load-share Vlt", "Load-share Vlt" },
    { 0x51, { .data32 = 0x01F425FF }, 0x41, "General", "CharData", "Character Data", "Character Data" },
    //{ 0x52, { 0.00 }, 0x41, "", "", "First Maint", "First Maint" },
    //{ 0x53, { 0.00 }, 0x41, "", "", "Second Maint", "Second Maint" },
    { 0x54, { .data32 = 0x4F0F6B00 }, 0x41, "General", "SNLow", "SN Low", "SN Low" },
    { 0x55, { .data32 = 0x00001000 }, 0x41, "General", "SNHigh", "SN High", "SN High" },
    { 0x56, { .data32 = 0x00000065 }, 0x41, "General", "Version", "Version", "Version" },
    { 0x58, { 100.00 }, 0x41, "General", "RunTime", "Runtime", "Runtime" },
    { 0x59, { 142.00 }, 0x41, "General", "RepairTime", "Repair Time", "Repair Time" },
    { 0x5A, { .data32 = 0x30333152 }, 0x41, "General", "Bar1", "BarCode 1", "BarCode 1" },
    { 0x5B, { .data32 = 0x31323333 }, 0x41, "General", "Bar2", "BarCode 2", "BarCode 2" },
    { 0x5C, { .data32 = 0x30302020 }, 0x41, "General", "Bar3", "BarCode 3", "BarCode 3" },
    { 0x5D, { .data32 = 0x20202020 }, 0x41, "General", "Bar4", "BarCode 4", "BarCode 4" },
    //{ 0x5E, { 1.10 }, 0x41, "", "", "Fan Vers", "Fan Vers" },
    //{ 0x5F, { 1.20 }, 0x41, "", "", "VoltProt Vers", "VoltProt Vers" },

};

//0x01 Output Volt, 0x02 Output Amp, 0x03 Current Limit, 0x04 M1 Temp, 0x05 AC Voltage, 0x07 Output Amps, 0x40 Read Status, 0x54 SN Low, 0x58 Runtime

CanMsgDef MsgDefList_Rect[] =
{
    //  MsgType; Responses[MAXRESP];
    { 0x00, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x07, 0x40, 0x54, 0x58, 0, 0, 0, 0, 0, 0, 0 } },
    { 0x01, { 0x41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
    { 0x10, { 0x0C, 0x0D, 0x0E, 0x40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
    { 0x20, { 0x51, 0x55, 0x56, 0x5a, 0x5b, 0x5c, 0x5d, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
};

CanRegDef RegDefList_Rect12[] =
{
    // ValueType, InitialValue; MsgType; Name[STRINGLEN];
    { 0x01, { 12.50 }, 0x41, "General", "OutVolt", "Output Volt", "Output Volt" },
    { 0x02, { 100.00 }, 0x41, "General", "OutAmpReal", "Output Amp", "Output Amp" },
    { 0x03, { 1.070 }, 0x41, "General", "CurrLim", "Current Limit", "Current Limit" },
    { 0x04, { 21.00 }, 0x41, "General", "Temp", "M1 Temp", "M1 Temp" },
    { 0x05, { 230.00 }, 0x41, "General", "ACVolt", "AC Voltage", "AC Voltage" },
    { 0x06, { 390.00 }, 0x41, "Special", "HVSDVolt", "HVSD Volt", "HVSD Volt" },
    { 0x07, { 100.00 }, 0x41, "General", "OutAmp", "Output Amps", "Output Amps" },
    { 0x08, { 50.00 }, 0x41, "Special", "PFC0Volt", "PFCO Voltage", "PFCO Voltage" },
    { 0x09, { 65.00 }, 0x41, "Special", "OverTemp", "Overtemp set", "Overtemp set" },
    { 0x0A, { 100.00 }, 0x41, "Special", "PFC1Volt", "PFC1 Voltage", "PFC1 Voltage" },
    { 0x0B, { 25.00 }, 0x41, "Temp", "U1", "U1 Temp", "U1 Temp" },
    { 0x0C, { 230.00 }, 0x41, "Voltage", "ACVoltA", "AC-A Voltage", "AC-A Voltage" },
    { 0x0D, { 231.00 }, 0x41, "Voltage", "ACVoltB", "AC-B Voltage", "AC-B Voltage" },
    { 0x0E, { 232.00 }, 0x41, "Voltage", "ACVoltC", "AC-C Voltage", "AC-C Voltage" },
    { 0x0F, { 12.50 }, 0x41, "General", "DefVolt", "Default Volt", "Default Volt" },
    { 0x10, { 25.00 }, 0x41, "Temp", "PFC", "PFC Temp", "PFC Temp" },
    { 0x11, { 1250.00 }, 0x41, "General", "OutPower", "Output Power", "Output Power" },
    { 0x12, { 250.00 }, 0x41, "Special", "DefAmps", "Default Amps", "Default Amps" },
    { 0x13, { 12.00 }, 0x41, "Special", "DefInVolts", "Def Input V", "Def Input V" },
    { 0x16, { 0.00 }, 0x41, "Special", "estop", "ESTOP", "ESTOP" },
    { 0x18, { 42.00 }, 0x41, "Setting", "DefPowerLim", "Def Power Lim", "Def Power Lim" },
    { 0x19, { 1350.00 }, 0x41, "Setting", "DefCurrLim", "Def Curr Lim", "Def Curr Lim" },
    { 0x1A, { 30.00 }, 0x41, "Setting", "DefACIn", "Def AC Input", "Def AC Input" },
    { 0x20, { 3300.00 }, 0x41, "Setting", "PowerLim", "Power Limit", "Power Limit" },
    { 0x21, { 12.510 }, 0x41, "Setting", "Voltage", "Voltage", "Voltage" },
    { 0x22, { 121.00 }, 0x41, "Setting", "CurrLim", "Current Limit", "Current Limit" },
    { 0x23, { 13.50 }, 0x41, "Setting", "HVLim", "HV Limit", "HV Limit" },
    { 0x24, { 12.50 }, 0x41, "Setting", "DefVolt", "Def Voltage", "Def Voltage" },
    { 0x25, { 1.00 }, 0x41, "Setting", "OverTemp", "OverTemp", "OverTemp" },
    { 0x26, { 1.00 }, 0x41, "Setting", "CurrShareVolt", "Curr Share V", "Curr Share V" },
    { 0x27, { 1.00 }, 0x41, "Setting", "PFCOutVolt", "PFC Out Volt", "PFC Out Volt" },
    { 0x28, { 12.00 }, 0x41, "Setting", "RestartTime", "Restart Time", "Restart Time" },
    { 0x29, { 8.00 }, 0x41, "Setting", "WalkTime", "Walk-in Time", "Walk-in Time" },
    { 0x2A, { 0.00 }, 0x41, "Setting", "StartInt", "Start Intervl", "Start Intervl" },
    { 0x2B, { 0.00 }, 0x41, "Setting", "CANBaud", "CAN baud rate", "CAN baud rate" },
    { 0x30, { 1.00 }, 0x41, "Setting", "On", "On/Off", "On/Off" },
    { 0x31, { 0.00 }, 0x41, "Setting", "OverVoltReset", "OverVlt Reset", "OverVlt Reset" },
    { 0x32, { 0.00 }, 0x41, "Setting", "Walk", "Walk-in", "Walk-in" },
    { 0x33, { 1.00 }, 0x41, "Setting", "FanSpeed", "Fan Speed", "Fan Speed" },
    { 0x34, { 1.00 }, 0x41, "Setting", "GreenLED", "Green LED", "Green LED" },
    { 0x35, { 0.00 }, 0x41, "Setting", "OverVoltRelay", "OverVolt Rely", "OverVolt Rely" },
    //{ 0x36, 0.00 }, 0x41, "", "", "CAN init", "CAN init" },
    //{ 0x37, 0.00 }, 0x41, "", "", "Calibration", "Calibration" },
    { 0x38, { 1.00 }, 0x41, "General", "Version", "Version", "Version" },
    //{ 0x39, 42.00, 0x41, "", "", "OV Prot Time", "OV Prot Time" },
    //{ 0x3a, 42.00, 0x41, "", "", "OV Disconnect", "OV Disconnect" },
    //{ 0x3b, 42.00, 0x41, "", "", "Measure Mem", "Measure Mem" },
    //{ 0x3c, 0.00, 0x41, "", "", "Addr re-id", "Addr re-id" },
    //{ 0x3d, 0.00, 0x41, "", "", "After AC Lost", "After AC Lost" },
    //{ 0x3e, 0.00, 0x41, "", "", "Volt Correl", "Volt Correl" },
    //{ 0x3f, 0.00, 0x41, "", "", "Start SelfDiag", "Start SelfDiag" },
    { 0x40, { 0 }, 0x42, "General", "Alarm", "Read Status", "Read Status" },
    //{ 0x50, 0.00, 0x41, "", "", "Load-share Vlt", "Load-share Vlt" },
    { 0x51, { .data32 = 0x01F425FF }, 0x41, "General", "CharData", "Character Data", "Character Data" },
    //{ 0x52, 0.00 }, 0x41, "", "", "First Maint", "First Maint" },
    //{ 0x53, 0.00 }, 0x41, "", "", "Second Maint", "Second Maint" },
    { 0x54, { .data32 = 0x4F0F6B00 }, 0x41, "General", "SNLow", "SN Low", "SN Low" },
    { 0x55, { .data32 = 0x00001000 }, 0x41, "General", "SNHigh", "SN High", "SN High" },
    { 0x56, { .data32 = 0x00000065 }, 0x41, "General", "Version", "Version", "Version" },
    { 0x58, { 100.00 }, 0x41, "General", "RunTime", "Runtime", "Runtime" },
    { 0x59, { 142.00 }, 0x41, "General", "RepairTime", "Repair Time", "Repair Time" },
    { 0x5A, { .data32 = 0x30333152 }, 0x41, "General", "Bar1", "BarCode 1", "BarCode 1" },
    { 0x5B, { .data32 = 0x31323333 }, 0x41, "General", "Bar2", "BarCode 2", "BarCode 2" },
    { 0x5C, { .data32 = 0x30302020 }, 0x41, "General", "Bar3", "BarCode 3", "BarCode 3" },
    { 0x5D, { .data32 = 0x20202020 }, 0x41, "General", "Bar4", "BarCode 4", "BarCode 4" },
    //{ 0x5E, 1.10, 0x41, "", "", "Fan Vers", "Fan Vers" },
    //{ 0x5F, 1.20, 0x41, "", "", "VoltProt Vers", "VoltProt Vers" },

};

//0x01 Output Volt, 0x02 Output Amp, 0x03 Current Limit, 0x04 M1 Temp, 0x05 AC Voltage, 0x07 Output Amps, 0x40 Read Status, 0x54 SN Low, 0x58 Runtime

CanMsgDef MsgDefList_Rect12[] =
{
    //  MsgType; Responses[MAXRESP];
    { 0x00, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x07, 0x40, 0x54, 0x58, 0, 0, 0, 0, 0, 0, 0} },
    { 0x01, { 0x41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    { 0x10, { 0x0C, 0x0D, 0x0E, 0x40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    { 0x20, { 0x51, 0x55, 0x56, 0x5a, 0x5b, 0x5c, 0x5d, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
};

CanRegDef RegDefList_BBU[] =
{
    // ValueType, InitialValue; MsgType; Name[STRINGLEN];
    { 0x01, { 12.5 }, 0x7A, "General", "OutVolt","Output Voltage", "Output Voltage" },
    { 0x02, { 100.00 }, 0x7A, "General", "OutAmp","Output Current", "Output Current" },
    { 0x03, { 85.00 }, 0x7A, "General", "Temp","Battery pack temperature", "Battery pack Temp" },
    { 0x04, { 100.00 }, 0x7A, "General", "RAC","RAC(external SOC) (0 to 100 %)", "RAC(external SOC)" },
    { 0x05, { 0.00 }, 0x7A, "General", "ToRecharge","battery time to recharge to 100 % RAC", "Time to 100% recharge" },
    { 0x06, { 12.00 }, 0x7A, "General", "ChargeVolt","Charging voltage", "Charging voltage" },
    { 0x07, { 12.00 }, 0x7A, "General", "ChargeAmp","Charge Amp", "Charge Amp" },
    { 0x08, { 70.00 }, 0x7A, "General", "SOC","RelativeStateOfCharge(SBS)", "Relative SOC(SBS)" },
    { 0x09, { 75.00 }, 0x7A, "General", "AbsSOC","AbsoluteStateOfCharge(SBS)", "Absolute SOC(SBS)" },
    { 0x10, { 90.00 }, 0x7A, "General", "Cap","RemainingCapacity(SBS)", "Capacity (SBS)" },
    { 0x11, { 92.00 }, 0x7A, "General", "FullChargeCap","FFullChargeCapacity(SBS)", "Full Charge Cap(SBS)"},
    { 0x20, { 12.20 }, 0x7A, "Setting", "Volt","Output voltage setpoint", "Out Voltage Set"},
    { 0x21, { 0 }, 0x7A, "Setting", "epo","EPO", "EPO" },
    { 0x40, { .data32 = 0xF9000000 }, 0x7B, "General", "Alarm", "Active alarms and status", "Alarms" },
    { 0x50, { 12.00 }, 0x7A, "Info", "DesignVolt","Design Voltage", "Design Voltage" },
    { 0x51, { .data32 = 0x42960000 }, 0x7A, "Info", "DesignPower", "Design Power", "Design Power" },
    { 0x52, { 3000.00 }, 0x7A, "Info", "RatedOut","Rated battery output @ 90 seconds", "Rated output" },
    { 0x53, { 0 }, 0x7A, "Info", "xxx","Reserved", "Reserved" },
    { 0x54, { .data32 = 0xCE0A0036 }, 0x7A, "General", "SNLow", "SN Low", "SN Low" },
    { 0x55, { .data32 = 0x0000F002 }, 0x7A, "General", "SNHigh", "SN High", "SN High" },
    { 0x56, { .data32 = 0x45064403 }, 0x7A, "General", "Version", "Version", "Version" },
    { 0x58, { 1.00 }, 0x7A, "General", "RunTime","Runtime", "Runtime" },
    { 0x5A, { .data32 = 0x31313142 }, 0x7A, "General", "Bar1", "Bar Code 1", "Bar Code 1" },
    { 0x5B, { .data32 = 0x31323330 }, 0x7A, "General", "Bar2", "Bar Code 2", "Bar Code 2" },
    { 0x5C, { .data32 = 0x30302020 }, 0x7A, "General", "Bar3", "Bar Code 3", "Bar Code 3" },
    { 0x5D, { .data32 = 0x41303320 }, 0x7A, "General", "Bar4", "Bar Code 4", "Bar Code 4" },
    { 0xF0, { .data32 = 0x42880000 }, 0x7A, "Info", "LifeCol", "Lifetime Coulomb count; cumulative discharge energy at 12V", "Lifetime Coulomb" },
    { 0xF1, { 42.00 }, 0x7A, "Info", "NumHigh","Number of high event cycles(discharge more than 2.0kW)", "Number high event" },
    { 0xF2, { 125.0 }, 0x7A, "Info", "MaxTemp", "Lifetime battery pack max temperature", "Max temperature" },
    { 0xF3, { 5.00 }, 0x7A, "Info", "MinTemp","Lifetime battery pack min temperature", "Min temperature"},
    { 0xF4, { .data32 = 0x41836666 }, 0x7A, "Info", "MaxCell", "Lifetime maximum cell voltage", "Maximum Cell" },
    { 0xF5, { .data32 = 0x41400000 }, 0x7A, "Info", "MinCell", "Lifetime minimum cell voltage", "Minimum Cell" },
    { 0xF6, { 28.0 }, 0x7A, "Info", "Discharges","Lifetime cycle count(Wh discharged / 75Wh)", "Lifetime discharges"},
    { 0xF7, { 1.00 }, 0x7A, "Info", "Charges", "Lifetime max charge current", "Lifetime max charge" },
    { 0xF8, { .data32 = 0x40BBF7CF }, 0x7A, "Info", "Discharges", "Lifetime max discharge current", "Lifetime max discharge" },
    { 0xF9, { 1000.0 }, 0x7A, "Info", "DaysLearn", "Days since last successful learning mode", "Days learning mode" },
    { 0xFA, { 3.0 }, 0x7A, "Info", "DisChargeLearn", "Wh discharged(like 0x00F0) since last successful learning mode", "Discharged learning mode"}
};

//0x01 Output Volt, 0x02 Output Amp, 0x03 Current Limit, 0x04 M1 Temp, 0x05 AC Voltage, 0x07 Output Amps, 0x40 Read Status, 0x54 SN Low, 0x58 Runtime

CanMsgDef MsgDefList_BBU[] =
{
    //  MsgType; Responses[MAXRESP];
    { 0x71, { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x40, 0x58, 0, 0, 0 } },
    { 0x01, { 0x41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
    { 0x10, { 0x0C, 0x0D, 0x0E, 0x40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
    { 0x70, { 0x51, 0x54, 0x55, 0x56, 0x5a, 0x5b, 0x5c, 0x5d, 0, 0, 0, 0, 0, 0, 0, 0 } },
};

CanDeviceDef CanDeviceDefList[] =
{
    //  CanAddressStart; ProtocolNumber; Name[STRINGLEN]; RegDefList; NumRegDefList; MsgDefList; NumMsgDefList; UniqueSNLow; StartingSNLow;
    { 0xDB, 0xA6, "BAY",    "Bay",          &RegDefList_SMDUE[0],  &MsgDefList_SMDUE[0],   false, { 0 }, 4, DISPLAY_TYPE_COLLAPSE },
    { 0xD3, 0xA5, "PANEL",  "Panel",        &RegDefList_SMDUH2[0], &MsgDefList_SMDUH2[0],  true,  { 0 }, 2, DISPLAY_TYPE_COLLAPSE  },
    { 0x00, 0x60, "REC12",  "Rectifier 12", &RegDefList_Rect12[0], &MsgDefList_Rect12[0],  true,  { 0x8F6B0019 }, 5, DISPLAY_TYPE_FIXED },
    { 0x00, 0xA4, "BBU",    "BBU",          &RegDefList_BBU[0],    &MsgDefList_BBU[0],     true,  { 0x8F6B0029 }, 4, DISPLAY_TYPE_FIXED },
    { 0x00, 0x68, "SMTEMP", "SMTEMP",       &RegDefList_SMTEMP[0], &MsgDefList_SMTEMP[0],  true,  { 0x00000CE3 }, 5, DISPLAY_TYPE_FIXED },
    { 0x00, 0x60, "REC",    "Rectifier",    &RegDefList_Rect[0],   &MsgDefList_Rect[0],    true,  { 0x8F6B0019 }, 5, DISPLAY_TYPE_FIXED },
};

/*******************************************************************************
 * Function : CanDefinitionsInitialize
 *******************************************************************************/
void
CanDefinitionsInitialize()
{
    NumRegDefList_SMDUE         = (sizeof(RegDefList_SMDUE)/sizeof(CanRegDef));
    NumMsgDefList_SMDUE         = (sizeof(MsgDefList_SMDUE)/sizeof(CanMsgDef));
    NumRegDefList_SMDUH2        = (sizeof(RegDefList_SMDUH2)/sizeof(CanRegDef));
    NumMsgDefList_SMDUH2        = (sizeof(MsgDefList_SMDUH2)/sizeof(CanMsgDef));
    NumRegDefList_SMTEMP        = (sizeof(RegDefList_SMTEMP)/sizeof(CanRegDef));
    NumMsgDefList_SMTEMP        = (sizeof(MsgDefList_SMTEMP)/sizeof(CanMsgDef));
    NumRegDefList_Rect  	= (sizeof(RegDefList_Rect)/sizeof(CanRegDef));
    NumMsgDefList_Rect  	= (sizeof(MsgDefList_Rect)/sizeof(CanMsgDef));
    NumRegDefList_Rect12        = (sizeof(RegDefList_Rect12)/sizeof(CanRegDef));
    NumMsgDefList_Rect12        = (sizeof(MsgDefList_Rect12)/sizeof(CanMsgDef));
    NumRegDefList_BBU   	= (sizeof(RegDefList_BBU)/sizeof(CanRegDef));
    NumMsgDefList_BBU   	= (sizeof(MsgDefList_BBU)/sizeof(CanMsgDef));
    NumDeviceDefs       	= (sizeof(CanDeviceDefList) / sizeof(CanDeviceDef));
}

