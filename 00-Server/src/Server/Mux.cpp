#include "Mux.h"
//muxed output proxy
Mux::Mux(int channel, NodeEnm node, int* list, int listLen){
  this->channel=channel;
  this->node=node;
  this->list=list;
  this->listLen=listLen;
};

//set mux out | return new value
long Mux::set(int muxPos, bool on){
  if(on){
    bitSet(this->current, muxPos);
  }else{
    bitClear(this->current, muxPos);
  }
  return this->current;
};

//decode muxed packet into devices
bool Mux::demux(long payload){
  this->current=payload;
  int pos=0;

  //fill values
  while (payload){
    Devices::getInstance()->get(this->list[pos])->decode((payload&1)?1:0);
    payload>>=1;
    pos++;
    if(pos>this->listLen) return true;
  };

  //fill empty values
  if(pos<this->listLen){
    for (int i = pos; i < this->listLen; i++){
      Devices::getInstance()->get(this->list[i])->decode(0);
    };
  }
  return true;
};
