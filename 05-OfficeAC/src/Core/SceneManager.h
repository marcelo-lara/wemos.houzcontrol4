#pragma once
#include "Model.h"
#include "Devices.h"
#include "TaskManager.h"
#include "QueueArray.h"

class SceneManager{
private:
  Devices *devices = Devices::getInstance();
  TaskManager *taskManager = TaskManager::getInstance();
  int currentScene = 0;
  QueueArray<Task> sceneCue;

public:
  SceneManager();
  void play(int sceneId);
  void update();
};
