#pragma once
#include "Arduino.h"
#include "../../Core/HausCore.h"
#include "RFsettings.h"
#include "RFcodec.h"
#include "Channels.h"

struct DevicePkt: Device {
    int cmd;
};

typedef void (*RFrxCallback)(Packet _Packet);

class RFlink{
public:
    RFlink(RFrxCallback rxEvent);
    void setup();
    void update(); //decode received packets into tasks + raise RF
    bool ready;
    
    //tx commands
    bool send(DevicePkt dev);

    //rx section


private:
    unsigned long deviceEncode(DevicePkt dev);
    TaskManager *taskManager; //master task manager
    RFrxCallback raiseRxEvent;
};
