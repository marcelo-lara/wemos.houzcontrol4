#include "RFlink.h"
#include "Arduino.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RFcodec* codec;

#define rf_ce   2 //D4
#define rf_csn 15 //D8
RF24 radio(rf_ce,rf_csn);

RFlink::RFlink(RFrxCallback rxEvent){
    raiseRxEvent = rxEvent;
}

void RFlink::setup(){
    ready=false;
    radio.begin();
    radio.setPALevel(RF24_PA_HIGH); //RF24_PA_HIGH | RF24_PA_LOW | RF24_PA_MAX
    radio.setDataRate(RF24_1MBPS);
    radio.enableDynamicAck();
    radio.setCRCLength(RF24_CRC_8);
    radio.setChannel(rfChannel);
    radio.setRetries(20, 10);  

    //pipes setup
    radio.openWritingPipe(rf_server_tx);
    radio.openReadingPipe(1, rf_office_tx);
    radio.openReadingPipe(2, rf_suite_tx);
    radio.openReadingPipe(3, rf_living_tx);

    //debug
    ready = (rfChannel == radio.getChannel()); //test if radio is enabled
    //radio.printDetails();
    Serial.println(ready?"rf online":"rf offline");
}

void RFlink::update(){

	//if radio is not enabled, discard anything
	if (!ready) return;

  //receive buffer
	uint8_t _radioNode;
	if (!radio.available(&_radioNode)) return;

	//get payload
	unsigned long _radioPayLoad;
	while (radio.available()) {
		radio.read(&_radioPayLoad, sizeof(unsigned long));
	}

	//prepare for next packet
	radio.startListening();

	//decode payload
	deviceData dev = codec->decode(_radioPayLoad, _radioNode);
  Serial.print("RF<-\t");
  Serial.print(_radioNode, HEX);
  Serial.print("|");
  Serial.println(_radioPayLoad, HEX);

  //delegate
  raiseRxEvent(dev);
}

bool RFlink::send(DevicePkt dev){
  Serial.print("RF->\t");

	//open write pipe
	uint64_t writeAddress;
  switch (dev.node) {
		case office_node: writeAddress = rf_office_rx; break;
		case suite_node: writeAddress = rf_suite_rx; break;
		case living_node: writeAddress = rf_living_rx; break;
  };
  u32 msg = deviceEncode(dev);

  Serial.print(dev.node, HEX);
  Serial.print("|");
  Serial.print(msg, HEX);
  Serial.print("\t");

  //rf send
  radio.stopListening();
  radio.openWritingPipe(writeAddress);
  bool result = 0;
  result = radio.write(&msg, sizeof(unsigned long), 0);
  radio.startListening();
  Serial.println(result?"ok":"error");
}

unsigned long RFlink::deviceEncode(DevicePkt dev){
	unsigned long retVal = 0xD;
	retVal = (retVal << 4) + dev.cmd;
	retVal = (retVal << 8) + dev.id;
	retVal = (retVal << 16) + dev.payload;
	return retVal;
}