"use strict";
const ui = {
  table: undefined,
  setup: ()=>{
    if(typeof devices == 'undefined' || typeof api == 'undefined'){
      setTimeout(ui.setup, 100);
      return;
    }
    ui.table = document.getElementById("device_list");
    devices.setup().then(ui.renderlist);   //fetch devices from api
  },
  renderlist: data=>{
    for (const dev of data) {
      console.log(dev);
      let row = document.getElementById("row_template").cloneNode(true);
      row.style="";
      row.querySelector("td.id").innerText = dev.id;
      row.querySelector("td.type").innerText = dev.constructor.name;
      row.querySelector("td.zone").innerText = enumToString(zones, dev.zone);
      row.querySelector("td.name").innerText = dev.name;
      row.querySelector("td.on").innerText = dev.on;
      if(dev.elem)
        row.querySelector("td.html").appendChild(dev.elem);

      ui.table.appendChild(row);
    };


  }
};


(()=>{
  ui.setup();
})();