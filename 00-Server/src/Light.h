#pragma once
#include "Arduino.h"

class Light {
public:
    int id;
    int gpioOut;
    bool isOn;

    // actions
    void turnOn(){this->set(true);};
    void turnOff(){this->set(false);};
    void toggle(){this->set(!this->isOn);};
    void set(bool _status){
        this->isOn=_status;
        digitalWrite(gpioOut, _status);
        Serial.print("light ");
        Serial.print(this->id, HEX);
        Serial.print(" | ");
        Serial.println(this->isOn?"on":"off");
    };

    // json
    String getJson(){
        String json="{\"id\":";
        json += this->id;
        json += ",\"value\":";
        json +=  this->isOn?"1":"0";
        json += "}";
        return json;
    };

    void doAction(int _cmd, int _payload){
        if(_cmd==1){ //set
            switch (_payload){
                case 0:  this->turnOff(); break;
                case 1:  this->turnOn();  break;
                default: this->toggle();  break;
            }
        }
    };

    Light(int _id, int _out){
        this->id=_id;
        this->gpioOut=_out;
        pinMode(_out, OUTPUT);
        this->turnOff();
    };
};
