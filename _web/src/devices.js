'use strict';
const scenes = {
  NORMAL:  0,
  GOODBYE: 1,
  SLEEP:   2,
  DRAGON:  3,
  WELCOME: 4
}

const devType ={
  Light: 0,
  Ac: 1,
  Fan: 2,
  Environment: 3,
  Scene: 4
};

const devices = {
  list: [
    {id:0x00, name:'server_node', room:0, val: 0, elem: undefined},
    {id:0x01, name:'server_rf', room:0, val: 0},
    {id:0x11, name:'AC', room:1, val: 0, type: devType.Ac, temp: 0},
    {id:0x13, name:'main', room:1, val: 1, type: devType.Light},
    {id:0x1F, name:'enviroment', room:1, val: 0, temp: 0, humidity: 0, pressure: 0, light: 0},
    {id:0x21, name:'main', room:2, val: 0, type: devType.Light},
    {id:0x22, name:'fan', room:2, val: 3, type: devType.Fan, speed: 3, _speed: 3, _on: true, _max:4, _min:1},
    {id:0x23, name:'AC', room:2, val: 0, type: devType.Ac, temp: 0},
    {id:0x25, name:'enviroment', room:2, val: 0, temp: 0, humidity: 0, pressure: 0, light: 0},
    {id:0x90, name:'d1', room:3, val: 0, type: devType.Light},
    {id:0x91, name:'d2', room:3, val: 0, type: devType.Light},
    {id:0x92, name:'d3', room:3, val: 0, type: devType.Light},
    {id:0x94, name:'d4', room:3, val: 0, type: devType.Light},
    {id:0x95, name:'d5', room:3, val: 0, type: devType.Light},
    {id:0x96, name:'d6', room:3, val: 0, type: devType.Light},
    {id:0x97, name:'d7', room:3, val: 0, type: devType.Light},
    {id:0x98, name:'d8', room:3, val: 0, type: devType.Light},
    {id:0x99, name:'main', room:3, val: 0, type: devType.Light},
    {id:0x9A, name:'booksh', room:3, val: 0, type: devType.Light},
    {id:0x9B, name:'corner', room:3, val: 0, type: devType.Light},
    {id:0x9C, name:'fx1', room:3, val: 0, type: devType.Light},
    {id:0x9D, name:'fx2', room:3, val: 0, type: devType.Light},
    {id:0x38, name:'AC', room:3, val: 0, type: devType.Ac, temp: 0}
  ],
  byRoom: (roomId)=>{
    let ret = [];
    for (const dev of devices.list) {
      if(dev.room==roomId) ret.push(dev);
    };
    return ret;
  },
  byId: (deviceId)=>{
    for (const dev of devices.list) {
      if(dev.id==deviceId) return dev;
    }
    return undefined;
  }
};

let main = {
  currentScene: scenes.NORMAL,
  lastUpdate: 0,
  serverapi: "http://hauskontrol.local/api"
};
