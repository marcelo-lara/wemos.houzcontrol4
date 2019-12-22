#pragma once
#include "Arduino.h"
#include "RFcodec.h"


unsigned long RFcodec::encode(int _cmd, int deviceId, unsigned long devicePayload)
{
	unsigned long retVal = 0xD;
	retVal = (retVal << 4) + _cmd;
	retVal = (retVal << 8) + deviceId;
	retVal = (retVal << 16) + devicePayload;
	return retVal;
}
Packet RFcodec::decode(unsigned long rawData, unsigned long nodeId) {
	Packet decoded;
	decoded.raw = rawData;

	if (((rawData >> 28) == 0xD)) {
		decoded.node = nodeId;

		//parse values
		decoded.cmd = ((rawData >> 24) & 0x0F);
		decoded.id = ((rawData >> 16) & 0x0FF);
		decoded.payload = ((rawData) & 0x0000FFFF);
	}
	return decoded;
};

Packet RFcodec::decode(String str) { //from serial
	Packet dev;
	if (str.length() != 10 || str[0] != 'N' || str[2] != 'D')
		return dev;

	dev.node = StrToHex(str.substring(1, 2));
	dev.cmd = StrToHex(str.substring(3, 4));
	dev.id = StrToHex(str.substring(4, 6));
	dev.payload = StrToHex(str.substring(6, 10));
	dev.raw = StrToHex(str.substring(3, 10));
	return dev;
}

//////////////////////////////////////////////////////
// Pressure patch
unsigned long RFcodec::pressureEncode(float devicePayload){
	unsigned long ret = (devicePayload-900)*100; //900hPa offset
  if(ret>0xFFFF) ret=0xFFFF;
	return ret;
}

float RFcodec::pressureDecode(unsigned long devicePayload){
  return (devicePayload/100)+900; //900hPa offset
}


//////////////////////////////////////////////////////
// StrToHex
unsigned long RFcodec::StrToHex(char str[])
{
  return (long)strtol(str, 0, 16);
};
unsigned long RFcodec::StrToHex(String str)
{
  return (long)strtol(str.c_str(), 0, 16);
};
