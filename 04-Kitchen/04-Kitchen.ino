#include "Arduino.h"
#include "wemos.setup.h"

//light module fixtue
#define ch1 4
#define ch2 14
#define ch3 12
#define ch4 13

#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

#include "AsyncJson.h"
#include "ArduinoJson.h"
const String JSON_OK = "{\"result\":\"ok\"}";

// HouzControl4 modules
#include "src/Light.h"
#include "src/TaskManager.h"

#include "src/Button.h"
Button button;

// total devices on node
#define deviceCount 4

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Light object
Light devices[deviceCount] = {
    Light(1, ch1), 
    Light(2, ch2),
    Light(3, ch3), 
    Light(4, ch4)
};

void setup(){
    wemosWiFi.connect("houzkitchen");
    button.init(5, handleButtonClick);
    for (size_t i = 0; i < deviceCount; i++)
        devices[i].onLightStatusChange = handleLightStatusChange;
    apiSetup();
};

void loop(){
    wemosWiFi.update();
    button.update();
    taskWorker();
};

void taskWorker(){
    if(!taskManager.arePendingTasks()) return; //nothing to do...
    Task task = taskManager.getNextTask();

    //scene shortcut
    if(task.command==command_set_scene){
        renderScene(task.device.payload);
        return;
    }

    //action should be performed on a device
    for (size_t i = 0; i < deviceCount; i++)
    {
        if(devices[i].id!=task.device.id) continue;
        devices[i].doAction(task.command, task.device.payload);
        break;
    };

};

// delegates
void handleButtonClick(int btnClickType){
    switch (btnClickType){
        case buttonEv_click:     taskManager.addTask(command_set_device, 1, -1); break;
        case buttonEv_dblclick:  taskManager.addTask(command_set_device, 2, -1); break;
        case buttonEv_longpress: taskManager.addTask(command_set_scene, 0, scene_sleep); break;
    }
};

void handleLightStatusChange(int deviceId, int newStatus){
    Serial.print("light ");
    Serial.print(deviceId);
    Serial.print("\t");
    Serial.println(newStatus?"on":"off");
    wemosWiFi.blink();
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// API
void apiSetup(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        //TODO: single ui to handle devices
        request->send(200, "text/plain", "HouzNode");
    }); 

    server.on("/api", HTTP_GET, api_get_api);
    server.on("/api", HTTP_POST, nullRequest, NULL, api_post_api);

    server.onNotFound(notFound);
    server.begin();

};

void notFound(AsyncWebServerRequest *request) {request->send(404, "text/plain", "Not found");}
void nullRequest(AsyncWebServerRequest *request){};

void api_get_api(AsyncWebServerRequest *request){
    Serial.println("GET::/api");
    String jsonRes = "[";
    for (size_t i = 0; i < deviceCount; i++)
    {
        jsonRes += devices[i].getJson();
        jsonRes += (i+1<deviceCount)?",":"";
    };
    jsonRes += "]";
    request->send(200, "application/json", jsonRes);
}


/////////////////////////////////////
// Devices
void api_post_api(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.println("POST::/api");
    if(index){
        request->send(422 , "text/plain", "no body?");
        return;
    };

    //decode json
    StaticJsonDocument<500> doc;
    DeserializationError error = deserializeJson(doc, (char*)data);
    if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
      request->send(422, "application/json", "{\"result\":\"error\"}");
      return;
    };

    //set scene shortcut
    int scene=doc["scene"];
    if(scene>0){
        request->send(200, "application/json", JSON_OK);
        taskManager.addTask(command_set_scene, 0, scene);
        return;
    }

    //set device
    Device dev;
    dev.id=doc["id"];
    dev.payload=doc["payload"];
    if(!(dev.id==0&&dev.payload==0))
        taskManager.addTask(command_set_device, dev);
    request->send(200, "application/json", JSON_OK);
}

/////////////////////////////////////
// Scenes
void renderScene(int sceneId){
    SceneEnm scene = (SceneEnm)sceneId;
    switch (scene){
    case scene_hello:
        Serial.println("renderScene(scene_hello)");
        break;
    case scene_sleep:
        Serial.println("renderScene(scene_sleep)");
        break;
    case scene_goobye:
        Serial.println("renderScene(scene_goobye)");
        break;
    }
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void devicePrint(Device dev){
  Serial.print("id  \t");
  Serial.println(dev.id, HEX);
  Serial.print("payload\t");
  Serial.println(dev.payload, HEX);
};


