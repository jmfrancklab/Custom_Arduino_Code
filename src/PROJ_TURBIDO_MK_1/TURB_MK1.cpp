// Including the necessary libraries

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>


//Now defining important pins and instances hardwired

//TEMP PINS

#define ONE_WIRE_BUS A3 //The busline for the three temperature sensors
#define activator1 7 //The Relay pin for Bang Control
#define activator2 6 //The second relay control switch for Bang Control

//OPTICS AND SD

#define sensing_pin_op_amp A0 //The reading for cell count corrolation
#define slave_select_digi 9 // The slave communicator for the Digipot
#define Chip_Select_Pin 4 // The ethernet and SD chip select pin
#define HIGH_PIN 10 //In order for the arduino sheild to work must be present
//PUMP CONTROL

#define ENA_MotorPin 5 //Pin for PWM SPEED of PUMP MODULATION
#define IN1 A4 //Logical Determiner for on and off
#define IN2 A1 //Logical Determiner for on and off

//Button

#define IRupt 2 // The pin which opens up the menu for user interfacing


//DEFINING STRUCURES INSTENCES AND ADDRESSES

//Temp Control

OneWire oneWire(ONE_WIRE_BUS); // Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire); // Pass oneWire reference to DallasTemperature library
const uint8_t sensorA[8] = {0x28, 0x26, 0x3B, 0x94, 0x97, 0x0A, 0x03, 0x56};
const uint8_t sensorB[8] = {0x28, 0x0B, 0xB3, 0x79, 0x97, 0x00, 0x03, 0x6C};
const uint8_t sensorC[8] = {0x28, 0x10, 0x0A, 0x94, 0x97, 0x0A, 0x03, 0x40};
//0x28, 0x26, 0x3B, 0x94, 0x97, 0x0A, 0x03, 0x56 is A
//0x28, 0x0B, 0xB3, 0x79, 0x97, 0x00, 0x03, 0x6C is B
//0x28, 0x10, 0x0A, 0x94, 0x97, 0x0A, 0x03, 0x40 is C

//SPI and SD Settings and Control

SPISettings mySetting(16000000, MSBFIRST, SPI_MODE0); // Defining the SPI SETTINGS

struct datalog { 
  
  //For temp data
  
  float T_Water;
  float T_average_of_Al_Block; 
  unsigned long int millistime;
  unsigned long int heater_state;

    //For OD and Digipot Data
  float Voltage_analog_input;   // The analog input single from the OD sensor
  unsigned long int digi_pot_wiper_position;  // The digipot wiping position

  //For pump 

  unsigned long int pump_speed_setting; //Keeps a record of the pump speed
  float temp_baseline; //To allow comparision of the baseline temp target and the actual temperature

};

File MYDATA; //Created an instance of the open file







//NOW INTRODUCING VARIABLES WHICH WILL BE WITHIN FUNCITON ARGUEMENTS

//SD variables and Structure variables

const int datalen = 20; //The size of the data structure which is pushed into the SD card
datalog buffer[datalen]; // Size off each struct or... amount of time it takes to dump data to SD NOTE MUST BE CONST INT
const char *filename = "DATAREC.dat"; // The name of the file IMPORTANT CANNOT HAVE  more than 8 character within the name must follow the file naming system FAT 32
int place = 0; // The place is set to zero or one of the 20 slots within the structre since again, 0 is the first structure storage space
int serial_speed = 9600; //Setting the serial speed for configuration
int data_wrote; //The value of the file instance used to gather informaton about the write

//Pump Mechanics

int push_pump; // Value entered into the serial to control the pump 
bool Pump_State = true; // The value to determine the on or off state of the pump
float push_temp; // Value to set the temperature setting of the pump


//Interrupt mechanics

bool inter_on = false; // used to revert back to main program feed back after interuppt commmands finished 
int decider; //Helps with deciding which side program to activate
bool data_is_running = false; // A conditional switch to true after a command 
bool data_end = false; // A conditional which switches to true after a serial command to end the program is made

//Temp mechanics

float push_temp; // Value which is passed to temp target to find help with temp control stabalization
float ttar = 22; // Target temp 22 as defult 
float tolorance = 0.25; 
float t_max = ttar + tolorance; // The tolorances for each constraint
float t_min = ttar - tolorance; // The tolorances for each constraint
bool heater_on = false; // Heater starts of not on
bool hit_max = false;   // Has not entered max of min yet
bool hit_min = false; // Used to determine the on off cycle of the heater
unsigned long int passer; // Variable to pass the value of wether the heater is on or off


//OD and DIGIPOT MECHANICS

int place = 0; // The place is set to zero or one of the 20 slots within the structre since again, 0 is the first structure storage space
int digi_position; //The position of the digipot
int digi_value; //The value passed to the digiwrite function




//Adding indepentent action functions

void datadump()// writes the full structure to the file


{
  
  MYDATA= SD.open(filename,FILE_WRITE);
  data_wrote = MYDATA.write((const uint8_t *)buffer, sizeof(buffer)); // Writing to the file
  MYDATA.seek(EOF);
  Serial.print("Wrote: ");
  Serial.print(data_wrote); // To check if data actually written
  Serial.print(" much data\n");
  Serial.print("Is data avalible? ");
  Serial.println(MYDATA.available());
  Serial.print("The name of the file is: ");
  Serial.println(MYDATA.name());
  Serial.print("Size of Structure: ");
  Serial.println(sizeof(buffer));
  Serial.print("Position of the file is: ");
  Serial.println(MYDATA.position());
  Serial.print("The file Exist? ");
  Serial.println(SD.exists(filename));
  Serial.print("The size of the file is now: ");
  Serial.println(MYDATA.size());
  MYDATA.close();
  
}

int digiwrite(digi_value){ //Communicates a new resistence level to the digipot

  SPI.beginTransaction(mySetting);      // The transaction settings from the specific Arduino
  digitalWrite(slave_select_digi, LOW); // Once low the Settings shift to allow writing
  SPI.transfer(0x00);                   // The 8 bit address that is all zeros to prep to write the bit 0-255 for MCP4151
  SPI.transfer(digi_value);
  digitalWrite(slave_select_digi, HIGH); // Switching recieving bit back to zero
  SPI.endTransaction();                  // Ends the transaction for this specific spi device
}

void checkSD_initalize() //Set pinmode and initalize the SD 
{
  pinMode(Chip_Select_Pin, OUTPUT);
  pinMode(HIGH_PIN,OUTPUT);
  digitalWrite(HIGH_PIN, HIGH);
  
  Serial.print("Initializing SD card...");
  if (!SD.begin(Chip_Select_Pin))
  {                                          
    Serial.println("initialization failed!"); // Shows a problem if the SD never acually is reconigized by arduino
    return;
  }
  Serial.println("initialization done."); // Confirms Arduino SD error is not the problem
  if (SD.exists(filename)){  // If already a file
        SD.remove(filename); // Remove the file to stop weird confusing
    }
   
  
}

int datastore_add (place) { // Adds the current tuple of information to the structure
  
  buffer[place].millistime = millis();
  buffer[place].Voltage_analog_input = analogRead(sensing_pin_op_amp); // Analog Reading of OD
  buffer[place].digi_pot_wiper_position = digi_position; // Where the digipot is
  buffer[place].T_Water = sensors.getTempC(sensorA); // Temp Values
  buffer[place].T_average_of_Al_Block= (sensors.getTempC(sensorB)+sensors.getTempC(sensorC))/2; // Temp value
  buffer[place].pump_speed_setting = //Keeps a record of the pump speed
  buffer[place].temp_baseline = ttar; //To allow comparision of the baseline temp target and the actual temperature
  buffer[place].heater_state = passer;
}

void initalize_t_and_relay (){ // Sets up the temp control system


 
 pinMode(activator1,OUTPUT);
  pinMode(activator2,OUTPUT);
  digitalWrite(activator1,LOW);
  digitalWrite(activator2,LOW);
  sensors.begin();	// Start up the library
  Serial.print("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
  



}

void optics_setup(){ //Must be first within the set up
  
  pinMode(slave_select_digi, OUTPUT);
  pinMode(sensing_pin_op_amp, INPUT);
  SPI.begin(); // Initializes the SPI bus by setting SCK, MOSI, and SS to outputs, pulling SCK and MOSI low, and SS high.
  Serial.begin(serial_speed);
}

void temp_stabilizer() //Temp stabilizing function for the program
{
    if (sensors.getTempC(sensorA) >= t_max && !hit_max)
    {
        passer = 0;
        digitalWrite(activator1, LOW);
        digitalWrite(activator2, LOW);
        hit_max = true;
        hit_min = false;
        Serial.println("Heater Off");
        


    }
    else if (sensors.getTempC(sensorA) <= t_min && !hit_min)
    {
        passer = 1;
        digitalWrite(activator1, HIGH);
        digitalWrite(activator2, HIGH);
        hit_min = true;
        hit_max = false;
        Serial.println("Heater On");
        
    }
  }

void system_status(){

  Serial.print(" Digipot Position is: ");
  Serial.print(digi_position);
  Serial.print("Optic Integer [0-1024]: ");
  Serial.print()
}