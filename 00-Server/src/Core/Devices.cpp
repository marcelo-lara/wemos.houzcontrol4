#include "Devices.h"

Device* Devices::get(u8 deviceId){
  int i = 0;
  while (i<listLen){
    if(list[i].id==deviceId)
      return &list[i];
    i++;
  }
  Serial.printf("WARN: device %i not found!!", deviceId);
  return {};
};

void Devices::set(u8 deviceId, u32 payload){
  Device* dev = get(deviceId);
  if(dev->id!=0) 
    dev->payload = payload;
};


// lights /////////////////////////////////////
void Devices::lightsSetup(){
  this->lightsLen=(sizeof(lights)/sizeof(*lights));
};
Light* Devices::getLight(int _id){
  int i = 0;
  while (i<lightsLen){
    if(lights[i].id==_id) return &lights[i];
    i++;
  }
  Serial.printf("WARN: device %i not found!!", _id);
  Light l = Light(0);
  return &l;
};


///////////////////////////////////////////////
// json 
String Devices::toJson(){
  int i = 0;
  String json ="[";

  //lights
  for (int i = 0; i < lightsLen; i++)
    json += this->lights[i].toJson() + ", ";

  //fan
  for (int i = 0; i < fansLen; i++)
    json += this->fans[i].toJson() + ", ";

  //environments
  for (int i = 0; i < environmentsLen; i++)
    json += this->environments[i].toJson() + ", ";

  json += "]";
  return json;
};


///////////////////////////////////////////////
// instance 
Devices::Devices(){
  this->lightsSetup();
  this->listLen=(sizeof(list)/sizeof(*list));
};
Devices* Devices::instance = 0;
Devices* Devices::getInstance(){
  if(instance==0) instance=new Devices();
  return instance;
};
