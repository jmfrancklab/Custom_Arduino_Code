#include <Arduino.h>
#include <SPI.h>


// Setting integers to for data keepers

 const int num_average = 10; //The number of counts before the average is taken
 const int sensing_pin_op_amp = A0; //Where the analog integer will be housed
 const int slave_select_digi = 10; // Slave selected pin for the arduino (can change depending on board)
const int serial_speed = 9600;
//Defining the SPI SETTINGS
SPISettings mySetting(16000000, MSBFIRST,SPI_MODE0 ); 
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
double take_average_opamp(){
  int store = 0; // Store the amount of analog reading it has taken
  
  for(int i;i<num_average; i++){
    store += analogRead(sensing_pin_op_amp); // iterates adds the value from the sensor from the op amp
  }
  return store/num_average; //Return the divided out num averge to get average
}

void setup(){
SPI.begin();// Initializes the SPI bus by setting SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high.
Serial.begin(serial_speed);
pinMode(sensing_pin_op_amp,INPUT);


}

void loop(){
int j = 0; // Restarting the variable resistor program
  for( int j;j<=254;j++){
    Serial.print("At Digi Pot STEP");
    Serial.print(j);
    Serial.print(": With a the sensor reading");
    Serial.print(int(take_average_opamp));  //Taking the average of the opamp function rounded to integeger which is not what much accuracy taken away
    delay(500); //Delay to make sure voltimeter can confirm voltage changes happending
    
  }



}