#pragma once
#include "Arduino.h"
#include "../Core/Model.h"
#include "../Core/Channels.h"
#include <RfLink.h>

enum RfNodeStatus{
  unknown,
  ready,
  lost,
  awaiting_response
};

class RfNode{
public:
  RfNode(NodeEnm node){this->node=node;};
  NodeEnm node;
  RfNodeStatus status = unknown;
  long lastContact = 0;
  long lastTry = 0;
  long nextQuery;
  long timeout = 0;
};

enum RfNodeAction{
  RfNodeAction_requestStatus,
  RfNodeAction_awaitRequestStatus
};

class RfNodes{
public:
  RfNodes(RFlink *rfLink){this->rfLink=rfLink;};
  void setup();
  void update();
  void parsePacket(Packet packet);

  int nodesCount = 3;
  RfNode *nodes[3]{
    new RfNode(node_living),
    new RfNode(node_office),
    new RfNode(node_suite)
  };

  int currentNode = 0;
  RfNodeAction nextAction;
  RFlink *rfLink;

};