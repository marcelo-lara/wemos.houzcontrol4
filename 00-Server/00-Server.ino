#include "Arduino.h"
#include "wemos.setup.h"

// HouzControl4 modules
#include "src/Core/HausCore.h"
Devices *devices = devices->getInstance();
//SceneManager sceneManager;

//RF link
#include <RFlink.h>
void onRFrxCallback(Packet devicePacket);
static RFlink rfLink(onRFrxCallback);

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
  
  // Light* d = static_cast<Light*>(devices->list[0]); 
  // d->turnOff();


};

void loop(){
  wemosWiFi.update();
  rfLink.update();
  rfNodes.update();
  if(TaskManager::getInstance()->arePendingTasks()) runTask(); 
};

void runTask(){
  Task task = TaskManager::getInstance()->getNextTask();
  Serial.print("TASK| ");
  switch (task.command){

  
  case command_update_device:
    Serial.printf("command_update_device 0x%2X->0x%4X\n",task.device.id, task.device.payload);
    devices->get(task.device.id)->update(task.device.payload);
    break;

  case command_set_device:
    Serial.printf("command_set_device 0x%2X->0x%4X\n",task.device.id, task.device.payload);
    devices->get(task.device.id)->set(task.device.payload);
    break;

  case command_set_on:
    Serial.printf("command_set_on 0x%2X->0x%4X\n",task.device.id, task.device.payload);
    devices->get(task.device.id)->setOn(task.device.payload==0?false:true);
    break;

  // RF related /////////////////////////////////////////////////
  case command_rf_send: 
    Serial.printf("command_rf_send %i-0x%2X->0x%4X\n",task.device.node, task.device.id, task.device.payload);
    rfLink.send(Packet(task.device.id, RFCMD_SET, task.device.payload, task.device.node));
    break;
  case command_rf_query: 
    Serial.printf("command_rf_query %i-0x%2X\n",task.device.node, task.device.id);
    rfLink.send(Packet(task.device.id, RFCMD_QUERY, 0, task.device.node));
    break;

  // Scene Cue /////////////////////////////////////////////////
  case command_play_scene: 
    Serial.println("command_play_scene");
    break;

  // Custom actions /////////////////////////////////////////////////
  case command_fan_on: 
    Serial.printf("command_fan_on 0x%2X->0x%4X\n",task.device.id, task.device.payload);
    {Device* dev = devices->get(task.device.id);
    if(dev || dev->type==devtype_fan) ((Fan*)dev)->setOn(task.device.payload==0?false:true);}
    break;
  case command_fan_speed: 
    Serial.printf("command_fan_speed 0x%2X->0x%4X\n",task.device.id, task.device.payload);
    {Device* dev = devices->get(task.device.id);
    if(dev || dev->type==devtype_fan) ((Fan*)dev)->setSpeed(task.device.payload);
    else {Serial.printf("command_fan_speed> unknown device 0x%2X\n", task.device.id);};}
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
