#include "SceneManager.h"
SceneManager::SceneManager(){

};

void SceneManager::play(int sceneId){
  Serial.printf("SceneManager::play(%d)\n", sceneId);
  switch (sceneId){
    case scene_sleep:
      //turn off ac
      taskManager->addTask(command_set_on, living_AC, 0);
      taskManager->addTask(command_set_on, office_AC, 0);

      //turn off lights
      taskManager->addTask(command_set_on, living_dicro_1, 0);
      taskManager->addTask(command_set_on, living_dicro_2, 0);
      taskManager->addTask(command_set_on, living_dicro_3, 0);
      taskManager->addTask(command_set_on, living_dicro_4, 0);
      taskManager->addTask(command_set_on, living_dicro_5, 0);
      taskManager->addTask(command_set_on, living_dicro_6, 0);
      taskManager->addTask(command_set_on, living_dicro_7, 0);
      taskManager->addTask(command_set_on, living_dicro_8, 0);
      taskManager->addTask(command_set_on, living_main, 0);
      taskManager->addTask(command_set_on, living_corner, 0);
      taskManager->addTask(command_set_on, living_booksh, 0);
      taskManager->addTask(command_set_on, living_fx1, 0);
      taskManager->addTask(command_set_on, living_fx2, 0);
      taskManager->addTask(command_set_on, office_light, 0);
      taskManager->addTask(command_set_on, suite_light, 0);
    break;
  
  case scene_hello:
      //turn on lights
      taskManager->addTask(command_set_on, living_dicro_1, 1);
      taskManager->addTask(command_set_on, living_dicro_2, 1);
      taskManager->addTask(command_set_on, living_dicro_3, 1);
      taskManager->addTask(command_set_on, living_dicro_4, 1);
      taskManager->addTask(command_set_on, living_booksh,  1);
      
  break;

  default:
    Serial.printf("--> scene %X doesn't exist??\n", sceneId);
  break;
  }

};

void SceneManager::update(){
  
};
