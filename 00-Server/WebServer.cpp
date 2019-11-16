#include "WebServer.h"
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

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
    //server.on("/api/status", HTTP_POST, nullRequest, NULL, api_setNodeStatus);

    // -> node update
    //server.on("/api/update", HTTP_POST, nullRequest, NULL, api_updateNodeStatus);

    // -> status request
    server->on("/api", HTTP_GET, api_pingReply);

    // <- rf send request
    //server.on("/api/send", HTTP_POST, nullRequest, NULL, api_sendRfRequest);

    // <- node status    
    //server.on("/api", HTTP_POST, nullRequest, NULL, api_serverStatus);

    server.onNotFound(notFound);
    server.begin();

};

void WebServer::notFound(AsyncWebServerRequest *request) {request->send(404, "text/plain", "Not found");}
// response to node keepalive query
void WebServer::api_pingReply(AsyncWebServerRequest *request){request->send(200, "application/json", "{\"result\":\"ok\"}");}
