#pragma once
#include "Arduino.h"
#include "../../Core/Devices.h"

struct Channel {
public:
  u8  id;       //channel
  u8  node;     //node
  u32 payload;  //value
};

class Channels {
public:
    Channel channelList[44]{ 
      {server_node	     ,server_node, 0},
      {server_rf         ,server_node, 0},
      {office_node	     ,office_node, 0},
      {office_AC		     ,office_node, 0},
      {office_AC_temp	   ,office_node, 0},
      {office_light	     ,office_node, 0},
      {office_switchLed  ,office_node, 0},
      {office_switch     ,office_node, 0},
      {office_ir		     ,office_node, 0},
      {external_light	   ,office_node, 0},
      {external_temp	   ,office_node, 0},
      {external_humidity ,office_node, 0},
      {external_pressure ,office_node, 0},
      {external_weather  ,office_node, 0},
      {suite_node		     ,suite_node , 0},
      {suite_light	     ,suite_node , 0},
      {suite_fan		     ,suite_node , 0},
      {suite_AC		       ,suite_node , 0},
      {suite_enviroment  ,suite_node , 0},
      {suite_temp		     ,suite_node , 0},
      {suite_humidity    ,suite_node , 0},
      {suite_pressure	   ,suite_node , 0},
      {living_node       ,living_node, 0},
      {living_switch	   ,living_node, 0},
      {living_rawRender  ,living_node, 0},
      {living_mainLight  ,living_node, 0},
      {living_dicroLight ,living_node, 0},
      {living_spotLight  ,living_node, 0},
      {living_fxLight	   ,living_node, 0},
      {living_fx		     ,living_node, 0},
      {living_dicro_1    ,living_node, 0},
      {living_dicro_2    ,living_node, 0},
      {living_dicro_3    ,living_node, 0},
      {living_dicro_4    ,living_node, 0},
      {living_dicro_5    ,living_node, 0},
      {living_dicro_6    ,living_node, 0},
      {living_dicro_7    ,living_node, 0},
      {living_dicro_8    ,living_node, 0},
      {living_main       ,living_node, 0},
      {living_booksh     ,living_node, 0},
      {living_corner     ,living_node, 0},
      {living_fx1        ,living_node, 0},
      {living_fx2        ,living_node, 0},
      {living_AC		     ,living_node, 0}
    };


};