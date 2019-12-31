'use strict';

const scenes = {
  NORMAL:  0,
  GOODBYE: 1,
  SLEEP:   2,
  DRAGON:  3,
  WELCOME: 4
}

//device type
const devType = {
  undefined : 0, //not defined device (error!)
  light : 1, //light (use initializer to multiplex)
  env   : 2, //Environment
  fan   : 3, //fan 
  ac    : 4, //air conditioner
  bell  : 5  //door bell
};

//zones
const zones = {
  server  : 0,
  living  : 1,
  suite   : 2,
  office  : 3,
  kitchen : 4,
  door    : 5,
  outside : 6
};

//tasks
const tasks = {
  not_defined    :  0,
  request_update :  1,
  update_device  :  2,
  set_device     :  3,
  play_scene     :  4,
  set_scene      :  5,
  rf_send        :  6,
  ir_send        :  7,
  rf_query       :  8,
  scene_pause    :  9,
  set_on         : 10,
  fan_on         : 11,
  fan_speed      : 12
};

const enumToString= (enm, id)=>{
  for(const i in  enm){
    console.log(i, enm[i], (id==enm[i]?"<<<":""));
    if(id==enm[i]) return i;
  }
};

const devices = {
  list: [
    {id:0x00, name:'server_node', type: 0, on: 0, val: 0, zone: 0, elem: undefined},
    {id:0x01, name:'server_rf', room:0, val: 0}
  ],
  byRoom: (zone)=>{
    let ret = [];
    for (const dev of devices.list) {
      if(dev.zone==zone) ret.push(dev);
    };
    return ret;
  },
  byId: (deviceId)=>{
    for (const dev of devices.list) {
      if(dev.id==deviceId) return dev;
    }
    return undefined;
  },
  setup: ()=>{
    return api.post()
       .then(devices._setup_post);
  },
  _setup_post: (data)=>{
    for(const dev of data.devs){
      switch(dev.type){
        case devType.light: devices.list.push(new Light(dev)); break;
        case devType.fan:   devices.list.push(new Fan(dev));   break;
        default:
          console.log("unhandled type:", dev.type);
          break;
      }
    };
    return devices.list;
  }
};

let main = {
  currentScene: scenes.NORMAL,
  lastUpdate: 0,
  serverapi: "http://hauskontrol.local/api"
};
