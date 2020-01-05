'use strict';
class Environment{
  constructor(dev){
    //core
    this.id=dev.id;
    this.zone=dev.zone;
    this.on=dev.on;
    this.name=dev.name||"Environment";

    //custom props
    this.temp = dev.temp;
    this.humidity = dev.humidity;
    this.pressure = dev.pressure;
    this.light = dev.light;

    //build element
    this.elem=el("<div class=\"btn env\"><div class=\"temp\"></div><div class=\"humidity\"></div><div class=\"pressure\"></div><div class=\"light\"></div></div>");
    

    this.tempElem= this.elem.querySelector(".temp");
    this.humidityElem= this.elem.querySelector(".humidity");
    this.pressureElem= this.elem.querySelector(".pressure");
    this.lightElem= this.elem.querySelector(".light");
    

    this.renderStatus();
  }

  renderStatus(){
    this.tempElem.textContent = this.temp;
    this.humidityElem.textContent = this.humidity;
    this.pressureElem.textContent = this.pressure;
    this.lightElem.textContent = this.light;
  }

  changeState(){
    this.on=!this.on;
    if(this.on) this.elem.classList.add("on") 
    else this.elem.classList.remove("on") 
    api.addTask(tasks.set_device, this.id, this.on?1:0);
    // console.log("light|", this.on?"on":"off");
  }


}
