#include "Arduino.h"
#include "wemos.setup.h"

// #include <RF24.h>
// #define rf_ce   2 //D4
// #define rf_csn 15 //D8
// RF24 radio(rf_ce,rf_csn);
#include "src/Comm/RF/RFlink.h"
void onRFrxCallback(deviceData devicePacket);
RFlink rfLink(&taskManager, onRFrxCallback);

#include "AsyncJson.h"
#include "ArduinoJson.h"
const String JSON_OK = "{\"result\":\"ok\"}";

// HouzControl4 modules
#include "src/Core/TaskManager.h"
#include "src/Data/devs.h"
#include "WebServer.h"
WebServer webServer(&taskManager);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct DevicePointer{
    int  id;
    int  nodeId;
    long payload;
};

DevicePointer deviceList[] = {
    {office_node	   , office_node, 0},
    {office_AC		   , office_node, 0},
    {office_AC_temp	   , office_node, 0},
    {office_light	   , office_node, 0},
    {office_switchLed  , office_node, 0},
    {office_switch     , office_node, 0},
    {office_ir		   , office_node, 0},
    {external_light	   , office_node, 0},
    {external_temp	   , office_node, 0},
    {external_humidity , office_node, 0},
    {external_pressure , office_node, 0},
    {external_weather  , office_node, 0},
    {suite_node		   , suite_node,  0},
    {suite_light	   , suite_node,  0},
    {suite_fan		   , suite_node,  0},
    {suite_AC		   , suite_node,  0},
    {suite_enviroment  , suite_node,  0},
    {suite_temp		   , suite_node,  0},
    {suite_humidity    , suite_node,  0},
    {suite_pressure	   , suite_node,  0},
    {living_node       , living_node, 0},
    {living_switch	   , living_node, 0},
    {living_rawRender  , living_node, 0},
    {living_mainLight  , living_node, 0},
    {living_dicroLight , living_node, 0},
    {living_spotLight  , living_node, 0},
    {living_fxLight	   , living_node, 0},
    {living_fx		   , living_node, 0},
    {living_AC		   , living_node, 0}
};

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
    Serial.println("-- setup complete ---------------");

///////////////////////////////////////////////////
    int i;
    for (int i = 0; i < 3; i++){
        Serial.print("--node ");
        Serial.println(nodeList[i].id);
    }
    
};

void loop(){
    wemosWiFi.update();
    rfLink.update();
    if(taskManager.arePendingTasks()) runTask(); 
};

void runTask(){
    Task task = taskManager.getNextTask();
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
void onRFrxCallback(deviceData devicePacket){
    Serial.println(">>onRFrxCallback");
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void devicePrint(Device dev){
  Serial.print("id  \t");
  Serial.println(dev.id, HEX);
  Serial.print("payload\t");
  Serial.println(dev.payload, HEX);
};


