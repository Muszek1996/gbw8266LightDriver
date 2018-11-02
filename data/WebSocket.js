var rainbowEnable = false;
var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
connection.onopen = function () {
    connection.send('Connect ' + new Date());
    console.log("Connected to WebSocketServer");
};
connection.onerror = function (error) {
    console.log('WebSocket Error ', error);
};
connection.onmessage = function (e) {

    if(e.data!=null){
        var data = JSON.parse(e.data);
        if(data.type == "TempSensorReadings"){
            console.log("TempSensorReadings Received");
            if(data.data[0])
                document.getElementById('tempParagraph0').textContent = data.data[0];
            if(data.data[1])
                document.getElementById('tempParagraph1').textContent = data.data[1];
            document.getElementById("tempSlider").value = data.tempSlider;
            document.getElementById('temperatureText').textContent = data.tempSlider;
            document.getElementById("speedSlider").value = data.speedSlider;

        }else{
            console.log('Server: ', e.data);
        }
    }


};
connection.onclose = function () {
    console.log('WebSocket connection closed');
};

function sendTemp () {
    var temp = document.getElementById('tempSlider').value;
    document.getElementById('temperatureText').textContent = temp.toString();
    var obj = { 'type': "tempOnSlider", 'data': temp };
    var str = JSON.stringify(obj);
    console.log(str);
    connection.send(str);
}
function sendSpeed () {
    var temp = document.getElementById('speedSlider').value;
    var obj = { 'type': "speedOnSlider", 'data': temp };
    var str = JSON.stringify(obj);
    console.log(str);
    connection.send(str);
}

function openWindow(){
    var obj = { 'type': "windowAction", 'data': "1" };
    var str = JSON.stringify(obj);
    console.log(str);
    connection.send(str);
}


function closeWindow(){
    var obj = { 'type' : "analogArr" ,'vals' : [0,0,0,0,0,0,0] };
    var str = JSON.stringify(obj);
    console.log(str);
    connection.send(str);
}
function stopWindow(){
    var obj = { 'type' : "analogArr" ,'vals' : [200,200,0,200,200,1024,1024] };
    var str = JSON.stringify(obj);
    console.log(str);
    connection.send(str);
}

function setVal(i){


    var obj = { 'type' : "analogArr" ,'vals' : [document.getElementById('s1').value,
                                                document.getElementById('s2').value,
                                                document.getElementById('s3').value,
                                                document.getElementById('s4').value,
                                                document.getElementById('s5').value,
                                                document.getElementById('s6').value,
                                                document.getElementById('s7').value] };
    var str = JSON.stringify(obj);
    console.log(str);
    connection.send(str);
}

function changeTemp(){
    var temp = document.getElementById('tempSlider').value;
    document.getElementById('temperatureText').textContent = temp.toString();
}