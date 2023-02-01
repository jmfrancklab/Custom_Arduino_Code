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

File myfile;
//Will now make a structure which allows things to be written to a file

struct setlog {
uint16_t Analog;
/*Will set a time module but want to get this to work first*/
};






void setup(){
/*Setting pins*/

pinMode(A0,INPUT);

//SD Begin Input as 4 which is defult

// SS must be in output mode even it is not chip select
//pinMode(SS_PIN, OUTPUT); **
//digitalWrite(SS_PIN, HIGH); **

SD.open("System_State_Log.dat",FILE_WRITE);

//Starting serial terminal 

Serial.begin(9600);
ODRecord = 0;
numRead = 0;
OD = 0;



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

//Setting structure element to ODRecord
setlog logging;
logging.Analog = ODRecord; 


SD.begin(4);

myfile.write(logging.Analog);
myfile.close();

//Resetting the Counters and the Recordering Variable

numRead = 0;
ODRecord = 0;

}

delay(100);

}



