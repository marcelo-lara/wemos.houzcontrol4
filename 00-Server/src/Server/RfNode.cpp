#include "RfNode.h"

//RF connected node
RfNode::RfNode(NodeEnm node){
  this->node=node;
  this->status=nodeStatus_unknown;
};

//determine if we are awaiting response from node
bool RfNode::isTimeout(){
  //if(this->status!=nodeStatus_awaiting_response) return false;
  return (awaitTimeout<millis());
};

//raise when timeout is reached
void RfNode::onTimeout(){
  // Serial.printf("[%i]RfNode::onTimeout\n", this->node);
  this->setStatus(nodeStatus_lost);
  nextQuery=millis()+20000; //retry in 5s
};

//update node values
//returns: true when should wait
bool RfNode::onUpdate(){
  //skip update
  if(nextQuery>millis()) return false;

  //setup update
  awaitTimeout=millis()+1000;
  this->setStatus(nodeStatus_awaiting_response);

  //raise update
  this->raise_updater();
  return true;
};

//data received from node
void RfNode::onAck(){
  // Serial.printf("[%i]RfNode::onAck\n", this->node);
  this->setStatus(nodeStatus_ready);
  nextQuery=millis()+RfNodePollInterval;
};

void RfNode::setStatus(RfNodeStatus newStatus){
  // Serial.printf("\t[%i]RfNode::setStatus %i->%i\n", this->node, this->status, newStatus);
  this->status=newStatus;
};

void RfNode::printStatus(){
  Serial.printf("RFnode|%i - status:%i\n", this->node, this->status);
};