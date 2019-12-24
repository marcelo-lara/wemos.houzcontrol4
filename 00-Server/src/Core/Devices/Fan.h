#pragma once
#include "Arduino.h"
#include "../Model.h"
#include "../TaskManager.h"

//ceiling fan
class Fan : public Device {
public:  
  Fan(int id, Zone zone, NodeEnm node);
  NodeEnm node = node_server; //target node
  String toJson();

  void turnOn();
  void turnOff();
  void setSpeed(int speed);

  bool on      = 0; 
  int  speed   = 0; 

private:
  //default on speed
  int _onSpeed  = 3;
};