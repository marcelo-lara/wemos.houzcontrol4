#pragma once
#include "Arduino.h"
#include "src/Core/TaskManager.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include <ESPAsyncWebServer.h>

class WebServer {
public:
    WebServer(TaskManager *_taskManager);
    void setup();

private:
    TaskManager *taskManager;
};