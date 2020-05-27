#include "SceneManager.h"

int sm_current_scene=0;
int sm_total_steps=0;
int sm_current_step=0;
unsigned long sm_next_step = 0;

Task scene_cue_dicro_on[]{
  Task(command_set_on, Device(living_dicro_1, 0)),
  Task(command_scene_pause, Device(0, 250)),
  
  Task(command_set_on, Device(living_dicro_2, 0)),
  Task(command_scene_pause, Device(0, 250)),

  Task(command_set_on, Device(living_dicro_3, 0)),
  Task(command_scene_pause, Device(0, 250)),

  Task(command_set_on, Device(living_dicro_4, 0)),
  Task(command_scene_pause, Device(0, 250)),

  Task(command_set_on, Device(living_dicro_5, 0)),
  Task(command_scene_pause, Device(0, 250)),

  Task(command_set_on, Device(living_dicro_6, 0)),
  Task(command_scene_pause, Device(0, 250)),

  Task(command_set_on, Device(living_dicro_7, 0)),
  Task(command_scene_pause, Device(0, 250)),

  Task(command_set_on, Device(living_dicro_8, 0))
};

SceneManager::SceneManager(){

};

void SceneManager::play(int sceneId){
  Serial.printf("SceneManager::play(%d)\n", sceneId);
  sm_current_scene=sceneId;
  sm_current_step=0;

  switch (sceneId){
    case scene_sleep:
      //turn off ac
      sceneCue.enqueue(Task(command_set_on, Device(living_fx1, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_fx2, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_1, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_2, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_3, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_4, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_5, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_6, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_7, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_8, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_main, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_booksh, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_corner, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_4, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_4, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_4, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_4, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_4, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_4, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_4, 0)));
      sceneCue.enqueue(Task(command_set_on, Device(living_dicro_4, 0)));

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

  case scene_dicro_on:
    for (int d = living_dicro_1; d < living_dicro_8; d++){
      if(!Devices::getInstance()->get(d)->isOn()){
        sceneCue.enqueue(Task(command_set_on, Device(d, true)));
        sceneCue.enqueue(Task(command_scene_pause, Device(0, 100)));
      }
    }
  break;

  case scene_dicro_off:
    for (int d = living_dicro_1; d < living_dicro_8; d++){
      if(Devices::getInstance()->get(d)->isOn()){
        sceneCue.enqueue(Task(command_set_on, Device(d, false)));
        sceneCue.enqueue(Task(command_scene_pause, Device(0, 100)));
      }
    }
  break;


  default:
    Serial.printf("--> scene %X doesn't exist??\n", sceneId);
  break;
  }

};

void SceneManager::update(){
  if(sceneCue.isEmpty()) return;
  if(millis()<sm_next_step) return;//scene delay timer
  Task runTask = sceneCue.dequeue();

  if(runTask.command==command_scene_pause){
    sm_next_step = millis() + runTask.device.payload;
    return;
  }

  taskManager->addTask(runTask);
};
