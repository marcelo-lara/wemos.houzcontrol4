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
    wemosWiFi.connect("houzserver");
    button.init(5, onButtonClick);
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

    for (size_t i = 0; i < deviceCount; i++)
    {
        if(devices[i].id!=task.device.id) continue;
        devices[i].doAction(task.command, task.device.payload);
        break;
    };

};

void onButtonClick(int btnClickType){
    switch (btnClickType)
    {
        case buttonEv_click: taskManager.addTask(command_set, 1, -1); break;
        case buttonEv_dblclick: taskManager.addTask(command_set, 2, -1); break;
        case buttonEv_longpress: 
            taskManager.addTask(command_set, 1, 0); 
            taskManager.addTask(command_set, 2, 0); 
        break;
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// API
void apiSetup(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "HouzNode");
    }); 

    server.on("/api", HTTP_GET, api_get_api);
    server.on("/api", HTTP_POST, nullRequest, NULL, api_post_api);
    server.on("/api/scene", HTTP_POST, nullRequest, NULL, api_post_scene);

    server.onNotFound(notFound);
    server.begin();

};

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

void api_post_api(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.println("POST::/api");
    if(index){
        request->send(422 , "text/plain", "no body?");
        return;
    };

    //parse device
    Device dev = parseDevice((char*)data);
    if (dev.id==-1) {
      request->send(422, "application/json", "{\"parse\":\"error\"}");
      return;
    };

    //add task
    taskManager.addTask(command_set, dev);

    request->send(200, "application/json", "{\"parse\":\"ok\"}");
}

void api_post_scene(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    int rawScene = parseScene((char*)data);
    if(rawScene==-1) return;
    // Serial.printf("scene::%i\n", (int)scene);
}

Device parseDevice(String jsonStr){
  Device ret;
  ret.id=-1;

  //decode json
  StaticJsonDocument<500> doc;
  DeserializationError error = deserializeJson(doc, jsonStr);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return ret;
  };

  //parse device
  ret.id=doc["id"];
  ret.payload=doc["payload"];
  
  return ret;
 };

 int parseScene(String jsonStr){
    //decode json
    StaticJsonDocument<500> doc;
    DeserializationError error = deserializeJson(doc, jsonStr);
    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return -1;
    };
    return doc["scene"];
 };

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void devicePrint(Device dev){
  Serial.print("id  \t");
  Serial.println(dev.id, HEX);
  Serial.print("payload\t");
  Serial.println(dev.payload, HEX);
};


