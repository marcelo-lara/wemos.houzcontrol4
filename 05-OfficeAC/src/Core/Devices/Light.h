#pragma once
#include "Arduino.h"
#include "../Model.h"
#include "../TaskManager.h"

class Light: public Device {
public:
  Light(int id);
  Light(int id, u8 _muxCh, u8 _muxPos);
  Light(int id, u8 _gpio);

  Light(int id, Zone zone, NodeEnm _node);
  Light(int id, Zone zone, NodeEnm _node, u8 _muxCh, u8 _muxPos);

  TaskManager *taskManager = TaskManager::getInstance();
  String toJson();

  bool on      = false; //light status
  bool isMux   = false; //light is behind a multpiplexer
  bool isLocal = false; //light is connected or remote

  void turnOn();        //turn light on
  void turnOff();       //turn light off
  void setOn(bool _on);   //turn light on/off
  void set(long payload);//device ext
  bool isOn();

  void update();
  void decode(long _payload);

  u8   muxCh  = 0;      //channel of multpiplexer
  u8   muxPos = 0;      //bit behind multpiplexer
private:
  u8   gpio   = 0;      //single channel IO
};