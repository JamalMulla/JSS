document.addEventListener("DOMContentLoaded", function() {

    'use strict';

    var ws = null;

    function start(){

        ws = new WebSocket('ws://localhost:3000');
        ws.onopen = () => {
            document.getElementById("indicator").style.backgroundColor = "green";
            console.log('Connected');
        };
        ws.onmessage = (message) => {
            var msg = JSON.parse(message.data);
            document.getElementById(msg.reg).src = "data:image/jpeg;base64," + msg.data;
        };
        ws.onclose = () => {
            document.getElementById("indicator").style.backgroundColor = "red";
            console.log('Disconnected');
            check();
        };

    }

    function check(){
        if(!ws || ws.readyState === WebSocket.CLOSED) start();
    }

    start();
    setInterval(check, 100);

});