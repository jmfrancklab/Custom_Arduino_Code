#include <Arduino.h>
#include <SPI.h>
#define ENA_MotorPin A2
#define IRupt 2
#define IN1 3
#define IN2 5

//Defining the pump interupt flag

volatile bool PumpFlag = false;
volatile bool TempFlag = false;
volatile bool StatusFlag = false;
volatile bool Pump_switch = false;
bool Pump_State = false;

int decider; //Helps with deciding which side program to activate

//Defining the handling interupt flag 

void handleInterrupt(){
    Serial.print("Choose a Mode");
    Serial.println("Type 1 for Status Update");
    Serial.println("Type 2 for Pump Control");
    Serial.println("Type 3 for Temp Control");
    Serial.println("Type 4 for Turning on or off the Pump");
    
    int decider = Serial.parseInt();

    switch(decider) {
  case 1:
  StatusFlag = true;
  Serial.println("Wait for status update: ");
    break;
  case 2:
   PumpFlag = true;
   Serial.println("Wait for Pump Setting: ");
    break;
  case 3:
    TempFlag = true;
    Serial.println("Wait for TempSetting");
    break;
    case 4:
    Serial.println("Switching Pump State");


  default:
    break;
}



}













void setup(){




}


void loop (){ 





}