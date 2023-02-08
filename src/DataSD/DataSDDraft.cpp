/* Importing Libraries required to use SD and Arduino Funcitons*/
#include <SD.h>
#include <SPI.h>
#include <Arduino.h>
#define SD_CHIP_SELECT_PIN 4

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

//Making a structure with the needed variables
struct Datastore {
    // note that the types on the arduino are slightly different, and you should
    // make sure that you match the type of the data that you're trying to
    // store!!
    unsigned int AnalogOD; // this should just be a raw arduino reading DO NOT DO THE OD MATH ON THE ARDUINO!!!
    unsigned long int time; // similarly, just the result of millis
    
};

File myfile;

//Now making a specific data type from the prevously defined structure

Datastore logchunk[1];



//This makes a structure which can record file size
//This will be referenced over time







void setup(){
/*Setting pins*/
pinMode(A0,INPUT);
digitalWrite(10,HIGH);
//Making name of file
 //why not const char here
 const char *filename = "SD.Logger.dat";
//Setting up the file instance and begining SD
SD.begin(SD_CHIP_SELECT_PIN);
//Now making file opener
myfile = SD.open(filename,FILE_WRITE);







//Starting serial terminal and making average control

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
//Recording Data point to serial Reader

Serial.print(ODRecord);
Serial.print(" At time: ");
Serial.print(millis()/500);
Serial.println(" Seconds");


//Recording OD variable within the strucure datalog
logchunk[1].AnalogOD = ODRecord;
logchunk[1].time = millis()/1000;
//Once the data is written to the structure the SD commands will write to file
myfile.write((const uint8_t *)logchunk, sizeof(1));
//Will now close the file to make sure data is saved
myfile.close();
//Resetting the Counters and the Recordering Variable

numRead = 0;
ODRecord = 0;

}

delay(100);

}



