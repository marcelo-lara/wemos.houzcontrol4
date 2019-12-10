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


// fixed scenes
enum SceneEnm{
    scene_unknown = 0,
    scene_sleep   = 1,
    scene_goobye  = 2,
    scene_hello   = 3
};


///////////////////////////////////////////////////////////////////////////
// TASKS

// Task Command
enum Command{
    command_set_device = 1,
    command_play_scene  = 2,
    command_set_scene  = 3,
    command_rf_send    = 4, 
    command_ir_send    = 5
};

// Task
struct Task {
public:
    Command command;
    Device  device;
};

