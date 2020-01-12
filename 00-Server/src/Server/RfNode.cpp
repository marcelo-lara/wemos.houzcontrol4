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
  // Serial.printf("\t[%i]RfNode::onTimeout\n", this->node);
  this->setStatus(nodeStatus_lost);
  nextQuery=millis()+60000; //retry in 5s
};

//update node values
//returns: true when should wait
bool RfNode::onUpdate(){
  //skip update
  if(nextQuery>millis()) return false;
  // Serial.printf("\t----[%i]RfNode::onUpdate----\n", this->node);

  //setup update
  awaitTimeout=millis()+1000;
  this->setStatus(nodeStatus_awaiting_response);

  //raise update
  this->raise_updater();
  return true;
};

//data received from node
#define RfNodePollInterval 3600000 //1h

void RfNode::onAck(){
  // Serial.printf("\t[%i]RfNode::onAck\n", this->node);
  this->setStatus(nodeStatus_ready);
  nextQuery=millis() + RfNodePollInterval;
};

void RfNode::setStatus(RfNodeStatus newStatus){
  // if(this->status!=newStatus)
  //   Serial.printf("\t[%i]RfNode::setStatus %i->%i\n", this->node, this->status, newStatus);
  this->status=newStatus;
};

void RfNode::printStatus(){
  Serial.printf("\t[%i]RFnode|status:%i\n", this->node, this->status);
};