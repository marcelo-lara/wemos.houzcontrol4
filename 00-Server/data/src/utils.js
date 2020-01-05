'use strict';
const el=(domstring)=>{
  const html = new DOMParser().parseFromString( domstring , 'text/html');
  return html.body.firstChild;
};

const api={
   request: (url, method, data)=>{
    url = 'http://192.168.1.16/api/' + url;
    return new Promise(function (resolve, reject) {
      var xhr = new XMLHttpRequest();
      xhr.open(method, url);
      xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
      xhr.onload = function () {
        if (this.status >= 200 && this.status < 300) {
          resolve(JSON.parse(xhr.response));
        } else {
          reject({
            status: this.status,
            statusText: xhr.statusText
          });
        }
      };
      xhr.onerror = function () {
        reject({
          status: this.status,
          statusText: xhr.statusText
        });
      };
      xhr.send(JSON.stringify(data));
    });
  },
     
   
   post: async(apifx='',method='POST',data)=>{
    // Default options are marked with *
    let response = await fetch(api._url + apifx, {
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
    return api.request("task","POST",{
      task: _task,
      id: _id,
      payload: _payload
    });
  },
  link: ()=>{}
};

