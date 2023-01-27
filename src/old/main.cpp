#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <time.h>



/*Introducing variables*/
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


//A structure which switches has two elements that will be processed back into binary

struct Datastore {
  uint16_t OD;
  uint16_t Voltage;
};







/*Setting up the pins and SD */
void setup(){
SD.begin(10);
pinMode(Photo,INPUT);
pinMode(10,OUTPUT);
Pread = analogRead(Photo);
Serial.begin(9600);
//readings = 0;
//VReadSum = 0;

}




void loop(){
Pread = analogRead(Photo);
VRead = Pread * 5.0 / 1023;
VReadSum += VRead;
readings++;
if(readings == numReadings){
   
/*Printing values to the serial monitor */
    dtostrf(VReadSum/numReadings, 3, 4, buffer);
    Serial.print("Voltage: ");
    Serial.println(buffer);
    OD = log((VReadSum/numReadings)/5)/-2.861;
    dtostrf(OD, 3, 4, buffer);
    Serial.print("OD:" );
    Serial.println(buffer);

    /* Putting data within the structure to prep to write to SD file*/
    Datastore dummy;
    dummy.OD = OD;
    dummy.Voltage = VReadSum/numReadings;


    
    
   
    /*Writing to the sd.file (does not work yet) */
    SD.open("Test_Run.dat", FILE_WRITE);
    file.write((uint8_t *)&dummy, sizeof(dummy));
    file.close(); 
    /* FIle is closed*/
    
    
   /*Resetting average counters*/
    readings = 0;
    VReadSum = 0;
 
    
   
    



}
delay(50);
}
