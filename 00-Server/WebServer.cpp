#include "WebServer.h"
AsyncWebServer server(80);
const String JSON_OK = "{\"result\":\"ok\"}";

//declarations
void notFound(AsyncWebServerRequest *request);
void nullRequest(AsyncWebServerRequest *request);
void api_pingReply(AsyncWebServerRequest *request);
void api_addTask(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
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
    server.on("/api/task", HTTP_POST, nullRequest, NULL, api_addTask);         // -> node status
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

void api_return(AsyncWebServerRequest *request, String message, int code){
  request->send(code, "application/json", "{\"result\":\"" + message + "\"}");
}

void api_returnError(AsyncWebServerRequest *request, String message){
    Serial.print("ERROR: ");
    Serial.println(message);
    api_return(request, message, 422);
};


//////////////////////////////////////////////////////////////////////////////////////////////////
// API

// keepalive query response
void api_pingReply(AsyncWebServerRequest *request){
  Serial.println("::api_pingReply");
  request->send(200, "application/json", "{\"result\":\"ok\"}");
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// place task in TaskManager
void api_addTask(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  Serial.println("::api_addTask");

  //decode json
  StaticJsonDocument<500> doc;
  DeserializationError error = deserializeJson(doc, (char *)data);
  if (error) return api_returnError(request, "error parsing json");

  int cmd = doc["task"];
  if(cmd==0 || cmd>9) return api_returnError(request,"error task is not valid");
  int id = doc["id"];
  long payload = doc["payload"];
  switch (cmd)
  {
  case (int)command_set_device:
    if(id==0) return api_return(request, "error: id is not set", 422);
    TaskManager::getInstance()->addTask(command_set_device, id, payload);    
    return api_return(request, "set_device enqueued", 200);
    break;

  case (int)command_play_scene:
    if(payload==0) return api_returnError(request, "error: scene not set");
    TaskManager::getInstance()->addTask(command_play_scene, id, payload);    
    return api_return(request, "play_scene enqueued", 200);
    break;  

  default:
    return api_return(request, "command not handled", 422);
    break;
  }

  request->send(200, "application/json", "task enqueed");
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// update node device with received values (from node)
void api_updateNodeStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  Serial.println("::api_updateNodeStatus");
  request->send(200, "application/json", "not implemented");
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// request to deliver a RF packet
void api_sendRfRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  Serial.println("::api_sendRfRequest");
  
  //decode json
  StaticJsonDocument<500> doc;
  DeserializationError error = deserializeJson(doc, (char *)data);
  if (error) return api_returnError(request, "error parsing json");

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
