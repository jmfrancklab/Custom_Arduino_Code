
#include <Arduino.h>





void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);   
  Serial.begin(9600);
  Serial.println("  Lets test the bluetooth module");
  Serial.println("Press 1 to light the LED, and 2 to turn it off");
  Serial.println("Entery: ");  
   digitalWrite(13, HIGH);
}


void loop() {
  if (Serial.available()){
  char input = Serial.read();
  switch (input){
  case '1': //turn led on
  digitalWrite(13, HIGH);   // set the LED on
  delay(100);              // wait for a second
  Serial.println("it works on!!");  
  break;
  case '2':
  digitalWrite(13, LOW);    // set the LED off
  delay(100);              // wait for a second
  Serial.println("it works off!!");
  break;
}
  }
}