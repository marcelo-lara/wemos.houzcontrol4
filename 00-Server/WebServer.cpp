#include "WebServer.h"
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

//declarations
void notFound(AsyncWebServerRequest *request);
void nullRequest(AsyncWebServerRequest *request);
void api_pingReply(AsyncWebServerRequest *request);
void api_setNodeStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void api_updateNodeStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void api_sendRfRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
void api_serverStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

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


    // -> node status
    server.on("/api/status", HTTP_POST, nullRequest, NULL, api_setNodeStatus);

    // -> node update
    server.on("/api/update", HTTP_POST, nullRequest, NULL, api_updateNodeStatus);

    // -> status request
    server.on("/api", HTTP_GET, api_pingReply);

    // <- rf send request
    server.on("/api/send", HTTP_POST, nullRequest, NULL, api_sendRfRequest);

    // <- node status    
    server.on("/api", HTTP_POST, nullRequest, NULL, api_serverStatus);

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
    request->send(200, "application/json", "{\"result\":\"ok\"}");
};

// replace node devices with received values
void api_setNodeStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    request->send(200, "application/json", "{\"status\":\"ok\"}");
}

// // update node device with received values
void api_updateNodeStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    request->send(200, "application/json", "{\"status\":\"ok\"}");
}

// // request to deliver a RF packet
void api_sendRfRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    request->send(200, "application/json", "{\"status\":\"ok\"}");
}


// // response with all nodes and devices
void api_serverStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    request->send(200, "application/json", "{\"status\":\"ok\"}");
}



