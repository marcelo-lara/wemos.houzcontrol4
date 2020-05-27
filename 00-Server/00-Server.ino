#include "Arduino.h"
#include "wemos.setup.h"

// HouzControl4 modules
#include "src/Core/HausCore.h"
Devices *devices = devices->getInstance();
SceneManager sceneManager;

//RF link
#include <RFlink.h>
void onRFrxCallback(Packet devicePacket);
void onRFfailCallback(Packet devicePacket);
static RFlink rfLink(onRFrxCallback, onRFfailCallback);

//Node manager
#include "src/Server/RfNodes.h"
RfNodes rfNodes(&rfLink);

//API server
#include "src/Server/WebServer.h"
WebServer webServer;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){
  wemosWiFi.connect("hauskontrol");
  webServer.setup();
  rfLink.setup();
  rfNodes.setup();
  Serial.println("-- setup complete ---------------");

  Packet pkt;
  pkt.id=living_dicroLight;
  pkt.payload=0;
  rfNodes.parsePacket(pkt);

};

void loop(){
  wemosWiFi.update();
  rfLink.update();
  rfNodes.update();
  sceneManager.update();
  if(TaskManager::getInstance()->arePendingTasks()) runTask(); 
};

void runTask(){
  Task task = TaskManager::getInstance()->getNextTask();
  switch (task.command){
 
  case command_update_device:
    devices->toJsonVoid();
    devices->get(task.device.id)->decode(task.device.payload);
    break;

  case command_set_device:
    devices->get(task.device.id)->set(task.device.payload);
    break;

  case command_set_on:
    devices->get(task.device.id)->setOn(task.device.payload==0?false:true);
    break;

  // RF related /////////////////////////////////////////////////
  case command_rf_send: 
    rfLink.send(Packet(task.device.id, RFCMD_SET, task.device.payload, task.device.node));
    break;
  case command_rf_query: 
    rfLink.send(Packet(task.device.id, RFCMD_QUERY, 0, task.device.node));
    break;

  // Scene Cue //////////////////////////////////////////////////////
  case command_play_scene: 
    Serial.println("TASK-command_play_scene");
    sceneManager.play(task.device.payload);
    break;

  // Custom actions /////////////////////////////////////////////////
  case command_fan_on: 
    {Device* dev = devices->get(task.device.id);
    if(dev || dev->type==devtype_fan) ((Fan*)dev)->setOn(task.device.payload==0?false:true);}
    break;
  case command_fan_speed: 
    {Device* dev = devices->get(task.device.id);
    if(dev || dev->type==devtype_fan) ((Fan*)dev)->setSpeed(task.device.payload);
    else {Serial.printf("command_fan_speed> unknown device 0x%X\n", task.device.id);};}
    break;

  //muxed lights
  case command_send_to_mux:{
    Device* dev = devices->get(task.device.id);
    if(dev || dev->type==devtype_light){
      int muxId = ((Light*)dev)->muxCh;
      int muxPos = ((Light*)dev)->muxPos;
      bool on = ((Light*)dev)->on;
      rfNodes.sendToMux(muxId, muxPos, on);
    }}

    break;

  default:
    Serial.printf("unknown command %i\n", task.command);
    break;
  }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RF hook
void onRFrxCallback(Packet pkt){
  rfNodes.parsePacket(pkt); //delegate rfNodes
};
void onRFfailCallback(Packet pkt){
  Serial.println("->onRFfailCallback");
};

