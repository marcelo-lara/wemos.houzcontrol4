'use strict';
const ui={

  rooms: {
    list: [
      {id:1, name:"office", selected: false, elem: undefined},
      {id:2, name:"suite"},
      {id:3, name:"living"},
      {id:4, name:"kitchen"}
    ],
    
    bind: ()=>{
      const btn = document.querySelectorAll("div.rooms>div[room]");
      btn.forEach((room)=>{
        const _id=room.getAttribute("room")||-1;
        let _room=ui.rooms.getById(_id);
        if(_room!=undefined){
          room.addEventListener("click", (ev)=>{ui.rooms.set(_id)}, true);
          _room.elem = room;
        }
      });
      ui.rooms.set(2);
    },
    
    set: (roomId)=>{
      let room = ui.rooms.getById(roomId);
      if(room==undefined || room.selected==true) return;
      room.selected=true;

      //select on room menu
      ui.rooms.list.forEach((_room)=>{
        _room.selected=(_room.id==room.id);
        if(_room.selected)
          _room.elem.classList.add("on");
        else
          _room.elem.classList.remove("on");
      });

      //show devices
      const panel = document.querySelector("div.panel");
      panel.innerHTML='';
      devices.byRoom(roomId).forEach(dev=>{
        if(dev.elem) panel.appendChild(dev.elem);
      });

    },
    getById: (roomId)=>{
      for (const room of ui.rooms.list) 
        if(room.id==roomId) return room;
      return undefined;
    }
  },
  setup: ()=>{
    if(typeof devices == 'undefined' || typeof api == 'undefined'){
      setTimeout(ui.setup, 100);
      return;
    }
    devices.setup();   //fetch devices from api
    ui.rooms.bind();   //bind room buttons

  }
};

//setup
(()=>{
  ui.setup();
})();