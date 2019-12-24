#include "Arduino.h"
#include "wemos.setup.h"

// HouzControl4 modules
#include "src/Core/HausCore.h"
Devices *devices = devices->getInstance();
//SceneManager sceneManager;

//RF link
#include <RFlink.h>
void onRFrxCallback(Packet devicePacket);
RFlink rfLink(onRFrxCallback);

//API server
#include "WebServer.h"
WebServer webServer;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){
  wemosWiFi.connect("hauskontrol");
  webServer.setup();
  rfLink.setup();
  Serial.println("-- setup complete ---------------");
};

void loop(){
  wemosWiFi.update();
  rfLink.update();
  if(TaskManager::getInstance()->arePendingTasks()) runTask(); 
};

void runTask(){
    Task task = TaskManager::getInstance()->getNextTask();
    Serial.print("TASK| ");
    switch (task.command){
    case command_set_device: 
        Serial.println("command_set_device");
        break;

    case command_rf_send: 
        Serial.println("TASK| command_rf_send");
        break;

    case command_play_scene: 
        Serial.println("TASK| command_play_scene");
        break;
    
    default:
        break;
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RF hook
void onRFrxCallback(Packet pkt){
  Serial.println("::RFrxCallback");
};
