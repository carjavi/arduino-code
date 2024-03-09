/**
** este codigo lee ADC de un ADS1015 por el puerto i2C y lee datos proveniente de una RPI
*/

String receivedData = "";


#include <Wire.h>

#define SLAVE_ADDRESS 0x55 // comunication RPi
#define I2C_SDA 33
#define I2C_SCL 32

//ADC
#include <Adafruit_ADS1X15.h>
// Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
Adafruit_ADS1015 ads;     /* Use this for the 12-bit version */
#define ADS1015_ADDRESS 0x48 // Default address of ADS1015


void setup(void) {
  Serial.begin(115200); // Open Serial connection for debugging
  while (!Serial) {
    Serial.println("Failed to initialize Serial");
    while (1); // wait for serial port to connect. Needed for native USB
  }
  Serial.println("Serial Ready...");

  Wire.begin((uint8_t)SLAVE_ADDRESS);
  Wire.onReceive(receiveData); // Set up receive event handler
  
  // Initialize second I2C bus for ADS1015
  Wire1.begin(33, 32); // Initialize I2C communication with GPIO 31 as SDA and GPIO 32 as SCL
  ads.begin(ADS1015_ADDRESS, &Wire1); // Initialize ADS1015 with custom I2C bus
  if (!ads.begin(ADS1015_ADDRESS, &Wire1))
    {
      Serial.println("Failed to initialize ADS1015");
      while (1);
    }
  

  Serial.println("i2C Ready...");

}


void loop(void) {
  
  int16_t adc0;

  adc0 = ads.readADC_SingleEnded(0); // Read ADC value from channel 0

  Serial.print("AIN0: ");
  Serial.println(adc0);

  delay(500); // Add a delay to avoid flooding the serial monitor
  
}



void receiveData(int byteCount) {
  receivedData = "";
  while (Wire.available()) {
    receivedData += (char)Wire.read();
  }
  Serial.println(receivedData);
  //ValidateSerialData(receivedData);
}
