#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */

void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Getting single-ended readings from AIN0");
  ads.begin();
}

void loop(void) 
{
  int16_t adc0, adc1, adc2, adc3;

  adc0 = ads.readADC_SingleEnded(0);

  Serial.print("AIN0: "); 
  Serial.println(adc0);

  
  delay(500);
}
