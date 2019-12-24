#include "Fan.h"
Fan::Fan(int id, Zone zone, NodeEnm node):Device(id, devtype_fan, zone){
  this->node=node;
};

//turn fan on
void Fan::turnOn(){
  if(this->_onSpeed==0) this->_onSpeed=3; //default speed
  this->speed=this->_onSpeed;
  TaskManager::getInstance()->addTask(command_rf_send, this->id, this->speed);
};        

//turn fan off
void Fan::turnOff(){
  this->_onSpeed=this->speed;
  TaskManager::getInstance()->addTask(command_rf_send, this->id, 0);
};       

//set fan speed (0 to turn off)
void Fan::setSpeed(int speed){
  this->speed=speed;
  this->_onSpeed=this->speed;
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
