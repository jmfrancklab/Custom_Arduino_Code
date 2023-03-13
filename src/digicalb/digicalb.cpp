#include<Arduino.h>
#include<SPI.h>

int vsen = A0;
int slavecom = 10; // The slave communication to control the voltage
int timecali = 50000;



void setup(){
SPI.begin(); // Starting the  SPI slate
pinMode(vsen, INPUT);
pinMode(slavecom,OUTPUT);
digitalWrite(slavecom,LOW);

}






void loop(){

digitalWrite(slavecom,LOW);
SPI.transfer(0);
SPI.transfer(20);
 digitalWrite(slavecom,HIGH);   

//The saturation of water is about SPI.transfer(19)
    
}