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








