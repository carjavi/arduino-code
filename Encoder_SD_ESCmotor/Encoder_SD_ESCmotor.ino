/*
 * ESC Motor --> D10
 * MolorLaser: Brushless min valor = 1550 y max valor = 1750
 * //
 * SD
 * cs --> D4
 * sck --> D13
 * mosi --> D11
 * miso --> D12
 * //
 * Encoder
 * left pin --> 2 y pin --> 5
 * right pin --> 3 y pin --> 6
 * //
 * Start pin --> 7
 * Recoding ligth --> 9
 * 
 */



#include <digitalWriteFast.h>
#include <Servo.h>

// Quadrature encoders
// encoder 1: 
#define c_LeftEncoderInterrupt 0
#define c_LeftEncoderPinA 2
#define c_LeftEncoderPinB 5
#define LeftEncoderIsReversed
volatile bool _LeftEncoderBSet;
volatile long _LeftEncoderTicks = 0;
// encoder 2
#define c_RightEncoderInterrupt 1
#define c_RightEncoderPinA 3
#define c_RightEncoderPinB 6
volatile bool _RightEncoderBSet;
volatile long _RightEncoderTicks = 0;

// SD
#include <SD.h>
File logFile;

//
Servo ESC;
int ESCpin = 10;

//indicador REC
int Rec_ligth = 9;

boolean Recoding = false;

void setup() {
   Serial.begin(9600);

    pinMode(Rec_ligth, OUTPUT);
     
     // Quadrature encoders
    pinMode(c_LeftEncoderPinA, INPUT);      // sets pin A as input
    digitalWrite(c_LeftEncoderPinA, HIGH);  // turn on pullup resistors
    pinMode(c_LeftEncoderPinB, INPUT);      // sets pin B as input
    digitalWrite(c_LeftEncoderPinB, HIGH);  // turn on pullup resistors
    attachInterrupt(c_LeftEncoderInterrupt, HandleLeftMotorInterruptA, RISING);
      // Right encoder
    pinMode(c_RightEncoderPinA, INPUT);      // sets pin A as input
    digitalWrite(c_RightEncoderPinA, HIGH);  // turn on pullup resistors
    pinMode(c_RightEncoderPinB, INPUT);      // sets pin B as input
    digitalWrite(c_RightEncoderPinB, HIGH);  // turn on pullup resistors
    attachInterrupt(c_RightEncoderInterrupt, HandleRightMotorInterruptA, RISING);
    // SD
    Serial.println(F("Iniciando SD & motor... wait"));
    if (!SD.begin(4))
    {
      Serial.println(F("Error al iniciar"));
      return;
    }

    //Motor brushless del laser
    ESC.attach(ESCpin);
    //Activar el ESC
    ESC.writeMicroseconds(2000); //1000 = 1ms
    //Cambia el 1000 anterior por 2000 si
    //tu ESC se activa con un pulso de 2ms
    delay(4000); //Esperar 5 segundos para hacer la activacion
    // Separe la variable Servo de su pin
    ESC.detach();
    delay(1000); 
    Serial.println(F("Iniciado correctamente"));
  
    //INICIA Motor
    ESC.attach(ESCpin);
    for (int i = 1000; i <1700; i= i+100) {
     ESC.writeMicroseconds(i); 
     delay(250);
     //Serial.println(i);
    }
    ESC.writeMicroseconds(1650);
    Serial.println(F("Motor ON"));
    
    digitalWrite(Rec_ligth, HIGH);
    
}

void loop() {
  
  Serial.println(_RightEncoderTicks);
  Serial.println(_LeftEncoderTicks);
  Serial.println(" ");

  // REC start
  if(_RightEncoderTicks >100){
    Recoding = true;
  }

  if(Recoding){
      digitalWrite(Rec_ligth, LOW);
      delay(200);
      digitalWrite(Rec_ligth, HIGH);
      delay(200);
  
      // Abrir archivo y escribir valor
      logFile = SD.open("odometry.csv", FILE_WRITE);
      if (logFile) { 
            logFile.print("Time(ms)=,");
            logFile.print(millis());
            logFile.print(", TicksAdelante=,");
            logFile.print(_LeftEncoderTicks);
            logFile.print(", TicksAtras=,");
            logFile.println(_RightEncoderTicks);
            logFile.close();
      } 
      else {
        Serial.println("Error al abrir el archivo");
      }
  }

  
  delay(100);

}

 // Interrupt service routines for the left motor's quadrature encoder
void HandleLeftMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _LeftEncoderBSet = digitalReadFast(c_LeftEncoderPinB);   // read the input pin
 
  // and adjust counter + if A leads B
  #ifdef LeftEncoderIsReversed
    _LeftEncoderTicks -= _LeftEncoderBSet ? -1 : +1;
  #else
    _LeftEncoderTicks += _LeftEncoderBSet ? -1 : +1;
  #endif
}
// Interrupt service routines for the right motor's quadrature encoder
void HandleRightMotorInterruptA()
{
  // Test transition; since the interrupt will only fire on 'rising' we don't need to read pin A
  _RightEncoderBSet = digitalReadFast(c_RightEncoderPinB);   // read the input pin
 
  // and adjust counter + if A leads B
  #ifdef RightEncoderIsReversed
    _RightEncoderTicks -= _RightEncoderBSet ? -1 : +1;
  #else
    _RightEncoderTicks += _RightEncoderBSet ? -1 : +1;
  #endif
}
