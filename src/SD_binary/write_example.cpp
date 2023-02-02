// this file is a copy of the (normal C, runs on computer) write_example.ccpp -- we pulled the important parts of the setup from SD/SD-Card-Testing.cpp, which works, and then adapted the rest to use methods of the arduino File object instead of fopen, fwrite, etc
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#define SD_CHIP_SELECT_PIN 4

//A structure which two elements
struct Datastore {
    // note that the types on the arduino are slightly different, and you should
    // make sure that you match the type of the data that you're trying to
    // store!!
    unsigned int Voltage; // this should just be a raw arduino reading DO NOT DO THE OD MATH ON THE ARDUINO!!!
    unsigned long int time; // similarly, just the result of millis
    unsigned long int time_short;
};

File myFile;
int loopcounter;// I had thought about making this long, but if we do that, and do math with regular numbers, everything goes crazy!
const int datalen = 10;
const char *filename="test.dat";
Datastore mydata[datalen];

int data_written;
unsigned long int start_time;

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    digitalWrite(10,HIGH);

    Serial.print("Initializing SD card...");

    if (!SD.begin(SD_CHIP_SELECT_PIN)) {//even though this argument is technically optional, we do need to select chip 4 for this to work!!
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    if(SD.exists(filename)){
        SD.remove(filename);
    }
    loopcounter = 0;
    start_time = millis();
    data_written = 0;
    myFile = SD.open(filename, FILE_WRITE);
}

void loop()
{
    int j,k;
    int num_written;
    j = loopcounter % datalen;
    // open the file. note that only one file can be open at a time,
    if(millis() - start_time < 3000) // go for 3 secs -- in reality, could be replaced with button press, etc.
    {
        mydata[j].Voltage = (loopcounter % 5) * 10; // generate fake voltage data that's just 0, 10, 20... 40 and cycles
        mydata[j].time = millis();
        mydata[j].time_short = micros();
        Serial.println("j,V,t:");
        //Serial.println(loopcounter % 5);
        Serial.println(j);
        Serial.println(mydata[j].Voltage);
        Serial.println(mydata[j].time);
        if (loopcounter>0 && j == datalen-1){// write data only once we've filled up mydata
            for(k=0;k<datalen;k++){
                Serial.print("datapoint: ");
                Serial.print(k); Serial.print(": ");
                Serial.print(mydata[k].Voltage);
                Serial.print(",");
                Serial.println(mydata[k].time);
            }
            num_written = myFile.write((const uint8_t *)mydata, sizeof(mydata));

            Serial.println("wrote a chunk");
            Serial.println(num_written);
        }
    }else if(loopcounter>0 && !data_written){
        Serial.println("binary data done");
        data_written = 1;
        myFile.close();
    }

    loopcounter += 1;
    if(loopcounter>50000 && loopcounter % datalen == 0){
        loopcounter=0;// prevent overflow by rolling over
    }
}



