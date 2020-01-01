#pragma once
#include "Arduino.h"
#include "../Model.h"
#include "../TaskManager.h"

//ceiling fan
class Fan : public Device {
public:  
  Fan(int id, Zone zone, NodeEnm node);
  String toJson();

  void turnOn();
  void turnOff();
  void setSpeed(int speed);

  bool on      = 0; 
  int  speed   = 3; //default speed

  void update();
  void decode(long _payload);
  void setOn(bool _on);//device ext
  void set(long _payload);//device ext

private:
  //default on speed
  int _onSpeed  = 3;
};