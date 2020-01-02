#pragma once
#include "Arduino.h"
#include "../Core/Model.h"
#include "../Core/Channels.h"
#include "../Core/Devices.h"
#include "../Core/TaskManager.h"
#include "RfNode.h"
#include "Mux.h"
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
  void sendToMux(int muxCh, int pos, bool on);

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

  //muxed outputs
  int muxCount = 4;
  Mux *mux[4] = {
    new Mux(living_mainLight,  node_living, (int[2]){living_main, 0}, 1),
    new Mux(living_dicroLight, node_living, (int[8]){living_dicro_1, living_dicro_2, living_dicro_3, living_dicro_4, living_dicro_5, living_dicro_6, living_dicro_7, living_dicro_8}, 8),
    new Mux(living_fxLight,    node_living, (int[2]){living_fx1, living_fx2}, 2),
    new Mux(living_spotLight,  node_living, (int[2]){living_booksh,living_corner}, 2)    
  };

  RFlink *rfLink;
};