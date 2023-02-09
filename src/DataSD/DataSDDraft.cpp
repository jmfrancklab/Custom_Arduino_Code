//Working copy inspired by web
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>


//defining a structure
struct Datastore {
    unsigned int Analog;
    unsigned long int time
};

// I am going to test the ability to print structure to terminal first

//Now setting up the analog variables

int phold; // The counter for the elements in struct
int strsize = 5; // The size of the structure 


int rcount; // The reading counter
unsigned long int acount; // How the readings are added and eventuall divided by rcount
int amcount = 5; //The amount of reading taken before average




void setup(){
//Setting serial and pin 
pinMode(A0,INPUT);
Serial.begin(9600);

phold = 0;
rcount = 0;
acount = 0;

//Set the counting values to zero





}



void loop(){

acount =+ analogRead(A0); //Reading the voltage 
rcount += 1; //adding to the Voltage counter to probe the average

if(rcount == amcount){




}










}
