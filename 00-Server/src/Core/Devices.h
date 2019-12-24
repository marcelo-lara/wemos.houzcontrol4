#pragma once
#include "Arduino.h"
#include "Model.h"
#include "Channels.h"

#include "Devices/Light.h"
#include "Devices/Fan.h"



// device storage
class Devices{
private:
    static Devices* instance;
    void   lightsSetup();
    Devices();
    
    //TODO: cache json output
    String _jsonCache = "";

public:
  static Devices* getInstance();

  int listLen; //total channels
  Device* get(u8 deviceId);
  void set(u8 deviceId, u32 payload);

  //fans universe
  Fan fans[1]{
    Fan(suite_fan, zone_suite, node_suite)
  };
  int fansLen = 1;
  Fan* getFan(int _id);

  //lights universe
  Light lights[15]{
    Light(office_light  , zone_office, node_office),
    Light(suite_light   , zone_suite,  node_suite),
    Light(living_dicro_1, zone_living, node_living),
    Light(living_dicro_2, zone_living, node_living),
    Light(living_dicro_3, zone_living, node_living),
    Light(living_dicro_4, zone_living, node_living),
    Light(living_dicro_5, zone_living, node_living),
    Light(living_dicro_6, zone_living, node_living),
    Light(living_dicro_7, zone_living, node_living),
    Light(living_dicro_8, zone_living, node_living),
    Light(living_main   , zone_living, node_living),
    Light(living_booksh , zone_living, node_living),
    Light(living_corner , zone_living, node_living),
    Light(living_fx1    , zone_living, node_living),
    Light(living_fx2    , zone_living, node_living)
  };
  int    lightsLen = 0;
  Light* getLight(int _id);

  //ac universe
  //environment universe

  ///////////////////////////////////////////////////////////////
  Device list[16]{
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
    lights[14],
    fans[0]
  };

  //TODO: cache output
  String toJson();
};
