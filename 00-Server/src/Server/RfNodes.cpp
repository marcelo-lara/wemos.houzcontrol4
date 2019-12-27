#include "RfNodes.h"

void RfNodes::setup(){

  //initial query
  this->currentNode=0;
  this->nextAction=RfNodeAction_requestStatus;




};

void RfNodes::update(){
  switch (this->nextAction){
  case RfNodeAction_requestStatus:
    Serial.println("::RfNodeAction_requestStatus");
    {
      Packet packet;
      packet.id = this->nodes[this->currentNode]->node;
      packet.node = packet.id;
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

//nodes
  default:
    Serial.printf("RfNodes::parsePacket| unknown reference %i\n", packet.id);
    break;
  };


};