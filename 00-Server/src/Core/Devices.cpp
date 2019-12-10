#include "Devices.h"

Device* Devices::get(u8 deviceId){
  int i = 0;
  while (i<deviceListLen){
    if(deviceList[i].id==deviceId)
      return &deviceList[i];
    i++;
  }
  Serial.printf("WARN: device %i not found!!", deviceId);
  return {};
};

void Devices::set(u8 deviceId, u32 payload){
  get(deviceId)->payload = payload;
};

// instance ///////////////////////////////////
Devices::Devices(){
  this->deviceListLen=(sizeof(deviceList)/sizeof(*deviceList));
};
Devices* Devices::instance = 0;
Devices* Devices::getInstance(){
  if(instance==0) instance=new Devices();
  return instance;
};
