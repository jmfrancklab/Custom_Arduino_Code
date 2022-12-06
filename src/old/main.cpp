#include <Arduino.h>



int Photo = A0;
int numReadings = 100;
int Pread;
int readings;
double VRead;
double VReadSum;
char buffer[200];
double blank;
double OD;

void setup(){
pinMode(Photo,INPUT);
Pread = analogRead(Photo);
Serial.begin(9600);
readings = 0;
VReadSum = 0;
blank = analogRead(Photo)*5/1023;

}

void loop(){
Pread = analogRead(Photo);
VRead = Pread * 5.0 / 1023;
VReadSum += VRead;
readings++;
if(readings == numReadings){
    dtostrf(VReadSum/numReadings, 3, 4, buffer);
    Serial.print("Voltage: ");
    Serial.println(buffer);
    OD = log((VReadSum/numReadings)/5)/-2.861;
    dtostrf(OD, 3, 4, buffer);
    Serial.print("OD:" );
    Serial.println(buffer);
    readings = 0;
    VReadSum = 0;
}
delay(50);
}
