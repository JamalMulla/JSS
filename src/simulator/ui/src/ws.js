const ws = new WebSocket('ws://localhost:3000');

var A;
window.addEventListener('load', function () {
    A = document.getElementById("A");
});

ws.onopen = () => {
    console.log('Connected');
};

ws.onclose = function (event) {
    console.log('Disconnected');
};

ws.onmessage = (message) => {
    var msg = JSON.parse(message.data);
    document.getElementById(msg.reg).src = "data:image/jpeg;base64," + msg.data;
};
