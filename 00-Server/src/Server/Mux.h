#pragma once
#include "Arduino.h"
#include "../Core/Model.h"
#include "../Core/Devices.h"


class Mux {
public:
  Mux(int channel, NodeEnm node, int* list, int listLen);
  long set(int muxPos, bool on);
  bool demux(long payload);

  NodeEnm node; 
  int channel;  //channel

  int* list;
  int listLen;

private:
  long current;

};
