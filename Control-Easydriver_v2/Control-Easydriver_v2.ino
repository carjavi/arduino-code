/* 
Commands: sample 1CC500, 2CW500
stop: 0 (solo si esta scaneando)
Scan: 10
homing scan: 20
end CC: 30
end CW: 40
motor1 360: 50
Steps CC: 60
Steps CW: 70
Turn CC: 80
Turn CW: 90

Serial:115200
*/

//Declare pin functions on EasyDriver
#define dir1 4 // direction
#define stp1 5 // steps
#define EN1  6 // Enable HIGH (disabled) / LOW(enabled)
#define dir2 7 // direction
#define stp2 8 // steps
#define EN2  9 // Enable HIGH (disabled) / LOW(enabled)



//Declare variables for functions
int x;
int y = 0;
int k = 0;
int sec=1;
String inputString = "";      // a String to hold incoming data
int PotValue = 0;
boolean endScan = false; // determina cuando ya hizo el barrido del scan
boolean endTurn = false; // determina cuando ya hizo el giro de 360 del soporte del scan
int endCC = 980 ;
int endCW = 570 ;
int cen_sup = 790;
int cen_inf = 789;
//CENTER : 789-790
//CC end: 1020-1021
//CW end: 570-571
//***************************************



//Prototipos de las funciones
void Serial_Data_Validation(String data);


void setup() {
  pinMode(stp1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(stp2, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(EN2, OUTPUT);
  
  Serial.begin(115200); //Open Serial connection for debugging
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println("ready...");
  
  //resetEDPins(); 
  DisableMotores();
}
void(* resetFunc) (void) = 0;//declare reset function 



void loop() {
  
PotValue = analogRead(A0);
//Serial.println(PotValue);

  
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      Serial_Data_Validation(inputString);
      inputString = "";
      inChar = "";
    }
  }
}


void Serial_Data_Validation(String data)
{
   //Read POT
   PotValue = analogRead(A0);
  //Serial.println(PotValue);
  // Serial.println(data);
  // verifica si el comando es un home
  int str = data.toInt();
    
    if(str == 10){
       scan();
    }
    
    if(str == 20){
       homeMotor2();
    }

    //end CC
    if(str == 30){
       endMCC();
    }
      
     //end CW
     if(str == 40){
        endMCW();
     }

     if(str == 50){
        Forward360();
     }
     
     if(str == 60){
        StepCC();
     }

     if(str == 70){
        StepCW();
     }

     if(str == 80){
        TurnCC();
     }

     if(str == 90){
        TurnCW();
     }
     

      
        // Forward & Reverse
        // descompone la data serial 
        int motor = (data.substring(0,1)).toInt();
        String turn = data.substring(1,3); 
        int steps = (data.substring(3,data.length())).toInt();
              
        if(motor==1 or motor==2){ // si no es un homing mueve el motor Forward o Reverse
            //Serial.println(data.substring(1,3));
              if(turn=="CW" or turn=="CC"){
                 if(steps > 0){
                 //Serial.println("Validated");
                 ForwardReverse(motor, turn,steps);
              }
            }
         }    
  
  
}


void ForwardReverse(int motor, String turn, int steps)
{
  Serial.print(motor);
  Serial.print(turn);
  Serial.println(steps);

  if(motor==1){
      digitalWrite(EN1, LOW);
      if(turn == "CW"){digitalWrite(dir1, HIGH);}
      if(turn == "CC"){digitalWrite(dir1, LOW);}
      for(x= 0; x<steps; x++)  //Loop the forward stepping enough times for motion to be visible
      {
        digitalWrite(stp1,HIGH); //Trigger one step forward
        delay(sec);
        digitalWrite(stp1,LOW); //Pull step pin low so it can be triggered again
        delay(sec);
      }
      DisableMotores();
  }

  
  if(motor==2){
      digitalWrite(EN2, LOW);
      if(turn == "CW"){digitalWrite(dir2, HIGH);}
      if(turn == "CC"){digitalWrite(dir2, LOW);}
      for(x= 0; x<steps; x++)  //Loop the forward stepping enough times for motion to be visible
      {
        digitalWrite(stp2,HIGH); //Trigger one step forward
        delay(sec);
        digitalWrite(stp2,LOW); //Pull step pin low so it can be triggered again
        delay(sec);
      }
      DisableMotores();
  }
  
}


void DisableMotores()
{
  digitalWrite(stp1, LOW);
  digitalWrite(dir1, LOW);
  digitalWrite(EN1, HIGH);
  digitalWrite(stp2, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(EN2, HIGH);
}

void scan()
{
    endMCC();
    Serial.println("scan...");
    k = 0;
    digitalWrite(EN1, LOW); 
    digitalWrite(dir1, LOW); 
    while(PotValue >= endCW){
        PotValue = analogRead(A0);
        if(k == 10000){
          StepsScan();
          Serial.print("round ");
          Serial.println(PotValue);
          k = 0;
        }
        digitalWrite(stp1,HIGH); //Trigger one step forward
        delay(sec);
        digitalWrite(stp1,LOW); //Pull step pin low so it can be triggered again
        delay(sec);
        k = k + 1;
        serial_interruption();
    }
    DisableMotores(); // desbloquea motor
    homeMotor2();
    Serial.println(" Scan listo!");
}

void endMCC()
{
            //Serial.println("start end CC");
            digitalWrite(EN2, LOW);
              digitalWrite(dir2, LOW);
              PotValue = analogRead(A0);
              while(PotValue<=endCC){
                  digitalWrite(stp2,HIGH); //Trigger one step forward
                  delay(sec);
                  digitalWrite(stp2,LOW); //Pull step pin low so it can be triggered again
                  delay(sec);
                  PotValue = analogRead(A0);
                  Serial.println(PotValue);
              }
              DisableMotores();
              Serial.println("end CC");
}

void endMCW()
{
            //Serial.println("start end CW");
            digitalWrite(EN2, LOW);
              digitalWrite(dir2, HIGH);
              PotValue = analogRead(A0);
              while(PotValue>=endCW){
                  digitalWrite(stp2,HIGH); //Trigger one step forward
                  delay(sec);
                  digitalWrite(stp2,LOW); //Pull step pin low so it can be triggered again
                  delay(sec);
                  PotValue = analogRead(A0);
                  Serial.println(PotValue);
              }
              DisableMotores();
              Serial.println("end CW");
}


void homeMotor2()
{
  //Serial.println("Homing motor 2");
          PotValue = analogRead(A0);
          if(PotValue>cen_sup){
            //move CW 
            //Serial.println(">790"); 
            digitalWrite(EN2, LOW);
            digitalWrite(dir2, HIGH);
            while(PotValue>cen_sup){
                  digitalWrite(stp2,HIGH); //Trigger one step forward
                  delay(sec);
                  digitalWrite(stp2,LOW); //Pull step pin low so it can be triggered again
                  delay(sec);
                  PotValue = analogRead(A0);
              }
              DisableMotores();
          }
          
          if(PotValue<cen_inf){
              //move CC
              //Serial.println("<790");
              digitalWrite(EN2, LOW);
              digitalWrite(dir2, LOW);
              while(PotValue<cen_inf){
                  digitalWrite(stp2,HIGH); //Trigger one step forward
                  delay(sec);
                  digitalWrite(stp2,LOW); //Pull step pin low so it can be triggered again
                  delay(sec);
                  PotValue = analogRead(A0);
              }
              DisableMotores();
          }
          Serial.println("Home Already...");
}

void StepsScan()
{
              //Serial.println("step CW");
              digitalWrite(EN2, LOW);
              digitalWrite(dir2, HIGH);
              for(x= 0; x<50; x++){
                  digitalWrite(stp2,HIGH); //Trigger one step forward
                  delay(sec);
                  digitalWrite(stp2,LOW); //Pull step pin low so it can be triggered again
                  delay(sec);
              }
              digitalWrite(EN2, HIGH);
}

void Forward360()
{
  digitalWrite(EN1, LOW); // activa movimiento /bloquea motor
  digitalWrite(dir1, LOW); //Pull direction pin low to move "forward"
  for(x= 0; x<10000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp1,HIGH); //Trigger one step forward
    delay(sec);
    digitalWrite(stp1,LOW); //Pull step pin low so it can be triggered again
    delay(sec);
  }
  DisableMotores(); // desbloquea motor
  Serial.println("listo!");
}


// si llega otro dato en el puerto se detiene lo que esta haciendo
void serial_interruption() {
  if (Serial.available() > 0) {
    digitalWrite(stp1, LOW);
    digitalWrite(dir1, LOW);
    digitalWrite(EN1, HIGH);
    digitalWrite(stp2, LOW);
    digitalWrite(dir2, LOW);
    digitalWrite(EN2, HIGH);
    resetFunc(); // call reset
  }
}

void StepCC()
{
              digitalWrite(EN2, LOW);
              digitalWrite(dir2, LOW);
              for(x= 0; x<100; x++){
                  digitalWrite(stp2,HIGH); //Trigger one step forward
                  delay(sec);
                  digitalWrite(stp2,LOW); //Pull step pin low so it can be triggered again
                  delay(sec);
              }
              DisableMotores();
}

void StepCW()
{
              digitalWrite(EN2, LOW);
              digitalWrite(dir2, HIGH);
              for(x= 0; x<100; x++){
                  digitalWrite(stp2,HIGH); //Trigger one step forward
                  delay(sec);
                  digitalWrite(stp2,LOW); //Pull step pin low so it can be triggered again
                  delay(sec);
              }
              DisableMotores();
}

void TurnCC()
{
              digitalWrite(EN1, LOW);
              digitalWrite(dir1, LOW);
              for(x= 0; x<100; x++){
                  digitalWrite(stp1,HIGH); //Trigger one step forward
                  delay(sec);
                  digitalWrite(stp1,LOW); //Pull step pin low so it can be triggered again
                  delay(sec);
              }
              DisableMotores();
}

void TurnCW()
{
              digitalWrite(EN1, LOW);
              digitalWrite(dir1, HIGH);
              for(x= 0; x<100; x++){
                  digitalWrite(stp1,HIGH); //Trigger one step forward
                  delay(sec);
                  digitalWrite(stp1,LOW); //Pull step pin low so it can be triggered again
                  delay(sec);
              }
              DisableMotores();
}






/*
 * 
 * 
void Forward360Steps()
{
  digitalWrite(EN1, LOW); // activa movimiento /bloquea motor
  digitalWrite(dir1, LOW); //Pull direction pin low to move "forward"
  for(y=0; y<102; y++)  //11 veces 
  {
        Serial.println(y);

        // motor 1 turn
        for(x=0; x<100; x++)  //Loop the forward stepping enough times for motion to be visible
        {
          digitalWrite(stp1,HIGH); //Trigger one step forward
          delay(sec);
          digitalWrite(stp1,LOW); //Pull step pin low so it can be triggered again
          delay(sec);
        }
        delay(2000);
        
  }
  DisableMotores(); // desbloquea motor
  Serial.println("listo 360Steps!");
}



//Default microstep mode function
void StepForwardDefault()
{
  Serial.println("Moving forward at default step mode.");
  digitalWrite(EN1, LOW); // activa movimiento /bloquea motor
  digitalWrite(dir1, LOW); //Pull direction pin low to move "forward"
  for(x= 0; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp1,HIGH); //Trigger one step forward
    delay(sec);
    digitalWrite(stp1,LOW); //Pull step pin low so it can be triggered again
    delay(sec);
  }
  digitalWrite(EN1, HIGH); // desbloquea motor
  Serial.println("listo!");
  Serial.println();
}


//Reverse default microstep mode function
void ReverseStepDefault(){
  Serial.println("Moving in reverse at default step mode.");
  digitalWrite(EN1, LOW); // activa movimiento /bloquea motor
  digitalWrite(dir1, HIGH); //Pull direction pin low to move "forward"
  for(x= 0; x<1000; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp1,HIGH); //Trigger one step forward
    delay(sec);
    digitalWrite(stp1,LOW); //Pull step pin low so it can be triggered again
    delay(sec);
  }
  digitalWrite(EN1, HIGH); // desbloquea motor
  Serial.println("listo!");
  Serial.println();
}

*/
