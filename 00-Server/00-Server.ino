#include "Arduino.h"
#include "wemos.setup.h"

#include <RF24.h>
#define rf_ce   2 //D4
#define rf_csn 15 //D8
RF24 radio(rf_ce,rf_csn);

#include "AsyncJson.h"
#include "ArduinoJson.h"
const String JSON_OK = "{\"result\":\"ok\"}";

// HouzControl4 modules
#include "src/TaskManager.h"
#include "WebServer.h"
WebServer webServer;

void setup(){
    wemosWiFi.connect("houzserver");
    webServer.setup();
};

void loop(){
    wemosWiFi.update();
    if(taskManager.arePendingTasks()) runTask(); 
};

void runTask(){
    Task task = taskManager.getNextTask();

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void devicePrint(Device dev){
  Serial.print("id  \t");
  Serial.println(dev.id, HEX);
  Serial.print("payload\t");
  Serial.println(dev.payload, HEX);
};


