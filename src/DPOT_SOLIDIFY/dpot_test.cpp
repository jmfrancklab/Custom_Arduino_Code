#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define CHIP_SELECT_PIN_SD 4

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
int slave_select_digi = 9; // Slave selected pin for the arduino (can change depending on board)
int serial_speed = 9600;

//Now setting datastore structure and SD Mechanics

struct datastore //Now setting datastore structure 

{
// The datatype followed by the name of the place holder is given

unsigned long int millistime; //Time from the millis function count
unsigned long int microtime; // Time from the smaller micros function
float Voltage_analog_input; // The analog input single from the OD sensor
int digi_pot_wiper_position; //The digipot wiping position
};

File dataFile; // Created an instance of the open file 

const int datalen = 20; //Size off each struct or... amount of time it takes to dump data to SD NOTE MUST BE CONST INT
char *filename = "OD_Test_Solid.dat"; // The name of the file
int fileplaceholder = 1;


 //Creating the instance which opens the file
datastore dbuff[datalen]; // Created the datalen constant


SPISettings mySetting(16000000, MSBFIRST,SPI_MODE0 ); //Defining the SPI SETTINGS
// the defult settings for the arduino (Mega may be different)
// Initialized settings for the ATMEGA328P core
void checkSD (){
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);
  Serial.print("Initializing SD card...");
if (!SD.begin(CHIP_SELECT_PIN_SD))
    {                                             // even though this argument is technically optional, we do need to select chip 4 for this to work!!
        Serial.println("initialization failed!"); // Shows a problem if the SD never acually is reconigized by arduino
        return;
    }
    Serial.println("initialization done."); // Confirms Arduino SD error is not the problem

    // Write adds to end, so remove any preexisting file
    if (SD.exists(filename))
    {                        // If already a file
        SD.remove(filename); // Remove the file to stop weird confusing
    }
}
void print_structure (){

  for (int k = 0; k < datalen; k++)
                {
                    Serial.print("Datapoint: ");
                    Serial.print(k);
                    Serial.print(": ");
                    Serial.print(dbuff[k].Voltage_analog_input);
                    Serial.print(" ,");
                    Serial.print(dbuff[k].millistime);
                    Serial.print(" ,");
                    Serial.print(dbuff[k].microtime);
                    Serial.print(" ,");
                    Serial.print(dbuff[k].digi_pot_wiper_position);
                    Serial.print("\n");
                }
}
void datastore_add(int place,int j){ //Adds the current tuple of information to the structure
  dbuff[place].millistime = millis();
  dbuff[place].microtime = micros();
  dbuff[place].Voltage_analog_input = analogRead(sensing_pin_op_amp);
  dbuff[place].digi_pot_wiper_position = j; // Since the place i






}
void datadump(){
int data_wrote = dataFile.write((const uint8_t *)dbuff, sizeof(dbuff));//Writing to the file
Serial.print("Wrote: ");
Serial.print(data_wrote); // To check if data actually written 
Serial.print(" much data\n");
dataFile.close(); // Closes the file to preserve the data



}
void digiwrite (int digi_value){

SPI.beginTransaction(mySetting);//The transaction settings from the specific Arduino 
digitalWrite(slave_select_digi,LOW); // Once low the Settings shift to allow writing
SPI.transfer(0x00); //The 8 bit address that is all zeros to prep to write the bit 0-255 for MCP4151
SPI.transfer(digi_value);
digitalWrite(slave_select_digi,HIGH); // Switching recieving bit back to zero
SPI.endTransaction();  // Ends the transaction for this specific spi device
}
void serline(int j){ //function to print one line of code to the serial of information

  Serial.print("At Digi Pot STEP");
    Serial.print(j);
    digiwrite(j); //Write a new digiwrite resister value
    Serial.print(": With a the sensor reading: ");
    Serial.print(analogRead(A0)); //Taking the average of the opamp function rounded to integeger which is not what much accuracy taken away
    Serial.print("\n") ; 
    delay(500); //Delay to make sure voltimeter can confirm voltage changes happending
}
void setup(){
Serial.begin(9600);
SPI.begin(); // Initializes the SPI bus by setting SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high.
pinMode(slave_select_digi,OUTPUT);
pinMode(sensing_pin_op_amp,INPUT);
checkSD();


}
void loop(){

int j = 100; // Restarting the variable resistor program
int place = 0;
  
  for(j;j>=0;j--){
  serline(j);
  datastore_add(place,j);
  place++;
    if(place> datalen-1){
        Serial.print("\n\n");
        datadump();
        print_structure();
        place = 1;
    }
  }


}
  
