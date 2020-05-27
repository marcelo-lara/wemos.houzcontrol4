#include "Devices.h"

Device* Devices::get(u8 deviceId){
  int i = 0;
  while (i<listLen){
    if(list[i]->id==deviceId)
      return list[i];
    i++;
  }
  Serial.printf("WARN: device 0x%2X not found!!\n", deviceId);
  return {new Device(0, devtype_undefined)};
};

void Devices::set(u8 deviceId, u32 payload){
  Device* dev = get(deviceId);
  if(dev->id!=0) 
    dev->payload = payload;
};

///////////////////////////////////////////////
// json 
String Devices::toJson(){
  if(!toJson_cacheVoid)
    return toJson_cache;

  //rebuild json string    
  toJson_cache ="[";
  for (int i = 0; i < listLen; i++)
    toJson_cache += list[i]->toJson() + ((i+1)==listLen?"":",");
  toJson_cache += "]";
  toJson_cacheVoid=false;
  Serial.println("toJson_cache rebuilt");
  return this->toJson_cache;
};
void Devices::toJsonVoid(){
  toJson_cacheVoid=true;
};

///////////////////////////////////////////////
// instance 
Devices::Devices(){
  this->listLen=(sizeof(list)/sizeof(*list));
};
Devices* Devices::instance = 0;
Devices* Devices::getInstance(){
  if(instance==0) instance=new Devices();
  return instance;
};
