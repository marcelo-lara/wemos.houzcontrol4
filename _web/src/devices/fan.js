'use strict';
class Fan{
  constructor(dev){
    //core
    this.id=dev.id;
    this.zone=dev.zone;
    this.on=dev.on;

    //build element
    this.elem=el("<div class=\"btn fan\"><div>fan</div><div class=\"up\"></div><div class=\"speed\"><span>-</span></div><div class=\"down\"></div></div>");

    if(dev.on) this.elem.classList.add("on");

    //fan properties
    this.speedMax = 4;
    this.speedMin = 1;
    this.speed = dev.speed;
    this.speedElem = this.elem.querySelector(".speed span");
    this.speedElem.textContent = dev.speed;

    //bind fan actions
    this.elem.querySelector(".up").addEventListener("click", this.speedUp.bind(this), false);
    this.elem.querySelector(".down").addEventListener("click", this.speedDown.bind(this), false);
    this.elem.addEventListener("click", this.changeOnState.bind(this), false);
  }
  
  speedUp(event){
    event.stopPropagation();
    this.speed++;
    if(this.speed>this.speedMax) this.speed=this.speedMax;
    this.speedElem.textContent = this.speed;
  }
  speedDown(event){
    event.stopPropagation();
    this.speed--;
    if(this.speed==0 && this.on) this.changeOnState();
    if(this.speed<this.speedMin) this.speed=this.speedMin;
    this.speedElem.textContent = this.speed;
  }

  changeOnState(event){
    this.on=!this.on;
    if(this.on) this.elem.classList.add("on") 
    else this.elem.classList.remove("on") 
  }

}
