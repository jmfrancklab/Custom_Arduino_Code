#include <Arduino.h> 
#include <SPI.h>
#define CHIP_SELECT_10

int sselect = 10; // the slave pin which is either high (not ready to recieve information) or low ( ready to recieve)
int  i = 0;

void setup(){
  SPI.begin(); // Initializing the SPI library
  pinMode(sselect, OUTPUT);
  digitalWrite(sselect, LOW); // Set SS pin low initially
}


void loop(){
i = 0;
for( i; i<200;i++){
    digitalWrite(sselect,LOW);
    delay(50);
    SPI.transfer(0);
    SPI.transfer(i);
    delay(50);
    digitalWrite(sselect, HIGH);



 }

    
  
  }
