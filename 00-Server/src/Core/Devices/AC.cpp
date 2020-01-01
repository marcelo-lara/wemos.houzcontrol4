#include "AC.h"

  AC::AC(int id, Zone zone, NodeEnm node)
      :Device(id, devtype_ac, zone, node){};

  String AC::toJson(){
    String json = "{\"id\":";
    json += this->id;
    json += ", \"type\":";
    json += this->type;
    json += ", \"on\":";
    json += this->on?1:0;
    json += ", \"temp\":";
    json += this->temp;
    json += ", \"fan\":";
    json += this->fan;
    json += ", \"swing\":";
    json += this->swing;
    json += ", \"mode\":";
    json += this->mode;

    if(this->zone>0){
      json += ", \"zone\":";
      json += this->zone;
    };

    json += "}";
    return json;
  };
  void AC::set(long payload){

  };
  void AC::setOn(bool _on){

  };
  void AC::update(long payload){

  };

