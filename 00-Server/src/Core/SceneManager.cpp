#include "SceneManager.h"


SceneStep _sleep_cue[9]{
  {command_set_device, office_AC		    , 0},
  {command_set_device, office_light	    , 2},
  {command_set_device, suite_light	    , 0},
  {command_set_device, suite_fan		    , 4},
  {command_set_device, suite_AC		      , 0},
  {command_set_device, living_mainLight , 2},
  {command_set_device, living_dicroLight, 0},
  {command_set_device, living_spotLight , 0},
  {command_set_device, living_fxLight	  , 0},
};

SceneManager::SceneManager(){

};

void SceneManager::play(int sceneId){
  Serial.printf("SceneManager::play(%d)\n", sceneId);

  SceneStep *scene;
  int scene_lenght = 0;
  switch (sceneId)
  {
  case scene_sleep:
    play(_sleep_cue);
    break;

  default:
    break;
  }

};

void SceneManager::play(SceneStep *scene){
  int scene_lenght=(sizeof(scene)/sizeof(*scene));

  Serial.printf("scene lenght: %i steps...\n", scene_lenght);
  for (int i = 0; i < scene_lenght; i++){
    Serial.printf("step %i\t%i:%i>%i\n", i, scene[i].deviceId,devices->get(scene[i].deviceId)->payload, scene[i].payload);
    if(devices->get(scene[i].deviceId)->payload!=scene[i].payload){
      Serial.println("\tchange..");

      //taskManager->addTask(scene[i].command, scene[i].deviceId, scene[i].payload);

    };
  };
}
