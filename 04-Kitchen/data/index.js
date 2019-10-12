let request = obj => {
  return new Promise((resolve, reject) => {
    let xhr = new XMLHttpRequest();
    xhr.open(obj.method || "GET", obj.url);
    if(obj.method=="POST"){
      xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    };
    if (obj.headers) {
      Object.keys(obj.headers).forEach(key => {
        xhr.setRequestHeader(key, obj.headers[key]);
      });
    }
    xhr.onload = () => {
      if (xhr.status >= 200 && xhr.status < 300) {
        resolve(xhr.response);
      } else {
        reject(xhr.statusText);
      }
    };
    xhr.onerror = () => reject(xhr.statusText);
    xhr.send(obj.body);
  });
};

const ui = {
  apiUrl: "/api",
  setup: ()=>{
    ui.devices = document.querySelectorAll("div.panel>div");
    let devId=1;
    for (const dev of ui.devices) {
      const a = devId;
      dev.addEventListener("click", ()=>{
        request({url:ui.apiUrl, method:"POST", body: JSON.stringify({id: a, payload: dev.classList.contains("on")?0:1})})
        .then(()=>{ui.update()});
      });
      devId++;
    }

    ui.update();
  },
  update: () => {
    request({url:ui.apiUrl})
      .then(JSON.parse)
      .then(ui.render);
  },
  render: (data) => {
    for (const dev of data) {
      const eDev = ui.devices[dev.id-1];
      eDev.payload=dev.value;
      if(dev.value==1)
        eDev.classList.add("on");
      else
        eDev.classList.remove("on");
    }
  },
  devices: []
};

(() => {
  console.log("::start");
  ui.setup();
})();