
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
        //console.log("manufacturer: " + port.manufacturer);
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
      port.write('Hola mundo\n', (err) => {
        if (err) {
          return console.log('Error al enviar el mensaje: ', err.message);
        }
        console.log('hola mundo enviado');
      });
    });

    port.on("error", function(err) {
      console.log("Error: ", err.message);
    });

  } catch (error) {
    console.error("Error: ", error.message);
  }
}

start();



