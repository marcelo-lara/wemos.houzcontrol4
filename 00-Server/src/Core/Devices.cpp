#include "Devices.h"

Device* Devices::get(u8 deviceId){
  int i = 0;
  while (i<listLen){
    if(list[i]->id==deviceId)
      return list[i];
    i++;
  }
  Serial.printf("WARN: device %i not found!!", deviceId);
  Device d = Device(-1);
  return &d;
};

void Devices::set(u8 deviceId, u32 payload){
  Device* dev = get(deviceId);
  if(dev->id!=0) 
    dev->payload = payload;
};

///////////////////////////////////////////////
// json 
String Devices::toJson(){
  int i = 0;
  String json ="[";
  for (int i = 0; i < listLen; i++)
    json += list[i]->toJson() + ", ";
  json += "]";
  return json;
};

///////////////////////////////////////////////
// instance 
Devices::Devices(){
  // this->lightsSetup();
  this->listLen=(sizeof(list)/sizeof(*list));
};
Devices* Devices::instance = 0;
Devices* Devices::getInstance(){
  if(instance==0) instance=new Devices();
  return instance;
};
