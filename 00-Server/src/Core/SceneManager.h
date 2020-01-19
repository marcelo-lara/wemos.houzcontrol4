#pragma once
#include "Model.h"
#include "Devices.h"
#include "TaskManager.h"

class SceneManager{
private:
  Devices *devices = Devices::getInstance();
  TaskManager *taskManager = TaskManager::getInstance();
  int currentScene = 0;

public:
  SceneManager();
  void play(int sceneId);
  void update();
};
