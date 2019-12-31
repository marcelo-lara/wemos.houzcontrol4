#pragma once
#include "Arduino.h"
#include "../Core/Model.h"


enum RfNodeStatus{
  nodeStatus_unknown,
  nodeStatus_ready,
  nodeStatus_lost,
  nodeStatus_awaiting_response
};

#define RfNodePollInterval 3600000 //1h
typedef void (*RfNode_updater)();
class RfNode{
public:
  RfNode(NodeEnm node);
  NodeEnm node;
  
  //node status
  RfNodeStatus status = nodeStatus_unknown;
  long nextQuery = 0;
  long awaitTimeout = 0;
  void printStatus();

  //status changers
  void onAck();
  void onTimeout();
  bool onUpdate();
  bool isTimeout();
  RfNode_updater raise_updater;
  void setStatus(RfNodeStatus newStatus);
};