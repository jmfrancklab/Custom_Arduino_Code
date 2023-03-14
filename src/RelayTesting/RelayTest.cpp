#include <Arduino.h>
#include <SPI.h>

int Activator1 = 7;
int Activator2 = 8;

int heating_time = 10000;
int Butten = A0;


void setup(){

pinMode(Activator1, OUTPUT);
pinMode(Activator2,OUTPUT);
pinMode(Butten, INPUT);


}



void loop(){

Butten = analogRead(Butten);
if(analogRead(Butten)>1000){
    digitalWrite(Activator1,HIGH);
    delay(100);
    digitalWrite(Activator2,HIGH);
    delay(20000);
    digitalWrite(Activator1,LOW);
    delay(250);
    digitalWrite(Activator2,LOW);
    
    }
}