#include "Arduino.h"
#include "wemos.setup.h"

//light module fixtue
#define ch1 4
#define ch2 14
#define ch3 12
#define ch4 13

#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

#include <ESP8266HTTPClient.h>
bool annouce_set;
String announce_server = "";

#include "AsyncJson.h"
#include "ArduinoJson.h"
const String JSON_OK = "{\"result\":\"ok\"}";
const String JSON_ERR = "{\"result\":\"ok\"}";

// HouzControl4 modules
#include "src/Light.h"
#include "src/TaskManager.h"

#include "src/Button.h"
Button button;

// total devices on node
#define deviceCount 4

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Light object
Light devices[deviceCount] = {
    Light(1, ch1),
    Light(2, ch2),
    Light(3, ch3),
    Light(4, ch4)};

void setup()
{
  wemosWiFi.connect("houzkitchen");
  button.init(5, handleButtonClick);
  for (size_t i = 0; i < deviceCount; i++)
    devices[i].onLightStatusChange = handleLightStatusChange;
  apiSetup();
};

void loop()
{
  wemosWiFi.update();
  button.update();
  taskWorker();
};

void taskWorker()
{
  if (!taskManager.arePendingTasks()) return; 
  Task task = taskManager.getNextTask();

  switch (task.command){
  case command_set_scene:
    renderScene(task.device.payload);
    break;

  case command_set_device:
    getLight(task.device.payload).doAction(task.command, task.device.payload);
    break;

  case command_post_to_server:
    announce_send(task);
    break;
  
  default:
    Serial.printf("command [%i] not handled\n", task.command);
    break;
  }
};

Light getLight(int id){
  for (size_t i = 0; i < deviceCount; i++){
    if (devices[i].id == id) 
      return devices[i];
  };
  return Light(-1,-1);
}

// delegates
void handleButtonClick(int btnClickType)
{
  switch (btnClickType)
  {
  case buttonEv_click:
    taskManager.addTask(command_set_device, 1, -1);
    taskManager.addTask(command_post_to_server, 1, -1);
    break;
  case buttonEv_dblclick:
    taskManager.addTask(command_set_device, 2, -1);
    break;
  case buttonEv_longpress:
    taskManager.addTask(command_set_scene, 0, scene_sleep);
    break;
  }
};

void handleLightStatusChange(int deviceId, int newStatus)
{
  Serial.print("light ");
  Serial.print(deviceId);
  Serial.print("\t");
  Serial.println(newStatus ? "on" : "off");
  wemosWiFi.blink();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// API
void apiSetup()
{
  server.on("/api", HTTP_GET, api_get_api);
  server.on("/api", HTTP_POST, nullRequest, NULL, api_post_api);


  //FS
  if (SPIFFS.begin()){
    server.serveStatic("/", SPIFFS, "/index.html");
  }
  else{
    Serial.println("An Error has occurred while mounting SPIFFS");
  }
  server.onNotFound(notFound);
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  //enable cors
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  DefaultHeaders::Instance().addHeader("Pragma", "no-cache");
  DefaultHeaders::Instance().addHeader("Expires", "0");
  DefaultHeaders::Instance().addHeader("Server", "HouzControl/4.0.0");

  server.begin();
};

void notFound(AsyncWebServerRequest *request){request->send((request->method() == HTTP_OPTIONS) ? 200 : 404);};
void nullRequest(AsyncWebServerRequest *request){};

void api_get_api(AsyncWebServerRequest *request){
  String jsonRes = "[";
  for (size_t i = 0; i < deviceCount; i++)
  {
    jsonRes += devices[i].getJson();
    jsonRes += (i + 1 < deviceCount) ? "," : "";
  };
  jsonRes += "]";

  request->send(200, "application/json", jsonRes);
}


/////////////////////////////////////
// API
void api_post_api(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  if (index){
    request->send(422, "text/plain", "no body?");
    return;
  };

  //decode json
  StaticJsonDocument<500> doc;
  DeserializationError error = deserializeJson(doc, (char *)data);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    request->send(422, "application/json", "{\"result\":\"error\"}");
    return;
  };

  //set scene shortcut
  int scene = doc["scene"];
  if (scene > 0){
    request->send(200, "application/json", JSON_OK);
    taskManager.addTask(command_set_scene, 0, scene);
    return;
  }

  //set server shortcut
  String serverUri = doc["server"];
  if (serverUri.length() > 0){
    request->send(200, "application/json", JSON_OK);
    announce_server=serverUri;
    annouce_set=true;
    Serial.println(serverUri);
    return;
  }

  //set device
  Device dev;
  dev.id = doc["id"];
  dev.payload = doc["payload"];
  if (!(dev.id == 0 && dev.payload == 0))
    taskManager.addTask(command_set_device, dev);

  //send response
  request->send(200, "application/json", JSON_OK);
}

/////////////////////////////////////
// Announce to server
void announce_send(Task task){
  if(!annouce_set) return;

  WiFiClient client;
  HTTPClient httpClient;
  httpClient.begin(client, announce_server);
  int httpCode = httpClient.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("[HTTP] POST... failed, error: %s\n", httpClient.errorToString(httpCode).c_str());
    annouce_set=false;
  };

  Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  const String& payload = httpClient.getString();
  Serial.println("received payload:\n<<");
  Serial.println(payload);
  Serial.println(">>");
  
};

/////////////////////////////////////
// Scenes
void renderScene(int sceneId)
{
  SceneEnm scene = (SceneEnm)sceneId;
  switch (scene)
  {
  case scene_hello:
    Serial.println("renderScene(scene_hello)");
    break;
  case scene_sleep:
    Serial.println("renderScene(scene_sleep)");
    allLightsOff();
    break;
  case scene_goobye:
    Serial.println("renderScene(scene_goobye)");
    allLightsOff();
    break;
  }
};

void allLightsOff(){
  for (size_t i = 0; i < deviceCount; i++)
    taskManager.addTask(command_set_device, i + 1, 0);
}

