#pragma once
#include "Arduino.h"

///////////////////////////////////////////////////////////////////////////
// DEVICE

//Device Type
enum DeviceType {
  devtype_light = 1, //light (use initializer to multiplex)
  devtype_env   = 2, //Environment
  devtype_fan   = 3, //fan 
  devtype_ac    = 4, //air conditioner
  devtype_bell  = 5  //door bell
};

//Zone where device is located
enum Zone {
  zone_undefined=0,
  zone_living  = 1, 
  zone_suite   = 2,   
  zone_office  = 3,  
  zone_kitchen = 4, 
  zone_door    = 5,
  zone_outside = 6
};

enum NodeEnm{
  node_server  = 0,
  node_office  = 1,
  node_suite   = 2,
  node_living  = 3,
  node_kitchen = 4
};

class Device {
public:
  Device(){};
  Device(int _id, DeviceType _type){
    this->id=_id;
    this->type=_type;
  };
  DeviceType type;    //device type
  u8         id;      //channel
  u32        payload; //value
  Zone       zone;    //device location
};

class DeviceData : Device{
  u8      id;
  u8      node; //phisical node
  Zone    zone; //associated room
  String  name; //device name
};

// class Environment: Device {
// public:
//   Environment(int _chTemp = 0, int _chHum = 0, int _chPress = 0, int _chLight = 0);
//   void update();

//   bool  on;     //sensor is on
//   float temp;   //temperature
//   float hum;    //humidity
//   float press;  //pressure
//   float light;  //light level

// private:
//   u8    chTemp; //temperature channel
//   u8    chHum;  //humidity
//   u8    chPress;//pressure
//   u8    chLight;//light level
//   long  ts;     //sampled data timestamp
// };

// class Light: public Device {
// public:
//   Light(){};
//   Light(u8 _muxCh, u8 _muxPos);
//   Light(u8 _gpio);
//   Light(NodeEnm _node);

//   void turnOn();
//   void turnOff();
//   void set(bool _on);

//   operator Device(){return Device(1, devtype_light);};

// protected:
//   bool on;    //light status
//   bool isMux;   //light is behind a multpiplexer

// private:
//   u8   muxCh;    //channel of multpiplexer
//   u8   muxPos;   //bit behind multpiplexer
// };

// class Fan: Device {
// protected:
//   bool on;      //fan is on, if fan is off speed is 0
//   u8   speed;   //when on, fan speed
// public:
//   void setSpeed(int _speed);
//   void turnOn();
//   void turnOff();
//   void set(bool _on);
// };

// enum AcMode{
//   acmode_cool,
//   acmode_heat
// };

// class Ac: Device {
// protected:
//   bool on;      //ac is on
//   bool fan;     //fan is on
//   bool swing;   //swing is on
//   AcMode mode;  //ac mode (cool/heat)
//   int   temp;    //ac temperature
// };

///////////////////////////////////////////////////////////////////////////
// TASKS

// Task Command
enum Command{
  command_set_device  =  1,
  command_play_scene  =  2,
  command_set_scene   =  3,
  command_rf_send     =  4, 
  command_ir_send     =  5,
  command_scene_pause =  9,
  command_set_on      = 10
};

// Task
struct Task {
public:
  Command command;
  Device  device;
};



///////////////////////////////////////////////////////////////////////////
// SCENES


// fixed scenes
#define scene_unknown 0
#define scene_sleep   1
#define scene_goobye  2
#define scene_hello   3

class Scene {
public:
  int  id;
  Scene(int cueLenght): capacity(cueLenght) {_cue = new Task[cueLenght];}
  
private:
  const int capacity;
  Task *_cue;
};