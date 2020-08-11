#ifndef _canmsg_h_
#define _canmsg_h_

/*******************************************************************************!
 * Global Headers
 *******************************************************************************/
#include <unistd.h>
#include <stdint.h>
#include <linux/can.h>
#include <time.h>

/*******************************************************************************!
 * Local Headers
 *******************************************************************************/
#include "DeviceDef.h"
#include "NumericTypes.h"

/*******************************************************************************!
 * Local Macros
 *******************************************************************************/
#define LOGSTRING 256
#define STRINGLEN 64
#define SHORTSTRINGLEN 20
#define MAXRESP 16
#define MAXDEVICES 128
#define MAXGROUPS 16

#define DISPLAY_TYPE_FIXED 1
#define DISPLAY_TYPE_COLLAPSE 2

#define max(X, Y) (((X) > (Y)) ? (X) : (Y))
#define COMMANDSIZE 128
#define delim " \n"

/* 

The basic data structures in this applications are:
Can devices, which represent a device like a rectifier or SMDUE.
A can device has a set of registers.  The registers are defined in the GBB can spec.  Each register has a unique value type.
A can device also has a list of request messages.  For each request message, it keeps a list of the registers that it will send when this request is recieved.
For example, when a can device gets message type 0x24, it might respond with a list of registers (value types 0x01, 0x02, 0x10, and 0x12 for example).

Any static information about a device, register, or message is kept in CanDeviceDef, CanRegDef, and CanMsgDef.  For example, protocol number for a device is static.
Any dynamic information about a device or register is kept in CanDevice and CanReg.  For example, the current value for a register is dynamic.
CanDevice has a pointer to CanDeviceDef.  CanReg has a pointer to CanRegDef.

CanDeviceDefList[] is the list of all of the CanDeviceDef structures. This is the master list of all devices that are supported by the application.
CanDeviceList[] is the list of the CanDevice structures created in a session.

CanDevice has the device's Can address and its state.  
CanDevice has a link to its CanDeviceDef and a link to Registers[].  Registers[] is a list of CanReg.

CanDeviceDef defines the supported devices.  It has the starting Can address, the protocol number, the text name, the number of registers, 
and the number of message definitions.
CanDeviceDef also has a link to CanRegDef[] and CanMsgDef[].

CanRegDef[] is a list of CanRegDef.  CanRegDef contains the value type, the initial value for the register, the message type, and the text name of the register.

CanMsgDef[] is a list of CanMsgDef.  CanMsgDef contains the message type and the value types that are returned when this message type is received.

*/

/*******************************************************************************!
 * Exported Type : CanRegDef
 *******************************************************************************/
typedef const struct  {
    int ValueType;
    ufloatbit32_t InitialValue;
    int MsgType;
    char Group[SHORTSTRINGLEN];
    char Abbrev[SHORTSTRINGLEN];
    char Name[STRINGLEN];
    char DisplayLabel[STRINGLEN];
} CanRegDef;

/*******************************************************************************!
 * Exported Type : CanReg
 *******************************************************************************/
typedef struct  {
    ufloatbit32_t Value;
    DeviceRegDef* registerDef;
} CanReg;

/*******************************************************************************!
 * Exported Type : CanMsgDef
 *******************************************************************************/
typedef const struct  {
    int MsgType;
    int Responses[MAXRESP];
} CanMsgDef;

/*******************************************************************************!
 * Exported Type : CanDeviceDef
 *******************************************************************************/
typedef const struct{
    int CanAddressStart;
    int ProtocolNumber;
    char Name[STRINGLEN];
    char Desc[STRINGLEN];
    CanRegDef* RegDefList;
    CanMsgDef* MsgDefList;
    bool UniqueSNLow;
    ufloatbit32_t StartingSNLow;
    int DisplayColumns;
    int DisplayType;
} CanDeviceDef;

/*******************************************************************************!
 * Exported Type : CanDevice
 *******************************************************************************/
typedef struct  {
    int 				CanAddress;
    enum 				{ normal = 0xf0, error = 0xF2, offline = 0xFF, addressing = 0xf4 } State;
    DeviceDef*				deviceDefinition;
    CanReg*                         	Registers;
    int					registersCount;
    time_t				lastContactTime;
} CanDevice;

/*******************************************************************************!
 * Exported Type : frameid
 *******************************************************************************/
typedef union
{
    uint32_t data32;
    uint16_t msgid[2];
    struct
    {
        unsigned RES2 : 1;		/* reserved */
        unsigned RES1 : 1;		/* reserved */
        unsigned CNT : 1;		/* 1:continuous frame */
        unsigned SrcAddr : 8;		/* source address */
        unsigned DstAddr : 8;		/* destination/group address low bits*/
        unsigned PTP : 1;		/* 0:broadcast, 1:point to point */
        unsigned ProtNo : 9;		/* protocol number */
    }msgbit;
}frameid;

/*******************************************************************************!
 * Exported Type : dataframe
 *******************************************************************************/
typedef union  {
    uint64_t data64;
    int16_t msgid[4];
    struct
    {
        uint32_t Value;
        uint16_t ValueType;
        uint8_t ErrType;
        uint8_t MsgType;
    } data;
    uint8_t data8[CAN_MAX_DLEN];
} dataframe;


/*******************************************************************************!
 * Exported Functions
 *******************************************************************************/
frameid
GetFrameId
(int Protocol, int SrcAddr, int DestAddr, int Continue);

dataframe
GetDataFrame
(int ErrType, int MsgType, int ValueType, ufloatbit32_t Value);


void
ParseId
(frameid Id, int* Protocol, int* SrcAddr, int* DestAddr);

void
ParseData
(dataframe Data, int* IncomingMsgType, int* IncomingValueType, int* IncomingErrorType, ufloatbit32_t* IncomingValue);

CanDevice*
FindFirstDevice
(char* InDeviceName);

CanDevice*
FindNextDevice
(CanDevice* InCurrentDevice);

void
CanRegSetFromString
(CanReg* InCanReg, string InValueString);

string
CanRegGetFromString
(string InValueString, CanReg* InCanReg);

#endif
