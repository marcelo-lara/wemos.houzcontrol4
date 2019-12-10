#include "WebServer.h"
AsyncWebServer server(80);
const String JSON_OK = "{\"result\":\"ok\"}";

//declarations
void notFound(AsyncWebServerRequest *request);
void nullRequest(AsyncWebServerRequest *request);
void api_pingReply(AsyncWebServerRequest *request);
void api_setNodeStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void api_updateNodeStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void api_sendRfRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);


WebServer::WebServer(){
    devices = devices->getInstance();
};

void WebServer::setup(){
    Serial.println("--WebServer.setup()--");

    // frontend
    if (SPIFFS.begin()){
        server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    }
    else{
        Serial.println("An Error has occurred while mounting SPIFFS");
    }

    //enable cors
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    DefaultHeaders::Instance().addHeader("Pragma", "no-cache");
    DefaultHeaders::Instance().addHeader("Expires", "0");
    DefaultHeaders::Instance().addHeader("Server", "HouzControl/4.0.0");

    //api endpoints    
    server.on("/api/status", HTTP_POST, nullRequest, NULL, api_setNodeStatus);    // -> node status
    server.on("/api/update", HTTP_POST, nullRequest, NULL, api_updateNodeStatus); // -> node update
    server.on("/api/rfsend", HTTP_POST, nullRequest, NULL, api_sendRfRequest);    // <- rf send request
    server.on("/api", HTTP_GET, api_pingReply);                                   // -> status request
    server.on("/api", HTTP_POST, nullRequest, NULL, api_serverStatus);            // <- node status    

    server.onNotFound(notFound);
    server.begin();

};



//////////////////////////////////////////////////////////////////////////////////////////////////
// Generics

//not found response
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "lib - Not found");
};

void nullRequest(AsyncWebServerRequest *request){};

//////////////////////////////////////////////////////////////////////////////////////////////////
// API

// keepalive query response
void api_pingReply(AsyncWebServerRequest *request){
  Serial.println("::api_pingReply");
  request->send(200, "application/json", "{\"result\":\"ok\"}");
};

// replace node devices with received values
void api_setNodeStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  Serial.println("::api_setNodeStatus");

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
  TaskManager *taskManager = taskManager->getInstance();

  //set scene shortcut
  int scene = doc["scene"];
  if (scene > 0){
    request->send(200, "application/json", JSON_OK);
    taskManager->addTask(command_set_scene, 0, scene);
    return;
  }

  //set device
  Device dev;
  dev.id = doc["id"];
  dev.payload = doc["payload"];
  if (!(dev.id == 0 && dev.payload == 0))
    taskManager->addTask(command_set_device, dev);    
  request->send(200, "application/json", "{\"status\":\"ok\"}");
};

// // update node device with received values
void api_updateNodeStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  Serial.println("::api_updateNodeStatus");
  request->send(200, "application/json", "{\"status\":\"not implemented\"}");
};

// request to deliver a RF packet
void api_sendRfRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  Serial.println("::api_sendRfRequest");
  //decode json
  StaticJsonDocument<500> doc;
  DeserializationError error = deserializeJson(doc, (char *)data);

  if (error){
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    request->send(422, "application/json", "{\"result\":\"error parsing json\"}");
    return;
  };

  //set device
  Device dev;
  dev.id = doc["channel"];
  dev.node = doc["node"];
  dev.payload = doc["payload"];

  if (!(dev.id == 0 && dev.payload == 0)){
    TaskManager::getInstance()->addTask(command_rf_send, dev);    
    request->send(200, "application/json", "{\"status\":\"packet enqueed\"}");
  }else{
    request->send(200, "application/json", "{\"status\":\"error on data\"}");
  }
};

// response with all nodes and devices 
void WebServer::api_serverStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  Devices *dev = dev->getInstance();
  
  Serial.println("::api_serverStatus");
  String jsonStr;
  jsonStr = "{";
  jsonStr += "\"devs\": [";
  for (int i = 0; i < dev->deviceListLen; i++)
  {
    jsonStr += "{\"id\": ";
    jsonStr += dev->deviceList[i].id;
    jsonStr += ", \"val\": ";
    jsonStr += dev->deviceList[i].payload;
    jsonStr += "]";
    if(i != dev->deviceListLen) jsonStr += ", ";
  }
  jsonStr += "]}";
  request->send(200, "application/json", jsonStr);
};
