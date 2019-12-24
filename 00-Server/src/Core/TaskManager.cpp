#pragma once
#include "TaskManager.h"

QueueArray<Task> taskQueue;

//Add a task to queue
void TaskManager::addTask(Command _command, int _deviceId, long _payload, NodeEnm _node){
    Device dev;
    dev.id=_deviceId;
    dev.node=_node;
    dev.payload=_payload;
    this->addTask((Command)_command, dev);
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

// instance ///////////////////////////////////
TaskManager* TaskManager::getInstance(){
  if(instance==0) instance=new TaskManager();
  return instance;
};
TaskManager::TaskManager(){
};
TaskManager* TaskManager::instance = 0;
