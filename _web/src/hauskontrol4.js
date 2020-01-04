'use strict';
const ui={

  status: {
    elem: undefined,
    setup: ()=>{
      ui.status.elem=document.querySelector("div.wrapper>div.status");
      if(ui.status.elem)
        ui.status.update();
    },
    update: ()=>{
      var time = new Date();
      var minute = time.getMinutes();
      ui.status.elem.innerText='' + time.getHours() + ((minute < 10) ? ':0' : ':') + minute;
      setTimeout(ui.status.update, 60000);
    }
  },

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
      devices.byZone(roomId).forEach(dev=>{
        if(dev.elem) panel.appendChild(dev.elem);
      });

    },
    getById: (roomId)=>{
      for (const room of ui.rooms.list) 
        if(room.id==roomId) return room;
      return undefined;
    }
  },

  weather: {
    setup: (data)=>{
      ui.weather.envId=37;
      ui.weather.elem=document.querySelector("div.wrapper>div.weather");
      ui.weather.update();
      return data; // return promise data
    },
    update: ()=>{
      const el = devices.byId(ui.weather.envId).elem;
      el.classList.add("weather");
      ui.weather.elem.replaceWith(el);
    },
    envId: 0,
    elem: undefined
  },


  setup: ()=>{
    if(typeof devices == 'undefined' || typeof api == 'undefined'){
      setTimeout(ui.setup, 100);
      return;
    }
    //fetch devices from api
    devices.setup()
      .then(ui.weather.setup);
    ui.status.setup();
    ui.rooms.bind();   //bind room buttons

  }
};

//setup
(()=>{
  ui.setup();
})();