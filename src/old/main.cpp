#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <time.h>




int Photo = A0;
int numReadings = 10;
int Pread;
int readings;
double VRead;
double VReadSum;
char buffer[200];
double blank;
double OD;
File file;






void setup(){
SD.begin(4);
pinMode(Photo,INPUT);
Pread = analogRead(Photo);
Serial.begin(9600);
readings = 0;
VReadSum = 0;




//The SD stuff initalizing the code

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
    /* Writing to the SD file*/
    
    
   
    
    
    
    
    dtostrf(VReadSum/numReadings, 3, 4, buffer);
    Serial.print("Voltage: ");
    Serial.println(buffer);
    OD = log((VReadSum/numReadings)/5)/-2.861;
    
   
   
    /*Writing to the sd.file */
    SD.open("Test_Run.txt", FILE_WRITE);
    file.print("OD");
    file.print(" ");
    file.println("VReadSum/numReadings");
    file.close(); 
    /* FIle is closed*/
    
    dtostrf(OD, 3, 4, buffer);
    Serial.print("OD:" );
    Serial.println(buffer);
   
    readings = 0;
    VReadSum = 0;
 
    
   
    



}
delay(50);
}
