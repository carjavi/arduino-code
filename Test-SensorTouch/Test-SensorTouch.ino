/*
* Autor: carjavi@hotmail.com
* Web: www.instintodigital.net
* Copyright (c) 2023
* Titule: Testing sensor Touch 
* Library:
* Commands:
* Observation: 
*/



#define led 10
#define sensorT 7
int val = 0;

void setup() {
  pinMode(led,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(sensorT, INPUT);
  Serial.begin(115200);
  Serial.println("Ready...");
}

void loop() {
  Serial.println(digitalRead(sensorT));
  digitalWrite(led, digitalRead(sensorT));
  val = digitalRead(sensorT);
  digitalWrite(LED_BUILTIN, val);
  delay(1);
}
