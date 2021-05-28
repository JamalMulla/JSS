document.addEventListener("DOMContentLoaded", function () {

    'use strict';

    var ws = null;

    function start() {

        ws = new WebSocket('ws://localhost:3000');
        ws.onopen = () => {
            document.getElementById("indicator").style.backgroundColor = "green";
            console.log('Connected');
        };
        ws.onmessage = (message) => {
            var msg = JSON.parse(message.data);
            var reg = document.getElementById(msg.reg);
            if (reg !== null) {
                reg.src = "data:image/jpeg;base64," + msg.data;
            }

        };
        ws.onclose = () => {
            document.getElementById("indicator").style.backgroundColor = "red";
            console.log('Disconnected');
            check();
        };

    }

    function check() {
        if (!ws || ws.readyState === WebSocket.CLOSED) start();
    }

    start();
    setInterval(check, 100);


    // const labels = [
    //     'January',
    //     'February',
    //     'March',
    //     'April',
    //     'May',
    //     'June',
    // ];
    // const data = {
    //     labels: labels,
    //     datasets: [{
    //         label: 'My First dataset',
    //         backgroundColor: 'rgb(255, 99, 132)',
    //         borderColor: 'rgb(255, 99, 132)',
    //         data: [0, 10, 5, 2, 20, 30, 45],
    //     }]
    // };
    // const config = {
    //     type: 'line',
    //     data,
    //     options: {}
    // };
    // var ctx = document.getElementById('myChart')
    // var myChart = new Chart(
    //     ctx,
    //     config
    // );

});

