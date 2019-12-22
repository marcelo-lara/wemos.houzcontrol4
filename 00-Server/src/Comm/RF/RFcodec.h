#include "Arduino.h"

struct Packet {
public:
	u32  raw;
	bool hasData;

	u8	 id;
	u32  payload;

	byte node;
	u8	 cmd;
	u8	 media;

	String message;
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