#include "Fan.h"
Fan::Fan(int id, Zone zone, NodeEnm node):Device(id, devtype_fan, zone){
  this->node=node;
};

void Fan::set(long payload){
}

void Fan::setOn(bool _on){
  if(_on){
    turnOn();
  }else{
    turnOff();
  };
};
//turn fan on
void Fan::turnOn(){
  if(this->speed==0) this->speed=this->_onSpeed; //default speed
  TaskManager::getInstance()->addTask(command_rf_send, this->id, this->speed);
};        

//turn fan off
void Fan::turnOff(){
  if(this->on)
    this->_onSpeed=this->speed;
  TaskManager::getInstance()->addTask(command_rf_send, this->id, 0);
};       

//set fan speed
void Fan::setSpeed(int speed){
  this->speed=speed;
  this->_onSpeed=this->speed;
  if(this->on)
    TaskManager::getInstance()->addTask(command_rf_send, this->id, this->speed);
};


//get fan json string
String Fan::toJson(){
  String json = "{\"id\":";
  json += this->id;
  json += ", \"type\":";
  json += this->type;
  json += ", \"val\":";
  json += this->on?1:0;
  json += ", \"on\":";
  json += this->on?1:0;
  json += ", \"speed\":";
  json += this->speed;
  if(this->zone>0){
    json += ", \"zone\":";
    json += this->zone;
  };
  json += "}";
  return json;
};

////////////////////////////////////////////////////////////////////////////////////////
void Fan::update(){
    TaskManager::getInstance()->addTask(command_rf_query, this->id, 0);
};
void Fan::update(long _payload){
  this->on=(_payload!=0);
  if(_payload>0) this->speed=_payload;
  Serial.printf("[%i]Fan | on:%i | speed: %i\n", this->id, this->on, this->speed);
};