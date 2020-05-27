#include "Light.h"

Light::Light(int id){
  this->id=id;
  this->payload=0;
  this->type=devtype_light;
};

//local light
Light::Light(int id, u8 _muxCh, u8 _muxPos):Light(id){
  this->isMux=true;
  this->isLocal=true;
  this->muxCh=_muxCh;
  this->muxPos=_muxPos;
};
Light::Light(int id, u8 _gpio):Light(id){
  this->isLocal=true;
  this->gpio=_gpio;
};

// remote light
Light::Light(int id, Zone zone, NodeEnm _node):Device(id,devtype_light,zone,_node){};
Light::Light(int id, Zone zone, NodeEnm _node, u8 _muxCh, u8 _muxPos):Device(id,devtype_light,zone,_node){
  this->isMux=true;
  this->muxCh=_muxCh;
  this->muxPos=_muxPos;
};

//////////////////////////////////////////////////////////////////////
// Actions

//light as jsonString
String Light::toJson(){
  String json = "{";
  json += "\"id\":";
  json += this->id;
  json += ", \"type\":";
  json += this->type;
  json += ", \"on\":";
  json += this->on?1:0;
  json += ", \"val\":";
  json += this->on?1:0;
  if(this->zone>0){
    json += ", \"zone\":";
    json += this->zone;
  };

  json += "}";
  return json;
};

//////////////////////////////////////////////////////////////////////
// Actions
bool Light::isOn(){return this->on;};
void Light::turnOn(){this->setOn(true);};
void Light::turnOff(){this->setOn(false);};

//set physical device to state
void Light::setOn(bool _on){
  this->on=_on;
  if(this->isLocal){
    Serial.println("setLocalLight()");
    return;
  };
  
  //send command to node
  if(this->node>0){
    if(this->isMux){
      taskManager->addTask(command_send_to_mux, this->id, this->on?1:0, this->node);
    }else{
      taskManager->addTask(command_rf_send, this->id, this->on?1:0, this->node);
    };
  };
};

//set light status
void Light::set(long payload){
  this->setOn(payload!=0);
};

//request to query device
void Light::update(){
  if(!this->isLocal)
    taskManager->addTask(command_request_update, (muxCh>0)?muxCh:id, 0, this->node);
};

//update device values with received payload
void Light::decode(long _payload){
  this->on=(_payload!=0);
};