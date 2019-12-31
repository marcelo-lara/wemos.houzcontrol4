'use strict';
class Light{
  constructor(dev){
    //core
    this.id=dev.id;
    this.zone=dev.zone;
    this.on=dev.on;
    this.name=dev.name||"Light";

    //build element
    this.elem=el("<div class=\"btn light\"></div>");
    this.elem.textContent = "light[" + this.id +"]";
    if(dev.on) this.elem.classList.add("on");

    //bind actions
    this.elem.addEventListener("click", this.changeState.bind(this), true);

  }

  changeState(){
    this.on=!this.on;
    if(this.on) this.elem.classList.add("on") 
    else this.elem.classList.remove("on") 
    console.log("light|", this.on?"on":"off");
    api.post("/task","POST",{
      task: tasks.set_device,
      id: this.id,
      payload: this.on?1:0
    });
  }
}
