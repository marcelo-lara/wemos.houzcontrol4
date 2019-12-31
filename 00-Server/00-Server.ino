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

  case command_set_device:
    Serial.println("command_set_device");
    {Device* dev = devices->get(task.device.id);
    if(dev) {dev->set(task.device.payload);}
    else {Serial.printf("command_set_device> unknown device 0x%2X\n", task.device.id);};}
    break;

  case command_rf_send: 
    Serial.println("command_rf_send");
    rfLink.send(Packet(task.device.id, RFCMD_SET, task.device.payload, task.device.node));
    break;

  case command_fan_off: 
    {Device* dev = devices->get(task.device.id);
    if(dev || dev->type==devtype_fan) ((Fan*)dev)->turnOff();}
    break;
  case command_fan_on: 
    {Device* dev = devices->get(task.device.id);
    if(dev || dev->type==devtype_fan) ((Fan*)dev)->turnOn();}
    break;
  case command_fan_speed: 
    {Device* dev = devices->get(task.device.id);
    if(dev || dev->type==devtype_fan) ((Fan*)dev)->setSpeed(task.device.payload);
    else {Serial.printf("command_fan_speed> unknown device 0x%2X\n", task.device.id);};}
    break;

  case command_play_scene: 
    Serial.println("command_play_scene");
    break;

  case command_rf_query: 
    Serial.println("command_rf_query");
    rfLink.send(Packet(task.device.id, RFCMD_QUERY, 0, task.device.node));
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
