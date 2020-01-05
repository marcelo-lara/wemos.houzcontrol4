'use strict';
class AC{
  constructor(dev){
    //core
    this.id=dev.id;
    this.zone=dev.zone;
    this.on=dev.on;
    this.name=dev.name||"AC";

    //ac properties
    this.temp=dev.temp||23;
    this.fan=dev.fan||0;
    this.swing=dev.swing||0;
    this.mode=dev.mode||0; //0:cool|1:heat

    //build element
    this.elem=el("<div class=\"btn ac\"><div class=\"label\"></div><div class=\"up\"></div><div class=\"temp\"></div><div class=\"down\"></div><div class=\"acfan\"></div><div class=\"swing\"></div><div class=\"mode\"></div></div>");
    this.elem.querySelector(".label").textContent = this.name;
    if(dev.on) this.elem.classList.add("on");

    //ac elements
    this.elements = {
      main: undefined,
      temp: undefined,
      fan: undefined,
      swing: undefined,
      mode: undefined
    };

    //bind actions
    this.elements.temp=this.elem.querySelector(".temp");
    this.elem.querySelector(".up").addEventListener("click", this.temperature_up.bind(this), false);
    this.elem.querySelector(".down").addEventListener("click", this.temperature_down.bind(this), false);

    this.elements.fan = this.elem.querySelector(".acfan");
    this.elements.fan.addEventListener("click", this.fan_change.bind(this), false);

    this.elements.swing=this.elem.querySelector(".swing");
    this.elements.swing.addEventListener("click", this.swing_change.bind(this), false);

    this.elements.mode=this.elem.querySelector(".mode");
    this.elements.mode.addEventListener("click", this.mode_change.bind(this), false);

    this.elem.addEventListener("click", this.changeState.bind(this), false);

    this.renderStatus();
  }

  changeState(){
    this.on=!this.on;
    this.renderStatus();
  }

  temperature_up(event){
    event.stopPropagation();
    this.temp++;
    if(this.temp>25){this.temp=25;return;};
    this.renderStatus();
  }
  temperature_down(event){
    event.stopPropagation();
    this.temp--;
    if(this.temp<18){this.temp=18;return;};
    this.renderStatus();
  }

  fan_change(event){
    event.stopPropagation();
    this.fan = !this.fan;
    this.renderStatus();
  }
  swing_change(event){
    event.stopPropagation();
    this.swing = !this.swing;
    this.renderStatus();
  }
  mode_change(event){
    event.stopPropagation();
    this.mode = !this.mode;
    this.renderStatus();
  }

  renderStatus(){
    this.elements.temp.innerText=this.temp;
    this.setStatus(this.elem, this.on);
    this.setStatus(this.elements.fan, this.fan!=0);
    this.setStatus(this.elements.swing, this.swing!=0);
    this.elements.mode.innerText = this.mode==0?"cool":"heat";
    console.log(this.constructor.name, "|", this.on?"on":"off", "| temp:", this.temp);
  }

  setStatus(el, st){
    if(st)
      el.classList.add("on")
    else
      el.classList.remove("on")
  }

}
