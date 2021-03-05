const ws = new WebSocket('ws://localhost:3000');
ws.binaryType = 'arraybuffer';

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
    console.log('message received');
    A.src = "data:image/jpeg;base64," + message.data;
};