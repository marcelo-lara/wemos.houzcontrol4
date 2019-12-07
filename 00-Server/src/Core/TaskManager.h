#pragma once
#include "Arduino.h"
#include "HouzModel.h"
#include "QueueArray.h"

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
