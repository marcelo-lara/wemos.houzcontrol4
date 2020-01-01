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
    this.elem=el("<div class=\"env\"><span class=\"temp\" /><span class=\"humidity\" /><span class=\"pressure\" /><span class=\"light\"/></div>");
    

   // this.elems.temp= this.elem.querySelector(".temp");

    this.renderStatus();
  }

  renderStatus(){
  //  this.elems.temp.textContent = this.temp;
  }

  changeState(){
    this.on=!this.on;
    if(this.on) this.elem.classList.add("on") 
    else this.elem.classList.remove("on") 
    api.addTask(tasks.set_device, this.id, this.on?1:0);
    console.log("light|", this.on?"on":"off");
  }


}
