// Including the necessary libraries

#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <SD.h>
#include <SPI.h>

// Now defining important pins and instances hardwired

// TEMP PINS

// JF comment: it's convention for macros to be uppercase
#define ONE_WIRE_BUS 3 // The busline for the three temperature sensors
#define ACTIVATOR1 7   // The Relay pin for Bang Control
#define ACTIVATOR2 6   // The second relay control switch for Bang Control

// OPTICS AND SD

#define SENSING_PIN_OP_AMP A0 // The reading for cell count corrolation
#define SLAVE_SELECT_DIGI 53  // The slave communicator for the Digipot
#define CHIP_SELECT_PIN 4     // The ethernet and SD chip select pin
#define HIGH_PIN 10           // In order for the arduino sheild to work must be present
// PUMP CONTROL

// JF comment: what is ENA?

#define ENA_MOTORPIN 5 // Pin for PWM SPEED of PUMP MODULATION
#define IN1 A4         // Logical Determiner for on and off
#define IN2 A1         // Logical Determiner for on and off
#define DVOLT A2       // The voltage digipot Sensor

// Button

#define Button 2 // The pin which opens up the menu for user interfacing

// DEFINING STRUCURES INSTENCES AND ADDRESSES

// Temp Control

// JF comment: why isn't this done in setup?
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
    unsigned long int DigiV;
};

File data_fileobj; // for storing file instance -- note that similarly,
                   // things that are not macros should NOT be all caps

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


int decider;                  // Helps with deciding which side program to activate
bool data_is_running = false; // A conditional switch to true after a command
bool displaying_serial = false;
bool data_probe = false;
bool switcher = true;
// Temp mechanics
int device_count; // The amout of devices that are counted of temp prboes berfore program start
float ttar = 40;  // Target temp 22 as defult
float tolorance = 0.05;
float t_max = ttar + tolorance; // The tolorances for each constraint
float t_min = ttar - tolorance; // The tolorances for each constraint
bool heater_on = false;         // Heater starts of not on
bool hit_max = false;           // Has not entered max of min yet
bool hit_min = false;           // Used to determine the on off cycle of the heater
unsigned long int passer;       // Variable to pass the value of wether the heater is on or off
int timeout = 3000;
// OD and DIGIPOT MECHANICS

int digi_position; // The position of the digipot
int digi_value;    // The value passed to the digiwrite function

// Digipot Modulation Mechanics;

int high_range_digi = 0;
int low_range_digi = 80;
int modulation_low_range_low = 200;
int modulation_low_range_high = 400;
int modulation_high_range_high = 900;
int modulation_high_range_low = 700;

int high_range_digi = 10;
int low_range_digi = 70;
int modulation_low_range_low = 275;
int modulation_low_range_high = 475;
int modulation_high_range_high = 900;
int modulation_high_range_low = 500;

// button_check
int last_button_press;
bool button_toggle_state = false;

void button_check()
{

    if (digitalRead(Button) == HIGH) // is the button currently down?
        last_button_press = millis();
    {
        if (digitalRead(Button) == HIGH && millis() - last_button_press > 1000) // b/c we don't want the following if we're continuously holding down the button
        {
            button_toggle_state = true; // flip state of buttonToggleState
        }
    }
}
void modulate()
{

    if (switcher)
    {
        for (int i; i < 5; i++)
        {
            if (buffer[place].Voltage_analog_input < modulation_high_range_low)
            {
                digi_position = high_range_digi--;
            }
            if (buffer[place].Voltage_analog_input > modulation_high_range_high)
            {
                digi_position = high_range_digi++;
            }
        }
    }
    if (!switcher)
    {
        for (int i; i < 5; i++)
        {
            if (buffer[place].Voltage_analog_input < modulation_low_range_low)
            {
                digi_position = low_range_digi--;
            }
            if (buffer[place].Voltage_analog_input > modulation_low_range_high)
            {
                digi_position = low_range_digi++;
            }
        }
    }
}
// Adding indepentent action functions

void datadump()

{

    data_fileobj = SD.open(filename, FILE_WRITE);
    data_wrote = data_fileobj.write((const uint8_t *)buffer, sizeof(buffer)); // Writing to the file
    data_fileobj.seek(EOF);
    Serial.print("Wrote: ");
    Serial.print(data_wrote); // To check if data actually written
    Serial.print(" much data");
    Serial.println("Is data avalible? ");
    Serial.print(data_fileobj.available());
    Serial.println("The name of the file is: ");
    Serial.print(data_fileobj.name());
    Serial.println("Size of Structure: ");
    Serial.print(sizeof(buffer));
    Serial.println("Position of the file is: ");
    Serial.print(data_fileobj.position());
    Serial.println("The file Exist? ");
    Serial.print(SD.exists(filename));
    Serial.println("The size of the file is now: ");
    Serial.print(data_fileobj.size());
    data_fileobj.close();
}

unsigned long int digiwrite(unsigned long int digi_value)
{

    SPI.beginTransaction(mySetting);      // The transaction settings from the specific Arduino
    digitalWrite(SLAVE_SELECT_DIGI, LOW); // Once low the Settings shift to allow writing
    SPI.transfer(0x00);                   // The 8 bit address that is all zeros to prep to write the bit 0-255 for MCP4151
    SPI.transfer(digi_value);
    digitalWrite(SLAVE_SELECT_DIGI, HIGH); // Switching recieving bit back to zero
    SPI.endTransaction();
    return digi_value; // Ends the transaction for this specific spi device
}

void datastore_add()
{

    buffer[place].millistime = millis();
    buffer[place].Voltage_analog_input = analogRead(SENSING_PIN_OP_AMP);                               // Analog Reading of OD
    buffer[place].digi_pot_wiper_position = digi_position;                                             // Where the digipot is
    sensors.requestTemperatures();                                                                     // Put requests the temperature
    buffer[place].T_Water = sensors.getTempC(sensorA);                                                 // Temp Values
    buffer[place].T_average_of_Al_Block = (sensors.getTempC(sensorB) + sensors.getTempC(sensorC)) / 2; // Temp value
    buffer[place].pump_speed_setting = push_pump;                                                      // Keeps a record of the pump speed
    buffer[place].temp_baseline = ttar;                                                                // To allow comparision of the baseline temp target and the actual temperature
    buffer[place].heater_state = passer;
    buffer[place].microtime = micros();
    buffer[place].DigiV = analogRead(DVOLT);
}

void temp_stabilizer()
{

    if (buffer[place].T_average_of_Al_Block >= t_max && !hit_max)
    {
        passer = 0;
        digitalWrite(ACTIVATOR1, LOW);
        digitalWrite(ACTIVATOR2, LOW);
        hit_max = true;
        hit_min = false;

        Serial.println("Heater Off");
    }
    if (buffer[place].T_average_of_Al_Block <= t_min && !hit_min)
    {
        passer = 1;
        digitalWrite(ACTIVATOR1, HIGH);
        digitalWrite(ACTIVATOR2, HIGH);
        hit_min = true;
        hit_max = false;

        Serial.println("Heater On");
    }
}

void system_status()
{

    Serial.print(" Digipot Position is: ");

    Serial.print(buffer[place].digi_pot_wiper_position);
    Serial.println("Optic Integer [0-1024]: ");
    Serial.print(buffer[place].Voltage_analog_input);
    Serial.println("Pump Speed");
    Serial.print(buffer[place].pump_speed_setting); // JF comment: give this variable a better name
    Serial.println("Temp Setting: ");
    Serial.print(buffer[place].temp_baseline); // JF comment: give this variable a better name
    Serial.print("Sensor A: ");
    Serial.print(buffer[place].T_Water);

    Serial.print(" [C] ,");
    Serial.println("Sensor B, C Average: ");
    Serial.print(buffer[place].T_average_of_Al_Block);
    Serial.println("At time:  ");
    Serial.print(buffer[place].millistime / 60000);
    Serial.print(" Minutes ");
    Serial.println("Structure position: ");
    Serial.print(place);
    Serial.println("Heater state: ");
    Serial.print(buffer[place].heater_state);
}

void initalize_pump_and_interupt()
{

    pinMode(Button, INPUT_PULLUP);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA_MOTORPIN, OUTPUT);
}

void user_choice_interface()
{

    Serial.flush(); // To make sure if the user presses a faulty key the program does not fail

    Serial.println("\n\n\n\n\nChoose a Mode");
    Serial.println("Type 1 for Pump Control");
    Serial.println("Type 2 for Temp Control");
    Serial.println("Type 3 for Turning on or off the Pump");
    Serial.println("Type 4 for Pump Direction Switch");
    Serial.println("Type 5 to start or end a recording program ");
    Serial.println("Type 6 to Start or Stop Live data feed for debugging");

    Serial.flush(); // To make sure if the user presses a faulty key the program does not fail
    while (!Serial.available() && millis() - timeout <= 3000)
    {
    }
    int decider = Serial.parseInt(); // JF comment: what happens if this is not an integer?

    switch (decider)
    {

    case 1: // pump speed

        Serial.println("Wait for Pump Setting: ");
        Serial.println("Please type the pump control: (0-255), Type xxx or xx. for x < 100");
        Serial.println("Make sure pump is on first");

        while (Serial.available() < 3 || Serial.peek() == -1)
        {
            delay(100); // Wait for input
        }
        push_pump = Serial.parseInt();
        analogWrite(ENA_MOTORPIN, push_pump);
        Serial.print("Pump set to analog setting of: ");
        Serial.print(push_pump);

        break;
    case 2: // temp

        Serial.println("Wait for TempSetting");
        Serial.println("Please type temp setting (20 C < T < 50 C ) Type xx.xx ");
        while (Serial.available() < 5 || Serial.peek() == -1)
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

        break;
    case 3: // pump power
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
            analogWrite(ENA_MOTORPIN, 10); // JF comment: what is 10 here? duty cycle as percentage? as fraction of 256?
        }
        break;
    case 4: // direction
        if (digitalRead(IN1) == 1 && digitalRead(IN2) == 0)
        {

            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            Serial.println("Motor set to reverse direction");
        }
        else if (digitalRead(IN1) == 0 && digitalRead(IN2) == 1)
        {

            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            Serial.println("Motor set to forward direction");
        }
        else
        {

            Serial.print("Should Turn on Pump First");
        }
        break;
    case 5: // data

        data_probe = true;

        break;

    case 6: // debug

        displaying_serial ^= true;
        break;

    default:
        Serial.print("Rerun the Interrupt Again Error Occured");

        break;
    }

    Serial.println("Execution of Order Complete please wait until menu available again");
    Serial.println("\n\n\nMenu available again press button to use");
}

void setup()
{

    // Making pinModes

    // Pump and interrupt

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA_MOTORPIN, OUTPUT);

    // Relay and Temp

    pinMode(ACTIVATOR1, OUTPUT);
    pinMode(ACTIVATOR2, OUTPUT);
    digitalWrite(ACTIVATOR1, LOW);
    digitalWrite(ACTIVATOR2, LOW);

    // OD and digipot
    pinMode(DVOLT, INPUT);
    pinMode(SLAVE_SELECT_DIGI, OUTPUT);
    pinMode(SENSING_PIN_OP_AMP, INPUT);
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
button_check();
   
    if (button_toggle_state)
    {
        // JF comment: why do this? why not just set user_choice_interface as the callback?
        button_toggle_state = false;
        delay(1000);
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
            digitalWrite(ACTIVATOR1, LOW);
            digitalWrite(ACTIVATOR2, LOW);
            SD.end();
            Serial.println("File Growth Run Complete:");
            Serial.println("You may remove SD CARD");
        }
        if (!data_is_running && !skip)
        {
            if (!SD.begin(CHIP_SELECT_PIN))
            {
                Serial.print("SD Fail");
                Serial.println("Reformat the SD card to fix");
            }
            else if (SD.begin(CHIP_SELECT_PIN))
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

        modulate();
        digiwrite(digi_position);
        temp_stabilizer();
        datastore_add();
        place++;
        delay(100);

        if (displaying_serial)
        {
            system_status();
        }
        if (place > datalen - 1)
        {
            datadump();
            place = 0;
            switcher ^= true;
        }
    }
}
