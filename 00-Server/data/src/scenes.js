'use strict';

class Scene{
  constructor(sc){
    this.id = sc.id||0;
    this.label = sc.label||'scene'+this.id;
    this.zone = sc.zone||0;

    this.elem=el("<div class=\"scene\"></div>");
    this.elem.textContent = "light[" + this.id +"]";
  }

}

const scenes = {
  list: [
    new Scene({id: 3, label: 'hello'}),
    new Scene({id: 2, label: 'goodbye'}),
    new Scene({id: 1, label: 'sleep'}),
    new Scene({id: 4, label: 'dicro on', zone: zones.living}),
    new Scene({id: 5, label: 'dicro off', zone: zones.living})
  ],
  setup: (data)=>{

    return data;
  },
  byZone: (zone)=>scenes.list.filter(x=>x.zone==zone)

}