#pragma once
#include "Arduino.h"
#include "Model.h"
#include "QueueArray.h"


class TaskManager{
public:
    void addTask(Task task);
    void addTask(Command _command, Device _device);
    void addTask(Command _command, int _deviceId, long _payload, NodeEnm _node = node_server);
    bool arePendingTasks();
    Task getNextTask();

    static TaskManager* getInstance();
private:
    QueueArray<Task> taskQueue;
    static TaskManager* instance;
    TaskManager();

};

