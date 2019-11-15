#include "Arduino.h"
#include "wemos.setup.h"

#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

#include <RF24.h>
#define rf_ce   2 //D4
#define rf_csn 15 //D8
RF24 radio(rf_ce,rf_csn);

#include "AsyncJson.h"
#include "ArduinoJson.h"
const String JSON_OK = "{\"result\":\"ok\"}";

// HouzControl4 modules
#include "src/TaskManager.h"


void setup(){
    wemosWiFi.connect("houzserver");
    apiSetup();
};

void loop(){
    wemosWiFi.update();
    if(taskManager.arePendingTasks()) runTask(); 
};

void runTask(){
    Task task = taskManager.getNextTask();

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// API
void apiSetup(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        //TODO: single ui to handle devices
        request->send(200, "text/plain", "HouzNode");
    }); 


    server.onNotFound(notFound);
    server.begin();

};

void notFound(AsyncWebServerRequest *request) {request->send(404, "text/plain", "Not found");}
void nullRequest(AsyncWebServerRequest *request){};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void devicePrint(Device dev){
  Serial.print("id  \t");
  Serial.println(dev.id, HEX);
  Serial.print("payload\t");
  Serial.println(dev.payload, HEX);
};


