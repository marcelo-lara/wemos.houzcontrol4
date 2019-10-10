#pragma once
#include "TaskManager.h"

QueueArray<Task> taskQueue;

//Add a task to queue
void TaskManager::addTask(Command _command, int _deviceId, long _payload){
    Device dev;
    dev.id=_deviceId;
    dev.payload=_payload;
    addTask((Command)_command, dev);
};

//Add a task to queue
void TaskManager::addTask(Command _command, Device _device){
    Task task;
    task.command = _command;
    task.device = _device;
    taskQueue.enqueue(task);
};

bool TaskManager::arePendingTasks(){
    return !taskQueue.isEmpty();
};

Task TaskManager::getNextTask(){
    return taskQueue.dequeue();
};

TaskManager taskManager;