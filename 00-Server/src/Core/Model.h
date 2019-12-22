#pragma once
#include "Arduino.h"




///////////////////////////////////////////////////////////////////////////
// DEVICE
enum DeviceType {
  devtype_light = 1,
  devtype_env   = 4,
  devtype_fan   = 2,
  devtype_ac    = 3,
  
};

struct Device {
public:
  u8  id;       //channel
  u8  node;     //node
  u32 payload;  //value
  u8  type;     //device type
};

struct Environment: Device {
  bool  on;     //sensor is on
  float temp;   //temperature
  float hum;    //humidity
  float press;  //pressure
  float light;  //light level
  
};

struct Light: Device {
  bool on;      //light status
  u8   muxId;   //id of multpiplexed out
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