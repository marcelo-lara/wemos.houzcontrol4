#pragma once
#include "Arduino.h"
#include "QueueArray.h"

enum Command{
    command_set = 1
};

struct Device {
public:
    u8  id;
	u32 payload;
};

enum SceneEnm{
    scene_unknown = 0,
    scene_sleep   = 1,
    scene_goobye  = 2,
    scene_hello   = 3
};

struct Task {
public:
    Command command;
    Device device;
};

class TaskManager{
public:
    void addTask(Command _command, Device _device);
    void addTask(Command _command, int _deviceId, long _payload);
    bool arePendingTasks();
    Task getNextTask();
private:
    QueueArray<Task> taskQueue;
};
extern TaskManager taskManager;
