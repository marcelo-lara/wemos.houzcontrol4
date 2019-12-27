#include "RfNodes.h"

int living_dicro[] = {
  living_dicro_1,
  living_dicro_2,
  living_dicro_3,
  living_dicro_4,
  living_dicro_5,
  living_dicro_6,
  living_dicro_7,
  living_dicro_8
};
int living_fxs[] = {
  living_fx1,
  living_fx2
};
int living_spots[] = {
  living_booksh,
  living_corner
};


void RfNodes::setup(){

  //build muxed channels



  //initial query
  this->currentNode=1;
  this->nextAction=RfNodeAction_requestStatus;




};

void RfNodes::update(){
  return;//RBF
  switch (this->nextAction){
  case RfNodeAction_requestStatus:
    Serial.println("::RfNodeAction_requestStatus");
    {
      Packet packet;
      packet.id = suite_enviroment;
      packet.node = node_suite;
      packet.cmd = RFCMD_QUERY;
      packet.payload = 0;
      this->nextAction=RfNodeAction_awaitRequestStatus;
      this->rfLink->send(packet);
    }
    break;
  
  default:
    break;
  }
};

//handle legacy rf channels
void RfNodes::parsePacket(Packet packet){
  switch (packet.id){

//muxed lights
  case living_mainLight:
    Devices::getInstance()->get(living_main)->update(packet.payload==0?0:1);
    break;

  case living_dicroLight: 
    this->demux(packet.payload, 8, living_dicro);
    break;

  case living_spotLight: 
    this->demux(packet.payload, 2, living_spots);
    break;
  
  case living_fxLight: 
    this->demux(packet.payload, 2, living_fxs);
    break;

//uncoded environments
  case external_temp:
    Serial.println("external_temp");
    break;
  case external_humidity: 
    Serial.println("external_humidity");
    break;
  case external_pressure: 
    Serial.println("external_pressure");
    break;

  case suite_temp		:
    {Device* dev = Devices::getInstance()->get(suite_enviroment);
    if(!dev) return;
    static_cast<Environment*>(dev)->update(envenm_temp, rfLink->codec->floatDecode(packet.payload));}    
    break;

  case suite_humidity:
    {Device* dev = Devices::getInstance()->get(suite_enviroment);
    if(!dev) return;
    static_cast<Environment*>(dev)->update(envenm_hum, rfLink->codec->floatDecode(packet.payload));}    
    break;

  case suite_pressure:
    {Device* dev = Devices::getInstance()->get(suite_enviroment);
    if(!dev) return;
    static_cast<Environment*>(dev)->update(envenm_press, rfLink->codec->pressureDecode(packet.payload));}    
    break;

//nodes
  default:
    Serial.printf("RfNodes::parsePacket| unknown reference %i\n", packet.id);
    break;
  };


};


void RfNodes::demux(long payload, int devLen, int* devArray){
  int pos=0;

  //fill values
  while (payload){
    Devices::getInstance()->get(devArray[pos])->update((payload&1)?1:0);
    payload>>=1;
    pos++;
    if(pos>devLen) return;
  };

  //fill empty values
  if(pos<devLen){
    for (int i = pos; i < devLen; i++){
      Devices::getInstance()->get(devArray[i])->update(0);
    };
  }

};