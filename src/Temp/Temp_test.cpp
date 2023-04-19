#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <SD.h>



// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);	
// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);
const uint8_t sensorA[8] = {0x28, 0x26, 0x3B, 0x94, 0x97, 0x0A, 0x03, 0x56};
const uint8_t sensorB[8] = {0x28, 0x0B, 0xB3, 0x79, 0x97, 0x00, 0x03, 0x6C};
const uint8_t sensorC[8] = {0x28, 0x10, 0x0A, 0x94, 0x97, 0x0A, 0x03, 0x40};
//0x28, 0x26, 0x3B, 0x94, 0x97, 0x0A, 0x03, 0x56 is A
//0x28, 0x0B, 0xB3, 0x79, 0x97, 0x00, 0x03, 0x6C is B
//0x28, 0x10, 0x0A, 0x94, 0x97, 0x0A, 0x03, 0x40 is C
int deviceCount = 0; // The devicecount checker or how many temp recorders are there
// Creating a structure for the temperature recording... and FILE SET UP STUFF
struct temprecord { 
  float T_Water;
  float T_average_of_Al_Block; 
  unsigned long int millistime;
  unsigned long int heater_state;
};
File tempfile; //Created an instance of the open file
const int datalen = 20; 
temprecord buffer[datalen]; // Size off each struct or... amount of time it takes to dump data to SD NOTE MUST BE CONST INT
const char *filename = "Temp_Rec.DAT"; // The name of the file IMPORTANT CANNOT HAVE  more than 8 character within the name must follow the file naming system FAT 32
void datadump()

{
  int data_wrote;
  tempfile = SD.open(filename,FILE_WRITE);

  data_wrote = tempfile.write((const uint8_t *)buffer,sizeof(buffer)); // Writing to the file
  tempfile.seek(EOF);
  Serial.print("Wrote: ");
  Serial.print(data_wrote); // To check if data actually written
  Serial.print(" much data\n");
  Serial.print("Is data avalible? ");
  Serial.println(tempfile.available());
  Serial.print("The name of the file is: ");
  Serial.println(tempfile.name());
  Serial.print("Size of Structure: ");
  Serial.println(sizeof(buffer));
  Serial.print("Position of the file is: ");
  Serial.println(tempfile.position());
  Serial.print("The file Exist? ");
  Serial.println(SD.exists(filename));
  Serial.print("The size of the file is now: ");
  Serial.println(tempfile.size());
  tempfile.close();
}
void checkSD()
{
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  Serial.print("Initializing SD card...");
  if (!SD.begin(4))
  {                                           // even though this argument is technically optional, we do need to select chip 4 for this to work!!
    Serial.println("initialization failed!"); // Shows a problem if the SD never acually is reconigized by arduino
    return;
  }
  Serial.println("initialization done."); // Confirms Arduino SD error is not the problem
  
  if (SD.exists(filename)){  // If already a file
        SD.remove(filename); // Remove the file to stop weird confusing
    }
   
  
}

//Temperature Setup stuff for writing to SD card as well as printing to serial and relay activator variables;
float ttar = 37;
float tolorance = 0.25;
float t_max = ttar + tolorance; // The tolorances for each constraint
float t_min = ttar - tolorance; // The tolorances for each constraint

// The relay activators; and heat controllers
int activator1 = 7;
int activator2 = 6;
bool heater_on = false; // Heater starts of not on
bool hit_max = false;   // Has not entered max of min yet
bool hit_min = false;
unsigned long int passer;

//The two functions below one checks the function for the correct range within the temp and the other checks or recording

void temp_rec(int j){

sensors.requestTemperatures();
buffer[j].T_Water = sensors.getTempC(sensorA);
buffer[j].T_average_of_Al_Block= (sensors.getTempC(sensorB)+sensors.getTempC(sensorC))/2;
buffer[j].millistime = millis();
buffer[j].heater_state = passer;

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
void temp_report(){
  
  sensors.requestTemperatures();
  Serial.print("Sensor A: ");
  Serial.print(sensors.getTempC(sensorA));
  Serial.print((char)176);//shows degrees character
  Serial.print(" C ,");
  Serial.print("Sensor B: ");
  Serial.print(sensors.getTempC(sensorB));
  Serial.print((char)176);//shows degrees character
  Serial.print(" C ,");
  Serial.print(" Sensor C: ");
  Serial.print(sensors.getTempC(sensorC));
  Serial.print((char)176);//shows degrees character
  Serial.print(" C ");

}

//Makes the steps to ready up the sensor
void initalize_t_and_relay (){
  sensors.begin();	// Start up the library
  Serial.print("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
  pinMode(activator1,OUTPUT);
  pinMode(activator2,OUTPUT);
  digitalWrite(activator1,LOW);
  digitalWrite(activator2,LOW);



}
void setup(void)
{
  Serial.begin(9600);
  checkSD();
  initalize_t_and_relay();
  
  
}
void loop(void)
{ 
  int j = 0;
  for(j; j <=datalen-1; j++)
  temp_rec(j);
  delay(200);

datadump();
temp_stabilizer();
temp_report();

delay(1000);


}