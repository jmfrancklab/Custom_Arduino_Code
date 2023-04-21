#include <Arduino.h>
#include <SPI.h>
#define ENA_MotorPin 5
#define IRupt 2
#define IN1 A4
#define IN2 A1

//Defining the pump interupt flag

volatile bool PumpFlag = false;
volatile bool TempFlag = false;
volatile bool StatusFlag = false;
volatile bool Pump_switch = false;
bool Pump_State = false;
int push_pump;
float push_temp;
bool inter_finished = false;
bool inter_on = false;


int decider; //Helps with deciding which side program to activate

//Defining the handling interupt flag 


void handleInterrupt(){
inter_on = true;

}
void user_choice_interface () {
  Serial.println("Choose a Mode");
  Serial.println("Type 1 for Status Update");
  Serial.println("Type 2 for Pump Control");
  Serial.println("Type 3 for Temp Control");
  Serial.println("Type 4 for Turning on or off the Pump");

  while (!Serial.available()) {
    delay(100); // Wait for input
  }
  int decider = Serial.parseInt();
  
  switch(decider) {
    case 1:
      StatusFlag = true;
      Serial.println("Wait for status update: ");
      break;
    case 2:
      PumpFlag = true;
      Serial.println("Wait for Pump Setting: ");
      Serial.println("Please type the pump control: (0-255)");
      
      while (Serial.available()<3 || Serial.peek() == ".") {
        delay(100); // Wait for input
      }
      push_pump = Serial.parseInt();
    
      
      break;
    case 3:
      TempFlag = true;
      Serial.println("Wait for TempSetting");
      Serial.println("Please type temp setting (20 C < T < 50 C ) Type [xx.xx] ");
      while (Serial.available()< 4|| Serial.peek() == "."){
        delay(100); // Wait for input
      }
      push_temp = Serial.parseFloat();
      break;
    case 4:
      Serial.println("Switching Pump State");
      Serial.println("Will Put in the xor state soon"); 
      Pump_State ^ true; // If true then false if false then true
      if( Pump_State ==true){
        analogWrite(ENA_MotorPin, 0);


      }



      break;
    default:
      break;  
  }
} 
void pump_turn 

void setup(){
Serial.begin(9600);
pinMode(IRupt, INPUT_PULLUP);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
attachInterrupt(digitalPinToInterrupt(IRupt), handleInterrupt, FALLING);

}





void loop (){ 
Serial.println("Doing other stuff");
delay(500);
if(inter_on){
    user_choice_interface();
    inter_on = false;
}



}