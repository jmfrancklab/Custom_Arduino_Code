#include <Arduino.h>
#include <SPI.h>
#include <SD.h>



int Photo = A0;
int numReadings = 100;
int Pread;
int readings;
double VRead;
double VReadSum;
char buffer[200];
double blank;
double OD;
const int sdcard = 10;



void setup(){
SD.begin();
pinMode(Photo,INPUT);
Pread = analogRead(Photo);
Serial.begin(9600);
readings = 0;
VReadSum = 0;
blank = analogRead(Photo)*5/1023;
//The SD stuff initalizing the code
const char myfile = SD.open("/eli-research/",FILE_WRITE);
}

//A structure which switches has three elements that will be processed back into binary

struct Datastore {
  uint16_t OD;
  uint16_t Voltage;
  uint16_t time;
};
// Are they pointers which point to the OD at a certain time?
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
// Working on how to write the datastoring as an array using a structure I guess

struct Datastore {
  uint16_t OD;
  uint16_t Voltage;
  uint16_t time;
}; 
//trying to assign the different datas to the different elements in the structure



}
delay(50);
}
