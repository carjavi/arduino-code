
String inputString = "";


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Wait Serial Data!");
  display.display(); 

  Serial.println("Wait Serial Data!");
  
}

void loop() {
  serialEvent(); 
}

void serialEvent() {
  while(Serial.available()) {
    char inChar = (char)Serial.read(); // get the new byte
    Serial.println(inChar);
    if (inChar == '@') { // ojo es '' y no ""
      Serial.println(inputString);
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Data Received...!");
      display.println(inputString);
      display.display();
      inputString = "";
      inChar = "";
      break;
    }
    inputString += inChar; // add it to the inputString
  }
}
