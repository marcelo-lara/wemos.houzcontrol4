'use strict';
const el=(domstring)=>{
  const html = new DOMParser().parseFromString( domstring , 'text/html');
  return html.body.firstChild;
};