'use strict';
const el=(domstring)=>{
  const html = new DOMParser().parseFromString( domstring , 'text/html');
  return html.body.firstChild;
};

const api={
   _fetch: async(apifx='',method='POST',data)=>{
    
    const uri=(window.location.href=="http://localhost/"?"http://hauskontrol.local/":window.location.href) + 'api' + apifx;

    let response = await fetch(uri, {
      method: method, // *GET, POST, PUT, DELETE, etc.
      mode: 'cors', // no-cors, *cors, same-origin
      cache: 'default', // *default, no-cache, reload, force-cache, only-if-cached
      credentials: 'same-origin', // include, *same-origin, omit
      headers: {'Content-Type': 'application/json'},
      redirect: 'follow', // manual, *follow, error
      referrer: 'client', // no-referrer, *client
      body: JSON.stringify(data) // body data type must match "Content-Type" header
    });

    let _data = await response.json();
    return _data; // parses JSON response into native JavaScript objects
  },
  addTask: (_task, _id, _payload)=>{
    return api._fetch("/task","POST",{
      task: _task,
      id: _id,
      payload: _payload
    });
  },
  getDeviceList: ()=>{
    return api._fetch().then(data=>{
      console.log(data);
      return data;
    });
  },
  link: ()=>{}
};

