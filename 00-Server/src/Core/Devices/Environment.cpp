#include "Environment.h"

Environment::Environment(int id, Zone zone, NodeEnm node, int chTemp, int chHum, int chPress, int chLight):Device(id, devtype_env, zone){
  this->node=node;
  this->chHum=chHum;
  this->chTemp=chTemp;
  this->chPress=chPress;
  this->chLight=chLight;
};
void Environment::set(long payload){};


String Environment::toJson(){
  String json = "{\"id\":";
  json += this->id;
  json += ", \"type\":";
  json += this->type;
  json += ", \"val\":";
  json += this->on?1:0;
  json += ", \"on\":";
  json += this->on?1:0;
  json += ", \"temp\":";
  json += this->temp;
  json += ", \"humidity\":";
  json += this->hum;
  
  if(this->press>0){
    json += ", \"pressure\":";
    json += this->press;
  };

  if(this->light>0){
    json += ", \"light\":";
    json += this->light;
  };  
  if(this->zone>0){
    json += ", \"zone\":";
    json += this->zone;
  };
  json += "}";
  return json;
};

//update environment values request
void Environment::update(){
  Serial.println("Environment::update()");
};

void Environment::update(long _payload){
  Serial.printf("[%i]Environment::update(payload:%l)\n", this->id, _payload);
};

void Environment::update(EnvironmentParam param, float val){
  switch (param){
  case envenm_temp:  this->temp=val;  break;
  case envenm_hum:   this->hum=val;   break;
  case envenm_press: this->press=val; break;
  case envenm_light: this->light=val; break;
  default: return; break;}  
  this->on=true;
  this->ts=millis();
};
