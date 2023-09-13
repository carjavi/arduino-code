/*
* Autor: carjavi@hotmail.com
* Web: www.instintodigital.net
* Copyright (c) 2023
* Titule: SerialEvent
* Library:
* Commands:
* Observation: Siempre el primer datos llega mal
*/



String inputString = "";
String str = "";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}

void serialEvent(){
  while (Serial.available()) {              
    char inChar = (char)Serial.read();   // get the new byte:
    inputString += inChar; // add it to the inputString:
    if (inChar == '\n') {
      ValidateSerialData(inputString);
      inputString = "";
    }
  }
}

void ValidateSerialData(String data){
  //Serial.println(data);
  str = data.substring(0, data.length()-1); //remove /n

  if (str == "on"){
      Serial.println("on led");                   
      digitalWrite(LED_BUILTIN, HIGH);
  }
  if (str.equals("off")){ 
      Serial.println("off led");                   
      digitalWrite(LED_BUILTIN, LOW);   
   }
}
