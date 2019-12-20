#include "Button.h"

// human configuration
#define inSwitch_debounce 20
#define inSwitch_clickInterval 300
#define inSwitch_pressInterval 1000
#define inSwitch_timeout 300



Button::Button(){
};

void Button::init(int buttonGpio, ButtonEventCallback onEventCallback){
    pinMode(buttonGpio, INPUT_PULLUP);
    this->_btnGpio=buttonGpio;
    onEvent=onEventCallback;
  inSw_lastStatus=true;
};

void Button::update(){
  if(_btnGpio<1) return; //not enabled
  unsigned long currMillis = millis();
  if(inSw_lastMs + inSwitch_debounce > currMillis) return; //debounce
  bool currStatus = digitalRead(_btnGpio)==HIGH; //read status
  
  if(!inSw_lastStatus){ 
    //button is pressed > longpress?
    if((currMillis - inSw_lastMs) > inSwitch_pressInterval ){
            onEvent(10); 
      inSwitchCount=0;
      inSw_lastMs=currMillis;
    }

  }else{
    //button isn't pressed > fire click
    if(inSwitchCount>0){
      if((inSw_lastMs + inSwitch_timeout) < currMillis){
                onEvent(inSwitchCount); //callback 
        inSwitchCount=0;
      }
    }
  }
  
  if(inSw_lastStatus==currStatus) return;

  //handle click
  if(currStatus 
      && (currMillis - inSw_lastMs)>20 //ignore noise 
      && (currMillis - inSw_lastMs)<inSwitch_clickInterval) 
    inSwitchCount++;

  //store action and wait for next
  inSw_lastStatus=currStatus;
  inSw_lastMs=currMillis;
};



