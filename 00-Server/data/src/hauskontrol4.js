'use strict';
const ui={

  loader:{
    hide: ()=>{
      document.querySelector("div.loader").style.display="none";
    },
    show: ()=>{
      document.querySelector("div.loader").style.display="inline";
    },
    showError: (msg)=>{
      document.querySelector("div.loader>div.loading").style.display="none";
      document.querySelector("div.loader>div.error").style.display="inline";
      ui.loader.retryTimer();
    },
    retryTimer: ()=>{
      ui.loader.retries++;
      if(ui.loader.retries>15){
        ui.loader.retries=0;
        ui.loader.show();
        ui.setup();
      }else{
        document.querySelector("div.loader>div.error").innerText="retry in "+(15-ui.loader.retries);
        setTimeout(ui.loader.retryTimer, 1000);
      }
    },
    retries: 0
  },

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
    //fetch devices from api
    devices.setup()
      .then(ui.weather.setup)
      .then(rooms.setup)
      .then(scenes.setup)
      .then(ui.loader.hide)
      .catch(ui.loader.showError);
    ui.status.setup();
  }
};

//setup
window.onload = ui.setup;