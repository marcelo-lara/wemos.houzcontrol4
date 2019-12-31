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

    //api endpoints    
    server.on("/api/task", HTTP_POST, nullRequest, NULL, api_addTask);            // -> node status
    server.on("/api/update", HTTP_POST, nullRequest, NULL, api_updateNodeStatus); // -> node update
    server.on("/api/rfsend", HTTP_POST, nullRequest, NULL, api_sendRfRequest);    // <- rf send request

    server.on("/api", HTTP_GET, api_pingReply);                                   // -> status request
    server.on("/api", HTTP_POST, api_serverStatus);                               // -> status request
    //server.on("/api", HTTP_POST, nullRequest, NULL, api_serverStatus);          // <- node status    

    server.onNotFound(notFound);

    //enable cors
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Max-Age", "10000");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");

    //force no-cache
    DefaultHeaders::Instance().addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    DefaultHeaders::Instance().addHeader("Pragma", "no-cache");
    DefaultHeaders::Instance().addHeader("Expires", "0");
    DefaultHeaders::Instance().addHeader("Server", "HouzControl/4.0.0");

    server.begin();

};



//////////////////////////////////////////////////////////////////////////////////////////////////
// Generics

//not found response
void notFound(AsyncWebServerRequest *request) {
  if (request->method() == HTTP_OPTIONS) {
    request->send(200);
  } else {
    request->send(404, "text/plain", "lib - Not found");
  }
};

void nullRequest(AsyncWebServerRequest *request){
};

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

  int tsk = doc["task"];
  if(tsk==0 || tsk>99) return api_returnError(request,"error task is not valid");
  int id = doc["id"];
  long payload = doc["payload"];
  if(id==0) return api_return(request, "error: id is not set", 422);

  try
  {
    Command cTsk = (Command)tsk;
    TaskManager::getInstance()->addTask(cTsk, id, payload);    
    request->send(200, "application/json", "{\"result\":\"ok\"}");
  }
  catch(const std::exception& e)
  {
    return api_return(request, e.what(), 422);
  }
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
 // dev.node = doc["node"];
  dev.payload = doc["payload"];

  if (!(dev.id == 0 && dev.payload == 0)){
//    TaskManager::getInstance()->addTask(command_rf_send, dev);    
    request->send(200, "application/json", "{\"status\":\"packet enqueed\"}");
  }else{
    request->send(200, "application/json", "{\"status\":\"error on data\"}");
  }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// response with all nodes and devices 
//void WebServer::api_serverStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
void WebServer::api_serverStatus(AsyncWebServerRequest *request){
  Serial.println("::api_serverStatus");
  String jsonStr;
  jsonStr = "{";
  jsonStr += "\"devs\": " + Devices::getInstance()->toJson();
  jsonStr += "}";
  request->send(200, "application/json", jsonStr);
};
