'use strict';
class AC{
  constructor(dev){
    //core
    this.id=dev.id;
    this.zone=dev.zone;
    this.on=dev.on;
    this.name=dev.name||"AC";

    //build element
    this.elem=el("<div class=\"btn ac\"><div class=\"label\">ac</div><div class=\"up\"></div><div class=\"temp\">24</div><div class=\"down\"></div><div class=\"acfan\">F</div><div class=\"swing\">S</div><div class=\"mode\">cool</div></div>");
    this.elem.textContent = this.name;
    if(dev.on) this.elem.classList.add("on");

    //bind actions
    this.elem.addEventListener("click", this.changeState.bind(this), true);
  }

  changeState(){
    this.on=!this.on;
    if(this.on) this.elem.classList.add("on") 
    else this.elem.classList.remove("on") 
//    api.addTask(tasks.set_device, this.id, this.on?1:0);
    console.log(this.constructor.name, "|", this.on?"on":"off");
  }
}
