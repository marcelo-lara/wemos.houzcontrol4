'use strict';
const el=(domstring)=>{
  const html = new DOMParser().parseFromString( domstring , 'text/html');
  return html.body.firstChild;
};

const api={
  _url: 'http://hauskontrol.local/api',
  post: (fx='',method='POST',data)=>{
    // Default options are marked with *
    const response = await fetch(api._url+fx, {
      method: method, // *GET, POST, PUT, DELETE, etc.
      mode: 'cors', // no-cors, *cors, same-origin
      cache: 'no-cache', // *default, no-cache, reload, force-cache, only-if-cached
      credentials: 'same-origin', // include, *same-origin, omit
      headers: {'Content-Type': 'application/json'},
      redirect: 'follow', // manual, *follow, error
      referrer: 'no-referrer', // no-referrer, *client
      body: JSON.stringify(data) // body data type must match "Content-Type" header
    });
    return await response.json(); // parses JSON response into native JavaScript objects
  },
  link: ()=>{}
};