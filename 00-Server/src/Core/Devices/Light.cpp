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
Light::Light(int id, NodeEnm _node): Light(id, zone_undefined, _node){
};
Light::Light(int id, NodeEnm _node, u8 _muxCh, u8 _muxPos):Light(id, _muxCh, _muxPos){
  this->node=_node;
};
Light::Light(int id, Zone zone, NodeEnm _node):Light(id){
  this->zone=zone;
  this->node=_node;
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
void Light::turnOn(){this->set(true);};
void Light::turnOff(){this->set(false);};
void Light::set(bool _on){
  this->on=_on;
  if(this->isLocal){
    Serial.println("setLocalLight()");
  };
  if(this->node>0){
    Serial.println("sendCommandToNode()");
    taskManager->addTask(command_rf_send, this->id, this->on?1:0);
  };
};