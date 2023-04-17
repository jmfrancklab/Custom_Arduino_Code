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



// Creating a structure for the temperature recording...

struct temprecord { 
  float T_1;
  float T_2; 
  float Average_of_Ts;
  unsigned long int millistime;

};

File tempfile; //Created an instance of the open file
const int datalen = 20; 
temprecord buffer[datalen];
              // Size off each struct or... amount of time it takes to dump data to SD NOTE MUST BE CONST INT
const char *filename = "Temp_Rec.DAT"; // The name of the file IMPORTANT CANNOT HAVE  more than 8 character within the name must follow the file naming system FAT 32

void temp_rec(int j){

sensors.requestTemperatures();
buffer[j].T_1 = sensors.getTempCByIndex(0);
buffer[j].T_2 = sensors.getTempCByIndex(1);
buffer[j].Average_of_Ts = (sensors.getTempCByIndex(0) +  sensors.getTempCByIndex(1))/2;
buffer[j].millistime = millis();


}

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
int deviceCount = 0;


void setup(void)
{

  sensors.begin();	// Start up the library
  Serial.begin(9600);
  checkSD();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  deviceCount = sensors.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
}




void loop(void)
{ 
  int j = 0;
  for(j; j <=datalen-1; j++)
  temp_rec(j);
  delay(200);

datadump();
delay(1000);


}