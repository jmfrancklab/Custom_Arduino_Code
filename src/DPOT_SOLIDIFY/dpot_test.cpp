#include <Arduino.h>
#include <SPI.h>
/*
[platformio]
src_dir = src/DPOT_SOLIDIFY

[env:uno]
platform = atmelavr
board = uno
framework = arduino
lib_deps = 
	arduino-libraries/SD@^1.2.4
	paulstoffregen/OneWire@^2.3.7
	milesburton/DallasTemperature@^3.11.0
*/



// Setting integers to for data keepers

 int num_average = 10; //The number of counts before the average is taken
 int sensing_pin_op_amp = A0; //Where the analog integer will be housed
 int slave_select_digi = 10; // Slave selected pin for the arduino (can change depending on board)
int serial_speed = 9600;

SPISettings mySetting(16000000, MSBFIRST,SPI_MODE0 ); //Defining the SPI SETTINGS
// the defult settings for the arduino (Mega may be different)
// Initialized settings for the ATMEGA328P core

void digiwrite (int digi_value){

SPI.beginTransaction(mySetting);//The transaction settings from the specific Arduino 
digitalWrite(slave_select_digi,LOW); // Once low the Settings shift to allow writing
SPI.transfer(0x00); //The 8 bit address that is all zeros to prep to write the bit 0-255 for MCP4151
SPI.transfer(digi_value);
digitalWrite(slave_select_digi,HIGH); // Switching recieving bit back to zero
SPI.endTransaction();  // Ends the transaction for this specific spi device
}


void setup(){

SPI.begin();// Initializes the SPI bus by setting SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high.
pinMode(slave_select_digi,OUTPUT);
pinMode(sensing_pin_op_amp,INPUT);
Serial.begin(9600);
digiwrite(0);
delay(5000);

}

void loop(){
int j = 100; // Restarting the variable resistor program
  for( j;j>=0;j--){
    Serial.print("At Digi Pot STEP");
    Serial.print(j);
    digiwrite(j); //Write a new digiwrite resister value
    Serial.print(": With a the sensor reading: ");
    Serial.print(analogRead(A0)); //Taking the average of the opamp function rounded to integeger which is not what much accuracy taken away
    Serial.print("\n") ; 
    delay(500); //Delay to make sure voltimeter can confirm voltage changes happending
    
  }
  
}