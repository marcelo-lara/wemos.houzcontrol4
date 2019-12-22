#include "Arduino.h"

//RF packet
struct Packet {
public:
	u8	 id;      //channel id
	u8	 cmd;     //rfcmd enum
	u32  payload; //32b payload

	byte node;    //rf station id
	u32  raw;     //encoded packet
};

class RFcodec{
    public:
        unsigned long encode(int _cmd, int deviceId, unsigned long devicePayload);
    	  Packet decode(unsigned long rawData, unsigned long rfNodeStation);
        Packet decode(String str);


        unsigned long StrToHex(char str[]);
        unsigned long StrToHex(String str);
        
        unsigned long pressureEncode(float devicePayload);
        float pressureDecode(unsigned long devicePayload);
        
    private:
};