#pragma once
#include "Arduino.h"
#include "../Core/HausCore.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include <ESPAsyncWebServer.h>

class WebServer {
public:
  WebServer();
  void setup();

private:
  Devices* devices;
  static void api_serverStatus(AsyncWebServerRequest *request);
};