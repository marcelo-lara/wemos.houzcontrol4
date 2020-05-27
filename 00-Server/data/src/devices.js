'use strict';

//device type
const devType = {
  undefined : 0, //not defined device (error!)
  light : 1, //light
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
    if(id==enm[i]) return i;
  }
};

const devices = {
  list: [],
  byZone: (zone)=>devices.list.filter(x=>x.zone==zone),
  byId: (deviceId)=>devices.list.find(x=>x.id==deviceId),
  setup: ()=>{
    return api.getDeviceList()
       .then(devices._setup_post);
  },
  _setup_post: (data)=>{
    devices.list = [];
    for(const dev of data.devs){
      switch(dev.type){
        case devType.light: devices.list.push(new Light(dev)); break;
        case devType.fan:   devices.list.push(new Fan(dev));   break;
        case devType.env:   devices.list.push(new Environment(dev));   break;
        case devType.ac:    devices.list.push(new AC(dev));    break;
        default:
          console.log("unhandled type:", dev);
          break;
      }
    };
    return devices.list;
  }
};

let main = {
  lastUpdate: 0,
  serverapi: "http://hauskontrol.local/api"
};
