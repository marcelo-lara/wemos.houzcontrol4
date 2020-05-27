#include "Arduino.h"
#include "wemos.setup.h"

#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

#include "src/Core/HausCore.h"
Devices *devices = devices->getInstance();
TaskManager *taskManager = TaskManager::getInstance();

#include "AsyncJson.h"
#include "ArduinoJson.h"
const String JSON_OK = "{\"result\":\"ok\"}";
const String JSON_ERR = "{\"result\":\"err\"}";


void setup()
{
  wemosWiFi.connect("hausofficeac");
  apiSetup();
};

void loop()
{
  wemosWiFi.update();

  
  // if (!taskManager->arePendingTasks()) return; 
  // Task task = taskManager->getNextTask();

  // switch (task.command){
  
  // default:
  //   Serial.printf("command [%X] not handled\n", task.command);
  //   break;
  // }
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// API
void apiSetup()
{
  server.on("/api", HTTP_GET, api_get_api);
  server.on("/api", HTTP_POST, nullRequest, NULL, api_post_api);


  //FS
  if (SPIFFS.begin()){
    server.serveStatic("/", SPIFFS, "/index.html");
  }
  else{
    Serial.println("An Error has occurred while mounting SPIFFS");
  }
  server.onNotFound(notFound);
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  //enable cors
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  DefaultHeaders::Instance().addHeader("Pragma", "no-cache");
  DefaultHeaders::Instance().addHeader("Expires", "0");
  DefaultHeaders::Instance().addHeader("Server", "HouzControl/4.0.0");

  server.begin();
};

void notFound(AsyncWebServerRequest *request){request->send((request->method() == HTTP_OPTIONS) ? 200 : 404);};
void nullRequest(AsyncWebServerRequest *request){};

void api_get_api(AsyncWebServerRequest *request){
  String jsonRes = "[";
  jsonRes += "]";

  request->send(200, "application/json", jsonRes);
}


/////////////////////////////////////
// API
void api_post_api(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  if (index){
    request->send(422, "text/plain", "no body?");
    return;
  };

  //decode json
  StaticJsonDocument<500> doc;
  DeserializationError error = deserializeJson(doc, (char *)data);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    request->send(422, "application/json", "{\"result\":\"error\"}");
    return;
  };

  //set device
  // taskManager->addTask(command_set_device, dev);

  //send response
  request->send(200, "application/json", JSON_OK);
}
