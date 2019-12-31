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

//get devices by multiplexed channel
Device* Devices::getByMux(int muxCh){

  //count matching elements
  int q = 0;
  for (int i = 0; i < listLen; i++){
    if(list[i]->type==devtype_light){

    };
  };
  return NULL;


};


///////////////////////////////////////////////
// json 
String Devices::toJson(){
  String json ="[";
  for (int i = 0; i < listLen; i++)
    json += list[i]->toJson() + ((i+1)==listLen?"":",");
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
