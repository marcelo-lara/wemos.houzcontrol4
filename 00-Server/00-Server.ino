#include "Arduino.h"
#include "wemos.setup.h"

//light module fixtue
#define ch1 4
#define ch2 14
#define ch3 12
#define ch4 13


#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "QueueArray.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Light object
class Light {
public:
    int id;
    int gpioOut;
    bool isOn;

    // actions
    void turnOn(){this->set(true);};
    void turnOff(){this->set(false);};
    void toggle(){this->set(!this->isOn);};
    void set(bool _status){
        this->isOn=_status;
        digitalWrite(gpioOut, _status);
        Serial.print("light ");
        Serial.print(this->id, HEX);
        Serial.print(" | ");
        Serial.println(this->isOn?"on":"off");
    };

    // json
    String getJson(){
        String json="{\"id\":";
        json += this->id;
        json += ",\"value\":";
        json +=  this->isOn?"1":"0";
        json += "}";
        return json;
    };

    void doAction(int _cmd, int _payload){
        if(_cmd==1){ //set
            switch (_payload){
                case 0:  this->turnOff(); break;
                case 1:  this->turnOn(); break;
                default:  this->toggle(); break;
            }
        }
    };

    Light(int _id, int _out){
        this->id=_id;
        this->gpioOut=_out;
        pinMode(_out, OUTPUT);
        this->turnOff();
    };
};

Light devices[4] = {
    Light(1, ch1), 
    Light(2, ch2),
    Light(3, ch3), 
    Light(4, ch4)
};
#define deviceCount 4;

enum Command{
    set = 1
};

struct Device {
public:
    u8  id;
	u32 payload;
};

struct Task {
public:
    int command;
    Device device;
};
QueueArray<Task> taskQueue;

void setup(){
    wemosWiFi.connect("houzserver");
    apiSetup();
    


};
void loop(){
    wemosWiFi.update();
    taskManager();


};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// API
void apiSetup(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "HouzNode");
    }); 

    server.on("/api", HTTP_GET, api_get_api);
    server.on("/api", HTTP_POST, nullRequest, NULL, api_post_api);

    server.onNotFound(notFound);
    server.begin();

};

void nullRequest(AsyncWebServerRequest *request){};

void api_get_api(AsyncWebServerRequest *request){
    Serial.println("::GET");
    String jsonRes = "[";
    jsonRes += devices[0].getJson();
    jsonRes += ",";
    jsonRes += devices[1].getJson();
    jsonRes += "]";
    request->send(200, "application/json", jsonRes);
}

void api_post_api(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    Serial.println("::/api");
    if(index){
        request->send(422 , "text/plain", "no body?");
        return;
    };

    //parse device
    Device dev = parseDevice((char*)data);
    if (dev.id==-1) {
      request->send(422, "application/json", "{\"parse\":\"error\"}");
      return;
    };

    //add task
    Task task;
    task.command = 1;
    task.device=dev;
    taskQueue.enqueue(task);

    devicePrint(dev);
    request->send(200, "application/json", "{\"parse\":\"ok\"}");
}

Device parseDevice(String jsonStr){
  Device ret;
  ret.id=-1;

  //decode json
  StaticJsonDocument<500> doc;
  DeserializationError error = deserializeJson(doc, jsonStr);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return ret;
  };

  //parse device
  ret.id=doc["id"];
  ret.payload=doc["payload"];
  
  return ret;
 };

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IO handler
void devicePrint(Device dev){
  Serial.print("id  \t");
  Serial.println(dev.id, HEX);
  Serial.print("payload\t");
  Serial.println(dev.payload, HEX);
};

void taskManager(){
    if(taskQueue.isEmpty()) return; //nothing to do...
    Task task = taskQueue.dequeue();

    for (size_t i = 0; i < 2; i++)
    {
        if(devices[i].id!=task.device.id) continue;
        devices[i].doAction(task.command, task.device.payload);
        break;
    };
    

}
