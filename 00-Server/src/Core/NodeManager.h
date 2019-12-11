#pragma once
#include "Devices.h"

enum NodeStatus{
    nodeStatus_unknown = 0,
    nodeStatus_up  = 1,
    nodeStatus_down = 2
};
enum NodeType{
    nodetype_rf = 0,
    nodetype_api  = 1
};

struct Node {
    int  id;
    long lastUpdate;
    NodeStatus status;
    NodeType nodeType;
};

Node nodeList[] = {
    {office_node, 0, nodeStatus_unknown, nodetype_rf},
    {suite_node, 0, nodeStatus_unknown, nodetype_rf},
    {living_node, 0, nodeStatus_unknown, nodetype_rf}
};
