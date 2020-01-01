#pragma once
#include "Arduino.h"
#include "../Model.h"
#include "../TaskManager.h"

class AC : public Device {
  AC(int id, Zone zone, NodeEnm node);
  String toJson();
  void set(long payload);//device ext
  void setOn(bool _on);
  void update(long payload);

  bool  on   =0;  
  int   temp =23; //default temp
  bool  fan  =1;  //fan on 


};