#pragma once
#include "Arduino.h"
#include "../Core/HausCore.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include <ESPAsyncWebServer.h>

class WebServer {
public:
    WebServer();
    Devices* devices;
    void setup();

private:
  static void api_serverStatus(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
};