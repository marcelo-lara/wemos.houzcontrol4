#pragma once
#include "Arduino.h"
#include "Model.h"
#include "Channels.h"

#include "Devices/Light.h"
#include "Devices/Fan.h"
#include "Devices/Environment.h"

// device storage
class Devices{
private:
    static Devices* instance;
    Devices();
    
    //TODO: cache json output
    String _jsonCache = "";

public:
  static Devices* getInstance();

  //devices universe
  Device *list[18]{
    new Light(office_light  , zone_office, node_office),
    new Light(suite_light   , zone_suite,  node_suite),
    new Light(living_dicro_1, zone_living, node_living, living_dicroLight, 0),
    new Light(living_dicro_2, zone_living, node_living, living_dicroLight, 1),
    new Light(living_dicro_3, zone_living, node_living, living_dicroLight, 2),
    new Light(living_dicro_4, zone_living, node_living, living_dicroLight, 3),
    new Light(living_dicro_5, zone_living, node_living, living_dicroLight, 4),
    new Light(living_dicro_6, zone_living, node_living, living_dicroLight, 5),
    new Light(living_dicro_7, zone_living, node_living, living_dicroLight, 6),
    new Light(living_dicro_8, zone_living, node_living, living_dicroLight, 7),
    new Light(living_main   , zone_living, node_living, living_mainLight,  0),
    new Light(living_booksh , zone_living, node_living, living_spotLight,  0),
    new Light(living_corner , zone_living, node_living, living_spotLight,  1),
    new Light(living_fx1    , zone_living, node_living, living_fxLight,    0),
    new Light(living_fx2    , zone_living, node_living, living_fxLight,    1),
    new Fan(suite_fan, zone_suite, node_suite),
    new Environment(external_weather, zone_outside, node_office, external_temp, external_humidity, external_pressure, external_light),
    new Environment(suite_enviroment, zone_suite, node_suite, suite_temp, suite_humidity, suite_pressure, suite_light)
  };
  int listLen = 18; //total channels
  Device* get(u8 deviceId);
  Device* getByMux(int muxCh);
  void set(u8 deviceId, u32 payload);


  //TODO: cache output
  String toJson();
};
