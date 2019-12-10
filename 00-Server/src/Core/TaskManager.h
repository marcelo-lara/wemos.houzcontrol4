#pragma once
#include "Arduino.h"
#include "Model.h"
#include "QueueArray.h"


class TaskManager{
public:
    void addTask(Command _command, Device _device);
    void addTask(Command _command, int _deviceId, long _payload);
    bool arePendingTasks();
    Task getNextTask();

    static TaskManager* getInstance();
private:
    QueueArray<Task> taskQueue;
    static TaskManager* instance;
    TaskManager();

};

