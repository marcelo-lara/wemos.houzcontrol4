#include "RfNode.h"

//RF connected node
RfNode::RfNode(NodeEnm node){
  this->node=node;
  this->status=nodeStatus_unknown;
};

//determine if we are awaiting response from node
bool RfNode::isTimeout(){
  if(this->status!=nodeStatus_awaiting_response) return false;
  return (awaitTimeout<millis());
};

//raise when timeout is reached
void RfNode::onTimeout(){
  status=nodeStatus_lost;
  nextQuery=millis()+5000; //retry in 5s
  Serial.printf("-> RfNode[%i].onTimeout \n", this->node);

};

//update node values
//returns: true when should wait
bool RfNode::onUpdate(){
  //skip update
  if(nextQuery>millis()) return false;

  //setup update
  awaitTimeout=millis()+1000;
  status=nodeStatus_awaiting_response;

  //raise update
  this->raise_updater();
  return true;
};

//data received from node
void RfNode::onAck(){
  status=nodeStatus_ready;
  nextQuery=millis()+RfNodePollInterval;
};
