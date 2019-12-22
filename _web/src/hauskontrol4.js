const ui={
  
  device: {
    build: ()=>{
      devices.list.forEach(dev=>{
      switch (dev.type) {
        
        case devType.Light:
          dev.elem = el("<div class=\"btn light\"></div>");
          dev.elem.textContent=dev.name;
          if(dev.val==1) dev.elem.classList.add("on");
          dev.elem.addEventListener("click", ()=>{
            console.log("change light status..", dev);
          }, true);
          break;
      
        case devType.Fan:
          //template
          dev.elem = el("<div class=\"btn fan\"><div>fan</div><div class=\"up\"></div><div class=\"speed\"><span>-</span></div><div class=\"down\"></div></div>");
          dev.espeed = dev.elem.querySelector(".speed span");

          //setup
          dev._on=dev.val>0;
          dev.espeed.innerText=dev.val;
          if(dev._on){
            dev.elem.classList.add("on");
            dev._speed=dev.val;
          } 

          // bindings //
          //speed up
          dev.elem.querySelector(".up").addEventListener("click", ()=>{
            dev.speed++;
            if(dev.speed>4){dev.speed=4; dev.val=4;}
            dev.espeed.innerText=dev.speed;
          });
          //speed down
          dev.elem.querySelector(".down").addEventListener("click", ()=>{
            dev.speed--;
            if(dev.speed<1){
              //turn off    
              dev.speed=1; dev.val=0;dev.elem.classList.remove("on");
            }
            dev.espeed.innerText=dev.speed;
            });
          //on/off
          dev.elem.querySelector("div:first-child").addEventListener("click", ()=>{
            //switch state
            if(dev._on){
              dev._on=false;
              dev.val=0;
              dev.elem.classList.remove("on");
            }else{
              dev._on=true;
              dev.val=dev._speed;
              dev.elem.classList.add("on");
            };
          });
          break;

        default:
          break;
      }

      });
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
      console.log("set room>",roomId);

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
    if(typeof devices == 'undefined'){
      setTimeout(ui.setup, 100);
      return;
    }

    ui.device.build(); //render device elements
    ui.rooms.bind();   //bind room buttons

  }
};



//setup
(()=>{ui.setup();})();