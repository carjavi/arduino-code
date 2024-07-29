
// npm i serialport
// npm i @serialport/parser-byte-length
// npm i @serialport/parser-readline
// arduino chino manufacturer = la86


const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

// Función para encontrar el puerto del dispositivo y determinar su tipo
async function findDevicePort() {
  const ports = await SerialPort.list();
  for (const port of ports) {
    if (port.manufacturer) {
        console.log("manufacturer: " + port.manufacturer);
        var manufacturer = port.manufacturer;
      if (port.manufacturer.includes('Arduino')) {
        return { path: port.path, type: 'Arduino-original' };
      } else if (port.manufacturer.includes('Silicon Labs') || port.manufacturer.includes('espressif')) {
        return { path: port.path, type: 'ESP32' };
      }else if (manufacturer.search("la86")) {
        return { path: port.path, type: 'Arduino' };
      }
    }
  }
  throw new Error('Arduino o ESP32 no encontrado');
}

async function start() {
  try {
    const deviceInfo = await findDevicePort();
    console.log(`Conectando al ${deviceInfo.type} en el puerto: ${deviceInfo.path}`);
    
    const port = new SerialPort({ path: deviceInfo.path, baudRate: 115200 });
    const parser = port.pipe(new ReadlineParser());

    parser.on("data", (data) => {
      console.log("InData: ", data);
    });

    port.on('open', function() {
      console.log(`${deviceInfo.type} puerto abierto`);
    });

    port.on("error", function(err) {
      console.log("Error: ", err.message);
    });

  } catch (error) {
    console.error("Error: ", error.message);
  }
}

start();




/*

const { SerialPort, SerialPortStream } = require('serialport');

// Función para encontrar el puerto del Arduino
async function findArduinoPort() {
  const ports = await SerialPort.list();
  for (const port of ports) {
    if (port.manufacturer && port.manufacturer.includes('Arduino')) {
      //return port.path;
      console.log(`Conectando al Arduino en el puerto: ${port.path}`);
    }
    if (port.manufacturer =='la86') {
        //return port.path;
        console.log(`Conectando al Arduino en el puerto: ${port.path}`);
    }


    console.log("manufacturer: " + port.manufacturer);
  }
  console.log('Arduino no encontrado');
}

//const arduinoPort = await findArduinoPort();
findArduinoPort();

*/

/*
var SerialPort = require('serialport').SerialPort;
var ArduinoUSBPort;

SerialPort.list().then(function(ports){
    var path_arduino;
    ports.forEach(function(port){
        //console.log("Port: ", port); // muestra todos loe puertos en la Raspberry
        if(port.manufacturer != undefined){ // solo muestra los puertos que tengan un dispositivo conectado
            path_arduino = port.path;
            //console.log("path: ", path_arduino); Muestra el path del puerto con conexion 
                ArduinoUSBPort = new SerialPort({
                path: path_arduino, 
                baudRate: 115200
            });
            ArduinoUSBPort.on('error', function(err) {
                console.log('error arduino port: ', err.message);
                msg = "error arduino port:" + err.message;
                ws.send(msg.toString());
            })
            ArduinoUSBPort.on('open', function(){
                console.log(`Control Camera is now connected at port (arduino): ${port.path}`);
                msg = `Control Camera is now connected at port (arduino): ${port.path}`;
                ws.send(msg.toString());
            })
        }
    })
    if(path_arduino == null){
        console.log("Camera Control Not Connected");
        msg = "camera control not connected (arduino)";
        ws.send(msg.toString());
    }
});
*/


/*
var SerialPort = require("serialport");

//  Muestra todos los puertos disponible
SerialPort.list(function (err, ports) {
ports.forEach(function(port) {
	console.log(port.comName);
	console.log(port.pnpId);
	console.log(port.manufacturer);
  });
});

*/