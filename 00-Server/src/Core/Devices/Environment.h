#pragma once
#include "Arduino.h"
#include "../Model.h"
#include "../TaskManager.h"


class Environment : public Device {
public:  
  Environment(int id, Zone zone, NodeEnm node, int chTemp, int chHum, int chPress, int chLight);
  NodeEnm node = node_server; //target node
  void  update();
  String toJson();

  bool  on=0;     //sensor is on
  float temp=0;   //temperature
  float hum=0;    //humidity
  float press=0;  //pressure
  float light=0;  //light level

private:
  u8    chTemp=0; //temperature channel
  u8    chHum=0;  //humidity
  u8    chPress=0;//pressure
  u8    chLight=0;//light level
  long  ts=0;     //sampled data timestamp
};