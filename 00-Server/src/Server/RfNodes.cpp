#include "RfNodes.h"
RfNodes::RfNodes(RFlink *rfLink){
  this->rfLink=rfLink;
};

/////////////////////////////////////////////////////

void RfNodes::setup(){

//office updater
  nodes[0]->raise_updater=[](){
    TaskManager::getInstance()->addTask(command_rf_query, office_light,      0, node_office);
    TaskManager::getInstance()->addTask(command_rf_query, external_weather,  0, node_office);
  };

//suite updater
  nodes[1]->raise_updater=[](){
    TaskManager::getInstance()->addTask(command_rf_query, node_suite, 0, node_suite);
  };

//living updater
  nodes[2]->raise_updater=[](){
    TaskManager::getInstance()->addTask(command_rf_query, node_living, 0, node_living);
  };
  nodesCurrent= -1;
};

void RfNodes::update(){
switch (this->rfStatus){
  case RfNodesLinkStatus_paused:
    return;
    break;

  case RfNodesLinkStatus_idle:

    //engage next node
    nodesCurrent++;
    if(nodesCurrent>=nodesCount){
      nodesCurrent=0;

      //dump status
      // Serial.println("-- RfNodes status ------------");
      // for (int i = 0; i < nodesCount; i++)
      //   nodes[i]->printStatus();
      // Serial.println("------------------------------");

      //this->rfStatus=RfNodesLinkStatus_paused;
     return;
    } 

    //await node response?
    this->rfStatus = nodes[nodesCurrent]->onUpdate()?RfNodesLinkStatus_awaiting:RfNodesLinkStatus_idle;
    break;

  case RfNodesLinkStatus_awaiting:
    if(!nodes[nodesCurrent]->isTimeout()) return; //request time is still valid
    // Serial.println("->RfNodesLinkStatus_awaiting:");
    rfStatus = RfNodesLinkStatus_idle;

    nodes[nodesCurrent]->onTimeout();
    break;

  default:
    break;
  }
};

void RfNodes::ackNode(int _node){
  if(_node<1)return; //only rfNodes
   if(this->rfStatus==RfNodesLinkStatus_awaiting && nodes[nodesCurrent]->node==_node){
    nodes[nodesCurrent]->onAck();
    this->rfStatus=RfNodesLinkStatus_idle;
   }else{
     for (int i = 0; i < nodesCount; i++){
       if(nodes[i]->node==_node) nodes[i]->onAck();
     }
   }
};

void RfNodes::sendToMux(int muxId, int muxPos, bool on){
  for (int i = 0; i < muxCount; i++){
    if(this->mux[i]->channel==muxId){
      long payload=mux[i]->set(muxPos, on);
      TaskManager::getInstance()->addTask(command_rf_send, mux[i]->channel, payload, mux[i]->node);
      return;
    }
  };
};



//handle received rf packet
void RfNodes::parsePacket(Packet packet){
  Devices::getInstance()->toJsonVoid();

  //update timers
  ackNode(packet.node);

  //handle legacy rf channels
  switch (packet.id){

  //muxed lights
  case living_mainLight:
    Devices::getInstance()->get(living_main)->decode(packet.payload==0?0:1);
    break;

  case living_dicroLight: {
    this->mux[1]->demux(packet.payload);}
    break;

  case living_spotLight:{
    this->mux[3]->demux(packet.payload);}
    break;

  case living_fxLight: {
    this->mux[2]->demux(packet.payload);}
    break;

//uncoded environments
  case external_temp:  //1A
    {Device* dev = Devices::getInstance()->get(external_weather);
    static_cast<Environment*>(dev)->update(envenm_temp, rfLink->codec->floatDecode(packet.payload));}
    Serial.println("external_temp");
    break;
  case external_humidity: //1B
    Serial.println("external_humidity");
    {Device* dev = Devices::getInstance()->get(external_weather);
    static_cast<Environment*>(dev)->update(envenm_hum, rfLink->codec->floatDecode(packet.payload));}
    break;
  case external_pressure: //1C
    Serial.println("external_pressure");
    {Device* dev = Devices::getInstance()->get(external_weather);
    static_cast<Environment*>(dev)->update(envenm_press, rfLink->codec->pressureDecode(packet.payload));}
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


  default:
    //delegate parse to device (if exists)
    Devices::getInstance()->get(packet.id)->decode(packet.payload);
    break;
  };
};

// demux bits into single devices
void RfNodes::demux(long payload, int devLen, int* devArray){
  int pos=0;

  //fill values
  while (payload){
    Devices::getInstance()->get(devArray[pos])->decode((payload&1)?1:0);
    payload>>=1;
    pos++;
    if(pos>devLen) return;
  };

  //fill empty values
  if(pos<devLen){
    for (int i = pos; i < devLen; i++){
      Devices::getInstance()->get(devArray[i])->decode(0);
    };
  }

};