#include "Arduino.h"
#include "wemos.setup.h"

// HouzControl4 modules
#include "src/Core/HausCore.h"
Devices *devices = devices->getInstance();
SceneManager sceneManager;

//RF link
#include "src/Comm/RF/RFlink.h"
void onRFrxCallback(deviceData devicePacket);
RFlink rfLink(onRFrxCallback);

//API server
#include "WebServer.h"
WebServer webServer;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){
    wemosWiFi.connect("hauskontrol");
    webServer.setup();
    rfLink.setup();
    devices->set(server_rf, rfLink.ready);
    Serial.println("-- setup complete ---------------");
};

void loop(){
    wemosWiFi.update();
    rfLink.update();
    if(TaskManager::getInstance()->arePendingTasks()) runTask(); 
};

void runTask(){
    Task task = TaskManager::getInstance()->getNextTask();
    Serial.print("task> ");
    switch (task.command){
    case command_set_device: 
        Serial.println("command_set_device");
        break;

    case command_rf_send: 
        Serial.println("command_rf_send");
        DevicePkt pkt;
        pkt.id=task.device.id;
        pkt.cmd=RFCMD_QUERY;
        pkt.node=task.device.node;
        pkt.payload=task.device.payload;
        rfLink.send(pkt);
        break;

    case command_play_scene: 
        Serial.println("command_play_scene");
        sceneManager.play(task.device.payload);
        break;
    
    default:
        break;
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RF hook
void onRFrxCallback(deviceData dev){
  devices->set(dev.id, dev.payload);
};



