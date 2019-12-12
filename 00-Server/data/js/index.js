(()=>{
    const bg = document.getElementById("bg");
    bg.oncanplay = function() {
        bg.muted = true;
        bg.play();
    };
    console.log("ready!");
})();