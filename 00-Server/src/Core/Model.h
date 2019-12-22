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
  zone_living  = 1, 
  zone_suite   = 2,   
  zone_office  = 3,  
  zone_kitchen = 4, 
  zone_door    = 5 
};

struct Device {
public:
  u8  id;       //channel
  u8  node;     //phisical node
  u32 payload;  //value
  
  DeviceType  type; //device type
  Zone        zone; //associated room
  String      name; //device name
};

struct Environment: Device {
  bool  on;     //sensor is on
  float temp;   //temperature
  float hum;    //humidity
  float press;  //pressure
  float light;  //light level

  u8    chTemp; //temperature channel
  u8    chHum;  //humidity
  u8    chPress;//pressure
  u8    chLight;//light level
  long  ts;     //sampled data timestamp
};

struct Light: Device {
  Light();
  Light (u8 _muxId, u8 _mucPos){};
protected:
   bool on;      //light status
   bool isMux;   //light is behind a multpiplexer
public:
  void turnOn();
  void turnOff();
private:
  u8   muxId;    //id of multpiplexed out
  u8   muxPos;   //id of multpiplexed out
};


struct Fan: Device {
  bool on;      //fan is on, if fan is off speed is 0
  u8   speed;   //when on, fan speed
};

struct Ac: Device {
  bool on;      //ac is on
  bool fanOn;   //ac fan is on
  u8   temp;    //ac temperature
};

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