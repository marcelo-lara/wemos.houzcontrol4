#pragma once
#include "Arduino.h"

struct Device {
public:
    u8  id;
    u8  node;
	u32 payload;
};

enum SceneEnm{
    scene_unknown = 0,
    scene_sleep   = 1,
    scene_goobye  = 2,
    scene_hello   = 3
};

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
    Device device;
};