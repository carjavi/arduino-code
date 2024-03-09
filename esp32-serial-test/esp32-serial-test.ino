

void setup() {
  Serial.begin(115200); //Open Serial connection for debugging
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println("ready...");

}

void loop() {
    if(Serial.available()){
      String command = Serial.readStringUntil('\n'); 
      Serial.println(command);
    }

}


