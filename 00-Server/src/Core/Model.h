#pragma once
#include "Arduino.h"


///////////////////////////////////////////////////////////////////////////
// DEVICE

struct Device {
public:
  u8  id;       //channel
  u8  node;     //node
	u32 payload;  //value
};

///////////////////////////////////////////////////////////////////////////
// TASKS

// Task Command
enum Command{
    command_set_device  = 1,
    command_play_scene  = 2,
    command_set_scene   = 3,
    command_rf_send     = 4, 
    command_ir_send     = 5,
    command_scene_pause = 9
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