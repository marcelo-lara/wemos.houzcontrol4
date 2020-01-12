'use strict';
const rooms = {
  wrapper: undefined,
  selected: 0,
  list: [
    {id:1, name:"living", label: "Wohnzimmer", selected: false, elem: undefined},
    {id:2, name:"suite", label: "Schlafzimmer"},
    {id:3, name:"office", label: "Büro"},
    {id:4, name:"kitchen", label: "Küche"}
  ],

  setup: ()=>{
    rooms.wrapper=document.querySelector("div.wrapper>div.rooms");
    for (const room of rooms.list) {
      let btn=document.createElement("div");
      btn.className=room.name;
      btn.innerText=room.label||room.name||"room";
      btn.addEventListener("click", ()=>rooms.select(room.id), true);
      room.elem=btn;
      rooms.wrapper.appendChild(room.elem);
    };
    rooms.select(rooms.list[0].id);
  },
  
  select: zone=>{
    if(zone==rooms.selected) return;
    rooms.selected=zone;

    //select new zone
    rooms.wrapper
      .querySelectorAll(".on")
      .forEach(x=>x.classList.remove("on"));
    rooms.list.find(x=>x.id==zone).elem.classList.add("on");

    //place devices
    const panel = document.querySelector("div.panel");
    panel.innerHTML='';
    devices.byZone(zone).forEach(dev=>{
      if(dev.elem) 
        panel.appendChild(dev.elem);
    });
}

}