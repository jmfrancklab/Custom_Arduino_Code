#include <Arduino.h> 
#include <SPI.h>
#define CHIP_SELECT_10

int sselect = 10; // the slave pin which is either high (not ready to recieve information) or low ( ready to recieve)


void setup(){
  SPI.begin(); // Initializing the SPI library
  pinMode(sselect, OUTPUT);
  digitalWrite(sselect, LOW); // Set SS pin low initially
}

void loop(){
  digitalWrite(sselect,LOW);
  SPI.transfer(0);
  SPI.transfer(10);
  delay(500);
  digitalWrite(sselect,HIGH);
  digitalWrite(sselect,LOW);
    digitalWrite(sselect, LOW);
    SPI.transfer(0);
    SPI.transfer(200); 
    delay(500);
    digitalWrite(sselect,HIGH);
   

    
  
  }
