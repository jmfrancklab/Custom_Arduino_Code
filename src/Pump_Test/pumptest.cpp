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
int push_pump;
bool Pump_State = true;
float push_temp;
bool inter_finished = false;
bool inter_on = false;


int decider; //Helps with deciding which side program to activate

//Defining the handling interupt flag 


void handleInterrupt(){
inter_on = true;

}
void user_choice_interface () {
  Serial.println("\n\n\n\n\nChoose a Mode");
  Serial.println("Type 1 for Pump Control");
  Serial.println("Type 2 for Temp Control");
  Serial.println("Type 3 for Turning on or off the Pump");
  Serial.println("Type 4 for Pump Direction Switch");

  while (!Serial.available()) {
    delay(100); // Wait for input
  }
  int decider = Serial.parseInt();
  
  switch(decider) {
    Serial.flush(); // To make sure if the user presses a faulty key the program does not fail
    
    case 1:
      
      Serial.println("Wait for Pump Setting: ");
      Serial.println("Please type the pump control: (0-255), Type xxx or xx. for x < 100");
      Serial.println("Make sure pump is on first");
      
      while (Serial.available()<3 || Serial.peek() == ".") {
        delay(100); // Wait for input
      }
      push_pump = Serial.parseInt();
      analogWrite(ENA_MotorPin,push_pump);
      Serial.print("Pump set to analog setting of: ");
      Serial.print(push_pump);
    
      
      break;
    case 2:
      
      Serial.println("Wait for TempSetting");
      Serial.println("Please type temp setting (20 C < T < 50 C ) Type xx.xx ");
      while (Serial.available()< 5|| Serial.peek() == "."){
        delay(100); // Wait for input
      }
      push_temp = Serial.parseFloat();
      Serial.print("Set temp baseline to: ");
      Serial.print(push_temp);
      Serial.print(" C");
      break;
    case 3:
      Serial.println("Switching Pump State");
      Pump_State ^= true; 
      //Switching the value from its orgional if off then on if on then off
      if( Pump_State == true){
        digitalWrite(IN1,LOW);
        digitalWrite(IN2,LOW);


      } else if (Pump_State ==false){
          digitalWrite(IN1,HIGH);
          digitalWrite(IN2, LOW);
          analogWrite(ENA_MotorPin,10);
          

      }
      break;
      case 4:
      if ( digitalRead(IN1) == 1 && digitalRead(IN2) == 0){

        digitalWrite(IN1,LOW);
        digitalWrite(IN2, HIGH);
        Serial.println("Switch Motor Flow Direction");

      } else if (digitalRead(IN1) == 0 && digitalRead(IN2) == 1){

        digitalWrite(IN1,HIGH);
        digitalWrite(IN2,LOW);
        Serial.print("Switched Motor Flow Direction");
      } else {

        Serial.print("Should Turn on Pump First");
        
      }
      
      break;

    default:
      Serial.print("Rerun the Interrupt Again Error Occured");
      
      break;  
  }
} 

void setup(){
Serial.begin(9600);
pinMode(IRupt, INPUT_PULLUP);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
attachInterrupt(digitalPinToInterrupt(IRupt), handleInterrupt, FALLING);

}





void loop (){ 
delay(500);
if(inter_on){
    user_choice_interface();
    inter_on = false;
}



}