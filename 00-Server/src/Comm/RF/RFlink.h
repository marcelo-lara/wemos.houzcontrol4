#pragma once
#include "Arduino.h"
#include "../../Core/HouzModel.h"
#include "../../Core/TaskManager.h"
#include "../../Data/devs.h"
#include "RFsettings.h"
#include "RFcodec.h"


// commands
#define RFCMD_QUERY			0xA
#define RFCMD_VALUE			0xB
#define RFCMD_SET			0xC
#define RFCMD_EVENT			0xD
#define RFCMD_STATUS		0xE


struct DevicePkt: Device {
    int cmd;
};

typedef void (*RFrxCallback)(deviceData _deviceData);

class RFlink{
public:
    RFlink(TaskManager *_taskManager, RFrxCallback rxEvent);
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
