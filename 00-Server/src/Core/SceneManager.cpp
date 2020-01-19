// ///////////////////////
// // TODO: handle scene steps
// // TODO: pause between steps

#include "SceneManager.h"
// #define arraySize(array) ((sizeof(array))/(sizeof(array[0])))

// SceneStep _sleep_cue[] = {
//   {command_set_device, office_AC		    ,0},
//   {command_set_device, office_light	    ,0},
//   {command_set_device, suite_light	    ,0},
//   {command_set_device, living_mainLight ,0},
//   {command_set_device, living_dicroLight,0},
//   {command_set_device, living_spotLight ,0},
//   {command_set_device, living_fxLight	  ,0}
// };

SceneManager::SceneManager(){

};

// void SceneManager::play(int sceneId){
//   Serial.printf("SceneManager::play(%d)\n", sceneId);
//   switch (sceneId){
//   case scene_sleep:
//     play(_sleep_cue, arraySize(_sleep_cue));
//     break;

//   default:
//     Serial.printf("--> scene %i doesn't exist??\n", sceneId);
//     break;
//   }

// };

// void SceneManager::play(SceneStep *scene, int scene_lenght){
//   //Serial.printf("scene lenght: %i steps...\n", scene_lenght);
//   for (int i = 0; i < scene_lenght; i++){
//     //Serial.printf("step %i\t%i:%i>%i\n", i, scene[i].deviceId,devices->get(scene[i].deviceId)->payload, scene[i].payload);
//     if(devices->get(scene[i].deviceId)->payload!=scene[i].payload){
//       //Serial.println("\tchange..");
//       taskManager->addTask(scene[i].command, scene[i].deviceId, scene[i].payload);
//     };
//   };
// }
