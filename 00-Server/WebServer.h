#pragma once
#include "Arduino.h"

class WebServer {
public:
    void setup();

    void notFound(AsyncWebServerRequest *request);
    void api_pingReply(AsyncWebServerRequest *request);

};