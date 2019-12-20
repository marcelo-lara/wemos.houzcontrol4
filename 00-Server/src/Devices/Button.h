#pragma once
#include "Arduino.h"

// helpers
#define buttonEv_longpress 10
#define buttonEv_click      1
#define buttonEv_dblclick   2

typedef void (*ButtonEventCallback)(int _click);

class Button{
private:
    int _btnGpio;
  bool inSw_lastStatus;
  unsigned long inSw_lastMs;
  unsigned long inSw_downMs;
    int inSwitchCount;
    ButtonEventCallback onEvent;

public:
    Button();
    void init(int buttonGpio, ButtonEventCallback onEventCallback);
    void update();
    
};