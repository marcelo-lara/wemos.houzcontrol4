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

  weather: {
    setup: (data)=>{
      ui.weather.envId=31;//external temp
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
      .then(ui.weather.setup)
      .then(rooms.setup);
    ui.status.setup();
  }
};

//setup
(()=>{
  ui.setup();
})();