#pragma once
#include "Arduino.h"
#include "Model.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Devices Config
// template: NCC  [node|channel]



// Master
#define server_node			0x00    //N0DA000000 ping?
#define server_rf 			0x01    // 0: offline / 1: online 

// Office
#define office_node			  0x01  //N1DC04F0F0
#define office_AC			    0x11 //Air Conditioner on/off
#define office_AC_temp		0x12 //Air Conditioner temperature
#define office_light		  0x13 //ceiling light | N1DC130001
#define office_switchLed	0x14 //wall switch led
#define office_switch  		0x15 //wall switch
#define office_ir			    0x16 //ir 
#define external_light		0x17 //light sensor [0-1024]
#define external_temp		  0x1A //temperature [celsius /100] | N1DA1A0000
#define external_humidity	0x1B //humidity [%] | N1DA1B0000
#define external_pressure	0x1C //pressure [hPa /FIX THIS ((x-900) * 100?)] | N1DA1C0000
#define external_weather	0x1F //all devices | N1DA1F0099

// Suite
#define suite_node		    0x2	 //N2DC02F0F0
#define suite_light		    0x21 //N2DC210001
#define suite_fan         0x22 //N2DC220001
#define suite_AC          0x23 //Air Conditioner on/off | N2DC230001
#define suite_enviroment	0x25 //bme280 sensors | N2DA200000
#define suite_temp			  0x26 
#define suite_humidity  	0x27 
#define suite_pressure		0x28 

// Living
#define living_node       0x3	 //N3DC04F0F0\n
#define living_switch		  0x31

#define living_dicro_1    0x300
#define living_dicro_2    0x301
#define living_dicro_3    0x302
#define living_dicro_4    0x304
#define living_dicro_5    0x305
#define living_dicro_6    0x306
#define living_dicro_7    0x307
#define living_dicro_8    0x308
#define living_main       0x309
#define living_booksh     0x30A
#define living_corner     0x30B
#define living_fx1        0x30C
#define living_fx2        0x30D

//composite devices [legacy]
#define living_rawRender	0x32 //N3DC320099\n 
#define living_mainLight	0x33 //N3DC330002\n - 51 center
#define living_dicroLight	0x34 //N3DC340000\n - 52 2x4 dicro array
#define living_spotLight	0x35 //N3DC350000\n - 53 spotlights
#define living_fxLight		0x36 //N3DC360000\n - 54 guidance leds
#define living_fx			    0x37 //N3DC370000\n - 55 fx 'scene' render

#define living_AC		    0x38 //N3DC380001\n

// scenes lighting fx
#define fx_dicroOn	0x1
#define fx_dicroOff	0x2



//////////////////////////////////////////////////////

// device storage
class Devices{
private:
    static Devices* instance;
    Devices();

public:
    static Devices* getInstance();

    int deviceListLen; //total channels
    Device* get(u8 deviceId);
    void set(u8 deviceId, u32 payload);

    Device deviceList[44]{ 
      {server_node	     ,server_node, 0},
      {server_rf         ,server_node, 0},
      {office_node	     ,office_node, 0},
      {office_AC		     ,office_node, 0},
      {office_AC_temp	   ,office_node, 0},
      {office_light	     ,office_node, 0},
      {office_switchLed  ,office_node, 0},
      {office_switch     ,office_node, 0},
      {office_ir		     ,office_node, 0},
      {external_light	   ,office_node, 0},
      {external_temp	   ,office_node, 0},
      {external_humidity ,office_node, 0},
      {external_pressure ,office_node, 0},
      {external_weather  ,office_node, 0},
      {suite_node		     ,suite_node , 0},
      {suite_light	     ,suite_node , 0},
      {suite_fan		     ,suite_node , 0},
      {suite_AC		       ,suite_node , 0},
      {suite_enviroment  ,suite_node , 0},
      {suite_temp		     ,suite_node , 0},
      {suite_humidity    ,suite_node , 0},
      {suite_pressure	   ,suite_node , 0},
      {living_node       ,living_node, 0},
      {living_switch	   ,living_node, 0},
      {living_rawRender  ,living_node, 0},
      {living_mainLight  ,living_node, 0},
      {living_dicroLight ,living_node, 0},
      {living_spotLight  ,living_node, 0},
      {living_fxLight	   ,living_node, 0},
      {living_fx		     ,living_node, 0},
      {living_dicro_1    ,living_node, 0},
      {living_dicro_2    ,living_node, 0},
      {living_dicro_3    ,living_node, 0},
      {living_dicro_4    ,living_node, 0},
      {living_dicro_5    ,living_node, 0},
      {living_dicro_6    ,living_node, 0},
      {living_dicro_7    ,living_node, 0},
      {living_dicro_8    ,living_node, 0},
      {living_main       ,living_node, 0},
      {living_booksh     ,living_node, 0},
      {living_corner     ,living_node, 0},
      {living_fx1        ,living_node, 0},
      {living_fx2        ,living_node, 0},
      {living_AC		     ,living_node, 0}
    };

  static void printDevice(Device device){
    Serial.print("id  \t");
    Serial.println(device.id, HEX);
    Serial.print("payload\t");
    Serial.println(device.payload, HEX);
  };
};
