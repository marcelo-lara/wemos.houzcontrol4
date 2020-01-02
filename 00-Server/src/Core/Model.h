#pragma once
#include "Arduino.h"

///////////////////////////////////////////////////////////////////////////
// DEVICE

//Device Type
enum DeviceType {
  devtype_undefined = 0, //not defined device (error!)
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
  Device(int id){this->id=id;};
  Device(int _id, DeviceType _type){
    this->id=_id;
    this->type=_type;
  };
  Device(int _id, DeviceType _type, Zone zone):Device(_id, _type){
    this->zone=zone;
  };
  Device(int _id, DeviceType _type, Zone zone, NodeEnm node):Device(_id, _type, zone){
    this->node=node;
  };

  // common properties
  DeviceType type =devtype_undefined; //device type
  u8         id =0;                //channel
  u32        payload=0;            //device raw value
  Zone       zone=zone_undefined;  //device location
  NodeEnm    node = node_server;   //external node

  // base 
  virtual String toJson(){
    Serial.println("not implemented!!!!");
  };
  
  //request to query device
  virtual void update(){
    Serial.println("device::update()");
  };
  
  //update device values
  virtual void decode(long _payload){};
  
  //set device output/behavior (hex encoded)
  virtual void set(long _payload){};

  //turn device on/off (hex encoded)
  virtual void setOn(bool _on){};

};

///////////////////////////////////////////////////////////////////////////
// TASKS

// Task Command
enum Command{
  command_not_defined     ,
  command_request_update  ,
  command_update_device   ,
  command_set_device      ,
  command_play_scene      ,
  command_set_scene       ,
  command_rf_send         , 
  command_ir_send         ,
  command_rf_query        ,
  command_scene_pause     ,
  command_set_on          ,
  command_fan_on          ,
  command_fan_speed       ,
  command_send_to_mux
  
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