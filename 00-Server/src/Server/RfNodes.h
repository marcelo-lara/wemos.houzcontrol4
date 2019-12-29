#pragma once
#include "Arduino.h"
#include "../Core/Model.h"
#include "../Core/Channels.h"
#include "../Core/Devices.h"
#include "../Core/TaskManager.h"
#include "RfNode.h"
#include <RfLink.h>

enum RfNodesLinkStatus{
  RfNodesLinkStatus_idle,
  RfNodesLinkStatus_paused,
  RfNodesLinkStatus_awaiting
};

class RfNodes{
public:
  RfNodes(RFlink *rfLink);
  void setup();
  void update();
  void parsePacket(Packet packet);
  void demux(long payload, int devLen, int* devArray);

  //node updater
  int nodesCount = 3;
  RfNode *nodes[3]{
    new RfNode(node_office),
    new RfNode(node_suite),
    new RfNode(node_living)
  };
  RfNodesLinkStatus rfStatus = RfNodesLinkStatus_idle;
  int nodesCurrent = 0;
  void ackNode(int node);

  RFlink *rfLink;
};