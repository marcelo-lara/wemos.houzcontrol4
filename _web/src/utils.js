'use strict';
const el=(domstring)=>{
  const html = new DOMParser().parseFromString( domstring , 'text/html');
  return html.body.firstChild;
};

const api={
  _url: 'http://hauskontrol.local/api',
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

  link: ()=>{}
};

