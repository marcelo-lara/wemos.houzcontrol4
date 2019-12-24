#pragma once
#include "Arduino.h"
#include "Model.h"
#include "Channels.h"
#include "Devices/Light.h"
// device storage
class Devices{
private:
    static Devices* instance;
    void   lightsSetup();
    Devices();

public:
  static Devices* getInstance();

  int listLen; //total channels
  Device* get(u8 deviceId);
  void set(u8 deviceId, u32 payload);

  //lights universe
  Light lights[15]{
    Light(office_light  , node_office),
    Light(suite_light   , node_suite),
    Light(living_dicro_1, node_living),
    Light(living_dicro_2, node_living),
    Light(living_dicro_3, node_living),
    Light(living_dicro_4, node_living),
    Light(living_dicro_5, node_living),
    Light(living_dicro_6, node_living),
    Light(living_dicro_7, node_living),
    Light(living_dicro_8, node_living),
    Light(living_main   , node_living),
    Light(living_booksh , node_living),
    Light(living_corner , node_living),
    Light(living_fx1    , node_living),
    Light(living_fx2    , node_living)
  };
  int    lightsLen = 0;
  Light* getLight(int _id);

  Device list[15]{
    lights[ 0],
    lights[ 1],
    lights[ 2],
    lights[ 3],
    lights[ 4],
    lights[ 5],
    lights[ 6],
    lights[ 7],
    lights[ 8],
    lights[ 9],
    lights[10],
    lights[11],
    lights[12],
    lights[13],
    lights[14]
  };

  static String printDevice(Device device){
    String str = "[id:";
    str += device.id;
    str += "|t";
    str += device.type;
    str += "|p";
    str += device.payload;
    str += "]";
    return str;
  };
};
