#include "RfNodes.h"

void RfNodes::setup(){

  //initial query
  this->currentNode=1;
  this->nextAction=RfNodeAction_requestStatus;




};

void RfNodes::update(){
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
    Serial.println("living_mainLight");
    break;

  case living_dicroLight: 
    Serial.println("living_dicroLight");
    break;

  case living_spotLight: 
    Serial.println("living_spotLight");
    break;
  
  case living_fxLight: 
    Serial.println("living_fxLight");
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
    Environment* env = static_cast<Environment*>(dev); 
    env->temp=rfLink->codec->floatDecode(packet.payload);
    env->on=true;
    env->ts=millis();}
    break;

  case suite_humidity:
    {Device* dev = Devices::getInstance()->get(suite_enviroment);
    if(!dev) return;
    Environment* env = static_cast<Environment*>(dev); 
    env->hum=rfLink->codec->floatDecode(packet.payload);
    env->on=true;
    env->ts=millis();}    
    break;

  case suite_pressure:
    {Device* dev = Devices::getInstance()->get(suite_enviroment);
    if(!dev) return;
    Environment* env = static_cast<Environment*>(dev); 
    env->press=rfLink->codec->pressureDecode(packet.payload);
    env->on=true;
    env->ts=millis();}    
    break;

//nodes
  default:
    Serial.printf("RfNodes::parsePacket| unknown reference %i\n", packet.id);
    break;
  };


};