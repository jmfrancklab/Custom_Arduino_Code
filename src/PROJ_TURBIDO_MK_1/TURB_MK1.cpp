// Including the necessary libraries

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Now defining important pins and instances hardwired

// TEMP PINS

#define ONE_WIRE_BUS 3 // The busline for the three temperature sensors
#define activator1 7   // The Relay pin for Bang Control
#define activator2 6   // The second relay control switch for Bang Control

// OPTICS AND SD

#define sensing_pin_op_amp A0 // The reading for cell count corrolation
#define slave_select_digi 53  // The slave communicator for the Digipot
#define Chip_Select_Pin 4     // The ethernet and SD chip select pin
#define HIGH_PIN 10           // In order for the arduino sheild to work must be present
// PUMP CONTROL

#define ENA_MotorPin 5 // Pin for PWM SPEED of PUMP MODULATION
#define IN1 A4         // Logical Determiner for on and off
#define IN2 A1         // Logical Determiner for on and off

// Button

#define IRupt 2 // The pin which opens up the menu for user interfacing

// DEFINING STRUCURES INSTENCES AND ADDRESSES

// Temp Control

OneWire oneWire(ONE_WIRE_BUS);       // Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire); // Pass oneWire reference to DallasTemperature library
const uint8_t sensorA[8] = {0x28, 0x26, 0x3B, 0x94, 0x97, 0x0A, 0x03, 0x56};
const uint8_t sensorB[8] = {0x28, 0x0B, 0xB3, 0x79, 0x97, 0x00, 0x03, 0x6C};
const uint8_t sensorC[8] = {0x28, 0x10, 0x0A, 0x94, 0x97, 0x0A, 0x03, 0x40};
// 0x28, 0x26, 0x3B, 0x94, 0x97, 0x0A, 0x03, 0x56 is A
// 0x28, 0x0B, 0xB3, 0x79, 0x97, 0x00, 0x03, 0x6C is B
// 0x28, 0x10, 0x0A, 0x94, 0x97, 0x0A, 0x03, 0x40 is C

// SPI and SD Settings and Control

SPISettings mySetting(16000000, MSBFIRST, SPI_MODE0); // Defining the SPI SETTINGS

struct datalog
{
  unsigned long int millistime;
  float Voltage_analog_input;
  unsigned long int digi_pot_wiper_position;
  float T_Water;
  float T_average_of_Al_Block;
  unsigned long int pump_speed_setting;
  float temp_baseline;
  unsigned long int heater_state;
  unsigned long int microtime;
};

File MYDATA; // Created an instance of the open file

// NOW INTRODUCING VARIABLES WHICH WILL BE WITHIN FUNCITON ARGUEMENTS

// SD variables and Structure variables

const int datalen = 20;               // The size of the data structure which is pushed into the SD card
datalog buffer[datalen];              // Size off each struct or... amount of time it takes to dump data to SD NOTE MUST BE CONST INT
const char *filename = "DATAREC.dat"; // The name of the file IMPORTANT CANNOT HAVE  more than 8 character within the name must follow the file naming system FAT 32
int place = 0;                        // The place is set to zero or one of the 20 slots within the structre since again, 0 is the first structure storage space
int serial_speed = 9600;              // Setting the serial speed for configuration
int data_wrote;                       // The value of the file instance used to gather informaton about the write
int start_time;

// Pump Mechanics

int push_pump;          // Value entered into the serial to control the pump
bool Pump_State = true; // The value to determine the on or off state of the pump
float push_temp;        // Value to set the temperature setting of the pump

// Interrupt mechanics

bool inter_on = false;        // used to revert back to main program feed back after interuppt commmands finished
int decider;                  // Helps with deciding which side program to activate
bool data_is_running = false; // A conditional switch to true after a command
bool displaying_serial = false;
bool data_probe = false;
bool switcher = false;
// Temp mechanics
int device_count; // The amout of devices that are counted of temp prboes berfore program start
float ttar = 22;  // Target temp 22 as defult
float tolorance = 0.25;
float t_max = ttar + tolorance; // The tolorances for each constraint
float t_min = ttar - tolorance; // The tolorances for each constraint
bool heater_on = false;         // Heater starts of not on
bool hit_max = false;           // Has not entered max of min yet
bool hit_min = false;           // Used to determine the on off cycle of the heater
unsigned long int passer;       // Variable to pass the value of wether the heater is on or off

// OD and DIGIPOT MECHANICS

int digi_position; // The position of the digipot
int digi_value;    // The value passed to the digiwrite function

// Adding indepentent action functions

void datadump()

{

  MYDATA = SD.open(filename, FILE_WRITE);
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
  Serial.print(sizeof(buffer));
  Serial.print("Position of the file is: ");
  Serial.print(MYDATA.position());
  Serial.print("The file Exist? ");
  Serial.print(SD.exists(filename));
  Serial.print("The size of the file is now: ");
  Serial.println(MYDATA.size());
  MYDATA.close();
}

int digiwrite(int digi_value)
{

  SPI.beginTransaction(mySetting);      // The transaction settings from the specific Arduino
  digitalWrite(slave_select_digi, LOW); // Once low the Settings shift to allow writing
  SPI.transfer(0x00);                   // The 8 bit address that is all zeros to prep to write the bit 0-255 for MCP4151
  SPI.transfer(digi_value);
  digitalWrite(slave_select_digi, HIGH); // Switching recieving bit back to zero
  SPI.endTransaction();
  return 0; // Ends the transaction for this specific spi device
}

void datastore_add(){

  buffer[place].millistime = millis();
  buffer[place].Voltage_analog_input = analogRead(sensing_pin_op_amp);                               // Analog Reading of OD
  buffer[place].digi_pot_wiper_position = digi_position;                                             // Where the digipot is
  buffer[place].T_Water = sensors.getTempC(sensorA);                                                 // Temp Values
  buffer[place].T_average_of_Al_Block = (sensors.getTempC(sensorB) + sensors.getTempC(sensorC)) / 2; // Temp value
  buffer[place].pump_speed_setting = push_pump;                                                        // Keeps a record of the pump speed
  buffer[place].temp_baseline = ttar;                                                            // To allow comparision of the baseline temp target and the actual temperature
  buffer[place].heater_state = passer;
  buffer[place].microtime = micros();
  
}

void temp_stabilizer()
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

void system_status()
{

  Serial.println(" Digipot Position is: ");
  Serial.print(digi_position);
  Serial.println("Optic Integer [0-1024]: ");
  Serial.print(analogRead(sensing_pin_op_amp));
  Serial.println("\n Pump Speed");
  Serial.print(push_pump);
  Serial.println("\n Temp Baseline: ");
  Serial.print(ttar);
  sensors.requestTemperatures();
  Serial.print("Sensor A: ");
  Serial.print(sensors.getTempC(sensorA));

  Serial.print(" [C] ,");
  Serial.print("Sensor B: ");
  Serial.print(sensors.getTempC(sensorB));

  Serial.print(" [C] ,");
  Serial.print(" Sensor C: ");
  Serial.print(sensors.getTempC(sensorC));
  Serial.println(" [C] ");

  Serial.println("At time:  ");
  Serial.print(millis() / 60000);
  Serial.print(" Minutes ");
  Serial.print("Structure position: ");
  Serial.print(place);
}

void handleInterrupt()
{
  inter_on = true;
}

void initalize_pump_and_interupt()
{

  pinMode(IRupt, INPUT_PULLUP);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA_MotorPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(IRupt), handleInterrupt, FALLING);
}

void user_choice_interface()
{
  int timeout = millis();

  Serial.flush(); // To make sure if the user presses a faulty key the program does not fail

  Serial.println("\n\n\n\n\nChoose a Mode");
  Serial.println("Type 1 for Pump Control");
  Serial.println("Type 2 for Temp Control");
  Serial.println("Type 3 for Turning on or off the Pump");
  Serial.println("Type 4 for Pump Direction Switch");
  Serial.println("Type 5 to start or end a recording program ");
  Serial.println("Type 6 to Start or Stop Live data feed");

  Serial.flush(); // To make sure if the user presses a faulty key the program does not fail
  while (millis() - timeout <= 6000)
  {
  }
  int decider = Serial.parseInt();

  switch (decider)
  {

  case 1:

    Serial.println("Wait for Pump Setting: ");
    Serial.println("Please type the pump control: (0-255), Type xxx or xx. for x < 100");
    Serial.println("Make sure pump is on first");

    while (Serial.available() < 3 || Serial.peek() == ".")
    {
      delay(100); // Wait for input
    }
    push_pump = Serial.parseInt();
    analogWrite(ENA_MotorPin, push_pump);
    Serial.print("Pump set to analog setting of: ");
    Serial.print(push_pump);
    

    break;
  case 2:

    Serial.println("Wait for TempSetting");
    Serial.println("Please type temp setting (20 C < T < 50 C ) Type xx.xx ");
    while (Serial.available() < 5 || Serial.peek() == ".")
    {
      delay(100); // Wait for input
    }
    push_temp = Serial.parseFloat();
    Serial.print("Set temp baseline to: ");
    Serial.print(push_temp);
    Serial.print(" C");
    ttar = push_temp;
    t_max = ttar + tolorance; // The tolorances for each constraint
    t_min = ttar - tolorance; // The tolorances for each constraint
    temp_stabilizer();
    break;
  case 3:
    Serial.println("Switching Pump State");
    Pump_State ^= true;
    // Switching the value from its orgional if off then on if on then off
    if (Pump_State == true)
    {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
    }
    else if (Pump_State == false)
    {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA_MotorPin, 10);
    }
    break;
  case 4:
    if (digitalRead(IN1) == 1 && digitalRead(IN2) == 0)
    {

      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      Serial.println("Switch Motor Flow Direction");
    }
    else if (digitalRead(IN1) == 0 && digitalRead(IN2) == 1)
    {

      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      Serial.print("Switched Motor Flow Direction");
    }
    else
    {

      Serial.print("Should Turn on Pump First");
    }
    break;
  case 5:

    data_probe = true;

    break;

  case 6:

    displaying_serial ^= true;
    break;

  default:
    Serial.print("Rerun the Interrupt Again Error Occured");

    break;
  }

  Serial.println("Execution of Order Complete please wait until menu avalible again");
  delay(1000);
  Serial.println("\n\n\nMenu avalible again press button to use");
}

void setup()
{

  // Making pinModes

  // Pump and interrupt
  pinMode(IRupt, INPUT_PULLUP);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA_MotorPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(IRupt), handleInterrupt, FALLING);

  // Relay and Temp

  pinMode(activator1, OUTPUT);
  pinMode(activator2, OUTPUT);
  digitalWrite(activator1, LOW);
  digitalWrite(activator2, LOW);

  // OD and digipot

  pinMode(slave_select_digi, OUTPUT);
  pinMode(sensing_pin_op_amp, INPUT);
  pinMode(HIGH_PIN, OUTPUT);
  digitalWrite(HIGH_PIN, OUTPUT);

  // Now setting up libraries and initalizing

  Serial.begin(serial_speed);
  sensors.begin();
  SPI.begin();


  // Temp Sensor Check

  Serial.print("Locating devices...");
  Serial.print("Found ");
  device_count = sensors.getDeviceCount();
  Serial.print(device_count, DEC);
  Serial.println(" devices.");
  Serial.println("");
}

void loop()
{

  if (inter_on)
  {

    inter_on = false;
    delay(3000);
    user_choice_interface();
  }

  if (data_probe)
  {
    data_probe = false;
    bool skip = false;
    if (data_is_running)
    {
      skip = true;
      data_is_running ^= true;
      ttar = 22;
      digitalWrite(activator1,LOW);
      digitalWrite(activator2, LOW);
      SD.end();
      Serial.println("File Growth Run Complete:");
      Serial.println("You may remove SD CARD");


      
    }
    if (!data_is_running && !skip)
    {
          if (!SD.begin(Chip_Select_Pin))
      {
        Serial.print("SD Fail");
        Serial.println("Reformat the SD card to fix");
      }
      else if (SD.begin(Chip_Select_Pin))
  {

    Serial.println("SD Pass");
  }
      Serial.println("Beginning Data Log Do Not Remove SD Card");
      data_is_running ^= true;
      skip = true;
      place = 0;

      if (SD.exists(filename))
      {
        SD.remove(filename);
      }
    }
  }

  if (data_is_running)
  {
    if(switcher){
      digiwrite(15);
    }
    
    datastore_add();
    place++;
    delay(250);

    if (displaying_serial)
    {
      system_status();
    }
    if (place > datalen-1 )
    {
      datadump();
      temp_stabilizer();
      place = 0;
      switcher ^= true;
    }
  }
}