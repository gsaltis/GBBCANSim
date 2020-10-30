/*****************************************************************************
 * FILE NAME    : CanMsg.c
 * DATE         : May 29 2020
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (C) 2020 by Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Global Headers
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/*****************************************************************************!
 * Local Headers 
 *****************************************************************************/
#include "CanMsg.h"
#include "ThreadSafePrint.h"
#include "String.h"
#include "MemoryManager.h"

/*****************************************************************************!
 * Exported Type : CanMessage
 *****************************************************************************/
typedef struct {
	int Protocol;
	int SrcAddr;
	int DestAddr;
	int MsgType;
	int ValueType;
	ufloatbit32_t Value;
} CanMessage;

/*****************************************************************************!
 *  
 *****************************************************************************/
uint64_t GetBits(uint64_t InNum, int InMSB, int InLSB)
{
	uint64_t Temp1, Temp2, Temp3, Temp4;
	Temp1 = InNum >> InLSB;
	Temp4 = 1;
	Temp2 = (Temp4 << (InMSB - InLSB + 1)) - 1;
	Temp3 = Temp1 & Temp2;
	return Temp3;
}

int GetBitsI(int InNum, int InMSB, int InLSB)
{
	uint64_t Temp1, Temp2, Temp3, Temp4;
	Temp1 = InNum >> InLSB;
	Temp4 = 1;
	Temp2 = (Temp4 << (InMSB - InLSB + 1)) - 1;
	Temp3 = Temp1 & Temp2;
	return Temp3;
}

uint64_t GetProtocolBits( uint64_t Address) { return GetBits(Address, 28, 20); }
uint64_t GetPIPBits( uint64_t Address) { return GetBits(Address, 19, 19); }
uint64_t GetDestBits( uint64_t Address) { return GetBits(Address, 18, 11); }
uint64_t GetSrcBits( uint64_t Address) { return GetBits(Address, 10, 3); }
uint64_t GetContBits( uint64_t Address) { return GetBits(Address, 2, 2); }
uint64_t GetMsgTypeBits( uint64_t Data) { return GetBits(Data, 63, 56); }
uint64_t GetErrTypeBits( uint64_t Data) { return GetBits(Data, 55, 48); }
uint64_t GetValTypeBits( uint64_t Data) { return GetBits(Data, 47, 32); }
uint64_t GetValBits( uint64_t Data) { return GetBits(Data, 31, 0); } 

frameid GetFrameId(int Protocol, int SrcAddr, int DestAddr, int Continue )
{ 
	volatile frameid Outbound;
	Outbound.data32 = 0;
	Outbound.msgbit.ProtNo = Protocol;
	Outbound.msgbit.PTP = 1;
	Outbound.msgbit.DstAddr = DestAddr; 
	Outbound.msgbit.SrcAddr = SrcAddr;
	Outbound.msgbit.CNT = Continue;
	Outbound.msgbit.RES1 = 1;
	Outbound.msgbit.RES2 = 1;
	return Outbound;
}

dataframe GetDataFrame(int ErrType, int MsgType, int ValueType, ufloatbit32_t Value)
{
	volatile dataframe Outbound;
	Outbound.data64 = 0;
	Outbound.data.MsgType = MsgType;
	Outbound.data.ErrType = ErrType;
	Outbound.data.ValueType = ValueType;
	Outbound.data.Value = Value.data32;
	if (ErrType != 0xF0)
	{
		Outbound.data.MsgType = Outbound.data.MsgType && 0x80;
	}
	return Outbound;
}

void ParseId(frameid Id, int* Protocol, int* SrcAddr, int* DestAddr)
{
	*Protocol = GetProtocolBits(Id.data32);
	*SrcAddr = GetSrcBits(Id.data32);
	*DestAddr = GetDestBits(Id.data32);
}

void ParseData(dataframe Data, int* IncomingMsgType, int* IncomingValueType, int* IncomingErrType, ufloatbit32_t* IncomingValue)
{
	*IncomingMsgType 	= GetMsgTypeBits(Data.data64);
	*IncomingValueType 	= GetValTypeBits(Data.data64);
	(*IncomingValue).data32 = GetValBits(Data.data64);
 	*IncomingErrType        = GetErrTypeBits(Data.data64);
}


/*

uint64_t SetBitsF( uint64_t InNum, ufloatbit32_t Value, int InMSB, int InLSB)
{
uint64_t Temp1, Temp2, Temp3, Temp4, tempValue;
tempValue = ((Value.intdata[1].id1) << 32) | (Value.intdata[0].id1);
Temp1 = tempValue >> InLSB;
Temp4 = 1;
Temp2 = (Temp4 << (InMSB - InLSB + 1)) - 1;
Temp3 = InNum || Temp2;
return Temp3;
}

uint64_t SetBitsI( uint64_t InNum, int Value, int InMSB, int InLSB)
{
uint64_t Temp1, Temp2, Temp3, Temp4;
Temp1 = ( uint64_t)Value >> InLSB;
Temp4 = 1;
Temp2 = (Temp4 << (InMSB - InLSB + 1)) - 1;
Temp3 = InNum || Temp2;
return Temp3;
}
private UInt64 m_Address;
private UInt64 m_Data;
private UInt64 m_Index;
private DateTime m_DateTime;

public CANMessage(String Address, String Data, UInt64 Index)
{
	String LocalAddress = Address.Replace("0x", "");
	String LocalData = Data.Replace("0x", "");
	bool Result1 = UInt64.TryParse(LocalAddress, NumberStyles.HexNumber, null, out m_Address);
	bool Result2 = UInt64.TryParse(LocalData, NumberStyles.HexNumber, null, out m_Data);
	m_Index = Index;
	m_DateTime = System.DateTime.Now;  // DateTime.Now.ToString("MM/dd HH:mm:ss.fff")
	return;
}

public CANMessage(UInt64 Address, UInt64 Data, UInt64 Index)
{
	m_Address = Address;
	m_Data = Data;
	m_Index = Index;
	m_DateTime = System.DateTime.Now;  // DateTime.Now.ToString("MM/dd HH:mm:ss.fff")
	return;
}

public String GetCSV()
{
	String CSVString = String.Format(
		"{0,9:d9}, {1,18:MM/dd HH:mm:ss.fff}, 0x{2,8:X8}, 0x{3,16:X16}, 0x{4,2:X2}, {5,3}, 0x{6,2:X2}, {7,0}, 0x{8,2:X2}, {9,0}, {10,8:X8}, {11,9}, {12,9}, {13,3}, 0x{14,2:X2}, 0x{15,2:X2}, {16,0}, {17,0}",
		m_Index, m_DateTime, m_Address, m_Data, GetProtocolBits(), GetProtocol(),
		GetMsgTypeBits(), GetMsgType(), GetValTypeBits(), GetValType(), GetVal(), GetValFloat(),
		GetErrTypeBits(), GetErrType(), GetSrcBits(), GetDestBits(),
		GetPIP(), GetCont());
	// _T("%s, %s, 0x%02x %3s, 0x%02x %s, 0x%02x %s, 0x%08x %7s, 0x%02x %s, 0x%02x, 0x%02x, 0x%02x %s, 0x%02x %s"),
	// Address, Data, iProtocol, sProtocol, iMsgType, sMsgType, iValType, sValType, iValue, sValue, iErrType, sErrType, iSrc, iDest, iPIP, sPIP, iCont, sCont );
	return CSVString;
}

public String GetShortString()
{
	String CSVString = String.Format(
		"{0,9:d9} {1,18:MM/dd HH:mm:ss.fff} 0x{2,2:X2} 0x{3,2:X2} {4,8:X8} {5,8:X8} {6,9} 0x{7,2:X2} 0x{8,2:X2}",
		m_Index, m_DateTime, GetProtocol(), GetMsgType(), GetValType(), GetValBits(), GetVal(), GetSrcBits(), GetDestBits());
	return CSVString;
}


public void GetCSVTitle(String TitleString)
{
TitleString = "";
TitleString = TitleString+String.

String.Format("{0,9}, {0,15}, {0,16}, X8, X16, X2, {0,3}, X2, {0,0}, X2, {0,0}, X2, {0,3}, X2, X2, {0,0}, {0,0}",
"Index","DateTime","Address", "Data", "Prot", "ProtType", "Msg", "MsgType", "ValType", "ValDesc", "Err", "ErrDesc", "Src", "Dst", "PIP", "Cont" );
return;
} 

public String GetAddress() { return String.Format("0x{0,8:X8}", m_Address); }
public String GetData() { return String.Format("0x{0,16:X16}", m_Data); }
public String GetIndex() { return String.Format("{0,0:00000000}", m_Index); }
public DateTime GetDateTimeBits() { return m_DateTime; }
public String GetDateTime() { return String.Format("{0,18:MM/dd HH:mm:ss.fff}", m_DateTime); }


public String GetProtocol()
{
	UInt64 iVal = GetProtocolBits();
	switch (iVal)
	{
	case 0x60: return ("edr");
	case 0x70: return ("pri");
	case 0xA4: return ("BBU");
	case 0xA5: return ("*SM");
	default: return ("  ");
	} // switch
}


public String GetPIP()
{
	UInt64 iVal = GetPIPBits();
	switch (iVal)
	{
	case 0x00: return ("Broadcast ");
	case 0x01: return ("Point     ");
	default: return ("          ");
	} // switch
}


public String GetDest()
{
	UInt64 iVal = GetDestBits(); return iVal.ToString("X2");
}

public String GetSrc()
{
	UInt64 iVal = GetSrcBits(); return iVal.ToString("X2");
}

public String GetCont()
{
	UInt64 iVal = GetContBits();
	switch (iVal)
	{
	case 0x00: return ("Singl");
	case 0x01: return ("Cont ");
	default: return ("     ");
	} // switch   
}

public String GetErrType()
{
	UInt64 iVal = GetErrTypeBits();
	switch (iVal)
	{
	case 0xF0: return ("Ok  ");
	case 0xF2: return ("ERR ");
	case 0xF4: return ("addr");
	default: return ("----");
	} // switch
}

public String GetMsgType()
{
	UInt64 iVal = GetMsgTypeBits();
	if (GetProtocol() == "*SM")
	{
		switch (iVal)
		{
		case 0x00: return ("Req setup     ");
		case 0x10: return ("Req setup data");
		case 0x20: return ("Req setup data");
		case 0x40: return ("Data Response ");
		case 0x41: return ("Data Response ");
		case 0x42: return ("Data Response ");
		case 0x24: return ("Current 01-10 ");
		case 0x25: return ("Current 11-20 ");
		case 0x26: return ("Current 20-25 ");
		case 0x50: return ("Current 26-30 ");
		case 0x51: return ("Current 31-40 ");
		case 0x36: return ("Hall    01-10 ");
		case 0x37: return ("Hall    11-20 ");
		case 0x58: return ("Hall    21-30 ");
		case 0x59: return ("Hall    30-40 ");
		default: return   ("              ");
		} // switch
	} // if 
	else if (GetProtocol() == "edr")
	{
		switch (iVal)
		{
		case 0x00: return ("Req setup     ");
		case 0x10: return ("Req setup data");
		case 0x20: return ("Req setup data");
		case 0x01: return ("Req data vals ");
		case 0x41: return ("Resp data vals");
		case 0x02: return ("Req data bits ");
		case 0x42: return ("Resp data bits");
		case 0x03: return ("Setup data    ");
		case 0x43: return ("Setup resp    ");
		default: return   ("              ");
		} // switch
	} // if 
	else return ("              ");
}

public string GetValueTypeNum() { UInt64 iVal = GetValTypeBits(); return iVal.ToString("X3"); }
public String GetValType()
{
	UInt64 iVal = GetValTypeBits();
	if (GetProtocol() == "*SM")
	{
		switch (iVal)
		{
		case 0x040: return ("Status        ");
		case 0x054: return ("SN Low        ");
		case 0x055: return ("SN High       ");
		case 0x056: return ("Version       ");
		case 0x058: return ("Runtime       ");
		case 0x05A: return ("Bar Code 1    ");
		case 0x05B: return ("Bar Code 2    ");
		case 0x05C: return ("Bar Code 3    ");
		case 0x05D: return ("Bar Code 4    ");
		default:
		{
			if ((iVal >= 0x100) && (iVal <= 0x118))
			{
				UInt64 temp = iVal - 0x100 + 1; return "Current   " + temp.ToString("D2") + "    ";
			}
			else if ((iVal >= 0x1b0) && (iVal <= 0x1be))
			{
				UInt64 temp = iVal - 0x1b0 + 26; return "Current   " + temp.ToString("D2") + "    ";
			}
			else if ((iVal >= 0x197) && (iVal <= 0x1af))
			{
				UInt64 temp = iVal - 0x197 + 1; return "Hall      " + temp.ToString("D2") + "    ";
			}
			else if ((iVal >= 0x1ec) && (iVal <= 0x1fa))
			{
				UInt64 temp = iVal - 0x1ec + 26; return "Hall      " + temp.ToString("D2") + "    ";
			}
			return ("unknown       ");
		}
		} // switch
	} //	IF
	else return ("unknown       ");
} // function

unsafe public float GetValFloat() { UInt64 iVal = GetValBits(); float fVal = *((float*)&iVal); return fVal; }

public String GetVal() { UInt64 iVal = GetValBits(); return "0x" + iVal.ToString("X8"); }

unsafe public String GetValFloatString()
{
	UInt64 iVal = GetValBits();
	float fVal = *((float*)&iVal);
	String FloatString = fVal.ToString("F3");
	if (FloatString.Length > 8) return ""; else return FloatString;
}
}
*/

/*****************************************************************************!
 * Function : CanRegSetFromString
 *****************************************************************************/
void
CanRegSetFromString
(CanReg* InCanReg, string InValueString)
{
  bool					b;

  if ( InCanReg->registerDef->formatType == 0 ) {
    InCanReg->Value.fd = GetFloatValueFromString(&b, InValueString);
  } else if ( InCanReg->registerDef->formatType == 1 ) {
    memcpy(&(InCanReg->Value.data32), InValueString, 3);
  } else if ( InCanReg->registerDef->formatType == 2 ) {
    InCanReg->Value.data32 = GetIntValueFromString(&b, InValueString);
  } else if ( InCanReg->registerDef->formatType == 3 ) {
    memcpy(&(InCanReg->Value.data32), InValueString, 4);
  } else if ( InCanReg->registerDef->formatType == 4 ) {
    InCanReg->Value.data32 = GetHex32ValueFromString(NULL, InValueString);
  }
}

/*****************************************************************************!
 * Function : CanRegSetFromJSONString
 *****************************************************************************/
void
CanRegSetFromJSONString
(CanReg* InCanReg, json_value* InValue, string InTagName)
{
  string                                s;
  json_value*							jsonValue;
  if ( InCanReg->registerDef->formatType == 0 ) {
	jsonValue = JSONIFGetValue(InValue, InTagName);
	if ( jsonValue->type == json_string ) {
	  CanRegSetFromString(InCanReg, jsonValue->u.string.ptr);
	} else {
      InCanReg->Value.fd = JSONIFGetFloat(InValue, InTagName);
	}
  } else if ( InCanReg->registerDef->formatType == 1 ) {
	s = JSONIFGetString(InValue, InTagName);
    memcpy(&(InCanReg->Value.data32), s, 3);
	FreeMemory(s);
  } else if ( InCanReg->registerDef->formatType == 2 ) {
	if ( InValue->type == json_string ) {
	  s = JSONIFGetString(InValue, InTagName);
	  CanRegSetFromString(InCanReg, s);
	  FreeMemory(s);
	} else {
      InCanReg->Value.data32 = JSONIFGetInt(InValue, InTagName);
	}
  } else if ( InCanReg->registerDef->formatType == 3 ) {
	s = JSONIFGetString(InValue, InTagName);
    memcpy(&(InCanReg->Value.data32), s, 4);
	FreeMemory(s);
  } else if ( InCanReg->registerDef->formatType == 4 ) {
	s = JSONIFGetString(InValue, InTagName);
    InCanReg->Value.data32 = GetHex32ValueFromString(NULL, s);
	FreeMemory(s);
  }
}

/*****************************************************************************!
 * Function : CanRegGetFromString
 *****************************************************************************/
string
CanRegGetFromString
(string InValueString, CanReg* InCanReg)
{
  // Normal floating point
  if ( InCanReg->registerDef->formatType == 0 ) {
    sprintf(InValueString, "%.03f", InCanReg->Value.fd);

  // Hardware Version
  } else if ( InCanReg->registerDef->formatType == 1 ) {
    sprintf(InValueString, "%c%c%c", InCanReg->Value.data8[3], InCanReg->Value.data8[2], InCanReg->Value.data8[1]);

  // Serial Number
  } else if ( InCanReg->registerDef->formatType == 2 ) {
    sprintf(InValueString, "%u", InCanReg->Value.data32);      

  // 4 ASCII characters
  } else if ( InCanReg->registerDef->formatType == 3 ) {
    sprintf(InValueString, "\"%c%c%c%c\"", InCanReg->Value.data8[3], InCanReg->Value.data8[2], InCanReg->Value.data8[1], InCanReg->Value.data8[0]);

  // Hex characters
  } else if ( InCanReg->registerDef->formatType == 4 ) {
    sprintf(InValueString, "\"%08X\"", InCanReg->Value.data32);
  }
  return InValueString;
}

