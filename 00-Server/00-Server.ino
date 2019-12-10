#include "Arduino.h"
#include "wemos.setup.h"

// HouzControl4 modules
#include "src/Core/HausCore.h"
Devices *devices = devices->getInstance();


//RF link
#include "src/Comm/RF/RFlink.h"
void onRFrxCallback(deviceData devicePacket);
RFlink rfLink(onRFrxCallback);

#include "WebServer.h"
WebServer webServer;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){
    wemosWiFi.connect("hauskontrol");
    webServer.setup();
    rfLink.setup();
    devices->set(server_rf, rfLink.ready);
    Serial.println("-- setup complete ---------------");

///////////////////////////////////////////////////
  Serial.print("deviceCount: ");
  Serial.println(devices->deviceListLen);

};

void loop(){
    wemosWiFi.update();
    rfLink.update();
    if(TaskManager::getInstance()->arePendingTasks()) runTask(); 
};

void runTask(){
    Task task = TaskManager::getInstance()->getNextTask();
    Serial.print("task> ");
    switch (task.command)
    {
    case command_set_device: 
        Serial.println("command_set_device");
        break;

    case command_rf_send: 
        Serial.println("command_rf_send");
        DevicePkt pkt;
        pkt.id=task.device.id;
        pkt.cmd=RFCMD_QUERY;
        pkt.node=task.device.node;
        pkt.payload=task.device.payload;
        rfLink.send(pkt);
        break;

    case command_play_scene: 
        Serial.println("command_play_scene");
        break;
    
    default:
        break;
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RF hook
void onRFrxCallback(deviceData dev){
  devices->set(dev.id, dev.payload);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void devicePrint(Device dev){
  Serial.print("id  \t");
  Serial.println(dev.id, HEX);
  Serial.print("payload\t");
  Serial.println(dev.payload, HEX);
};


