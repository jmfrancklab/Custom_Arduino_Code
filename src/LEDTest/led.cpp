//Not written by EP 



#include <Arduino.h>
#include <SPI.h>

#define LED_PIN 13 

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(LED_PIN, OUTPUT);   
  Serial.begin(9600);
  Serial.println("  Lets test the bluetooth module");
  Serial.println("Press 1 to light the LED, and 2 to turn it off");
  Serial.println("Entry: ");  
  digitalWrite(LED_PIN, HIGH);
}


void loop() {
  // this takes serial input from the terminal, and uses it to set the LED state
  if (Serial.available()){
  char input = Serial.read();
  // format this!!!!!
  // (this time I am reading, b/c it's short, but you absolutely need to format!
  // (we have discussed this on slack -- there is a shortcut key and you should ALWAYS format before committing)
  switch (input){
  case '1': //turn led on
  digitalWrite(LED_PIN, HIGH);   // set the LED on
  delay(100);              // wait for a second
  Serial.println("it works on!!");  
  break;
  case '2':
  digitalWrite(LED_PIN, LOW);    // set the LED off
  delay(100);              // wait for a second
  Serial.println("it works off!!");
  break;
}
  }
}
