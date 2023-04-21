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

struct temprecord { 
  
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
temprecord buffer[datalen]; // Size off each struct or... amount of time it takes to dump data to SD NOTE MUST BE CONST INT
const char *filename = "DATAREC.dat"; // The name of the file IMPORTANT CANNOT HAVE  more than 8 character within the name must follow the file naming system FAT 32


//Pump Mechanics

int push_pump; // Value entered into the serial to control the pump 
bool Pump_State = true; // The value to determine the on or off state of the pump
float push_temp; // Value to set the temperature setting of the pump


//Interrupt mechanics

bool inter_on = false; // used to revert back to main program feed back after interuppt commmands finished 
int decider; //Helps with deciding which side program to activate
bool data_is_running = false; // A conditional switch to true after a command 
bool data_end = false; // A conditional which switches to true after a serial command to end the program is made




