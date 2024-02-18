#include <SoftwareSerial.h>

// software serial : TX = digital pin 10, RX = digital pin 11
SoftwareSerial portOne(10, 11);

// software serial : TX = digital pin 8, RX = digital pin 9
SoftwareSerial portTwo(8, 9);

void setup()
{
  // Start the hardware serial port
  Serial.begin(9600);

  // Start both software serial ports
  portOne.begin(9600);
  portTwo.begin(9600);

}

void loop()
{
  portOne.listen();

  if (portOne.isListening()) {
   Serial.println("Port One is listening!"); 
}else{
   Serial.println("Port One is not listening!"); 
}

  if (portTwo.isListening()) {
   Serial.println("Port Two is listening!"); 
}else{
   Serial.println("Port Two is not listening!"); 
}

}
