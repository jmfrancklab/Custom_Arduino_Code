/* Importing Libraries required to use SD and Arduino Funcitons*/
#include <SD.h>
#include <SPI.h>
#include <Arduino.h>


/*Introducing variables for analyzing the analog input*/

//OD Placehold for single analog read 
int OD;
//Analog Tag for OD
int ReadOD = A0;
//Amount of Readings before taking an average
int numReadings;
//Analog number recorded onto the SD
int ODRecord;
//Readings Counter Goal
int numReadTar =5 ;
//Readings Counter
int numRead;
//File indentifier
File file;
/*
For: https://forum.arduino.cc/t/sd-examples-and-chip-select-definition/126821/3

The following notes were made:

For the Ethernet shield, pin 10 is chip select for the Ethernet controller
 and pin 4 is chip select for the SD card. So you should make pin 10 an output
  and set it high to disable the Ethernet controller, not because pin 10 is SS.
*/





void setup(){
/*Setting pins*/

pinMode(A0,INPUT);

//SD Begin Input as 4 which is defult

// SS must be in output mode even it is not chip select
//pinMode(SS_PIN, OUTPUT); **
//digitalWrite(SS_PIN, HIGH); **



//Starting serial terminal 

Serial.begin(9600);
ODRecord = 0;
numRead = 0;
OD = 0;
SD.open("System_State_Log.txt",FILE_WRITE);


}



void loop(){
//After Recording Value onto SD the num Reader goes back to zero


OD = analogRead(ReadOD);
ODRecord += OD;
numRead += 1;

//Now using if statement to determine if the number of readings were met

if(numRead == numReadTar){
ODRecord = ODRecord/numRead;
Serial.print("Analog OD Value: ");
Serial.println(ODRecord);
//Recording OD Analog to Record
SD.begin(4);
SD.open("System_State_Log.txt",FILE_WRITE);
file.print(ODRecord);
file.close();

//Resetting the Counters and the Recordering Variable

numRead = 0;
ODRecord = 0;

}

delay(100);

}



