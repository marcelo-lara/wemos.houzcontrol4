#include "Arduino.h"
#include "wemos.setup.h"

//#include <ESPAsyncWebServer.h>
//AsyncWebServer server(80);

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
    //apiSetup();
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
// void apiSetup(){



// };

// // replace node devices with received values
// void api_setNodeStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){}

// // update node device with received values
// void api_updateNodeStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){}

// // request to deliver a RF packet
// void api_sendRfRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){}

// // response with all nodes and devices
// void api_serverStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){}

// // response to node keepalive query
// void api_pingReply(AsyncWebServerRequest *request){request->send(200, "application/json", "{\"result\":\"ok\"}");}

// void notFound(AsyncWebServerRequest *request) {request->send(404, "text/plain", "Not found");}
// void nullRequest(AsyncWebServerRequest *request){};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void devicePrint(Device dev){
  Serial.print("id  \t");
  Serial.println(dev.id, HEX);
  Serial.print("payload\t");
  Serial.println(dev.payload, HEX);
};


