// this file is a copy of the (normal C, runs on computer) write_example.ccpp -- we pulled the important parts of the setup from SD/SD-Card-Testing.cpp, which works, and then adapted the rest to use methods of the arduino File object instead of fopen, fwrite, etc
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#define SD_CHIP_SELECT_PIN 4

File myFile;

//A structure which two elements
struct Datastore {
  // note that the types on the arduino are slightly different, and you should
  // make sure that you match the type of the data that you're trying to
  // store!!
  unsigned int Voltage; // this should just be a raw arduino reading DO NOT DO THE OD MATH ON THE ARDUINO!!!
  long int time; // similarly, just the result of millis
};

const char *filename="test.dat";
void setup()
{
    int j;
    int datalen = 10;
    int num_written;

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
    myFile = SD.open(filename, FILE_WRITE);
    Datastore mydata[datalen];
    for(j=0;j<datalen;j++){
        mydata[j].Voltage = j*10; // generate fake voltage data that's just 0, 10, 20... 90
        mydata[j].time = j + 1000; // generate fake time data that's 0.1, 1.1, 2.1, etc.
                                      //printf("wrote %d %f\n",mydata[j].Voltage, mydata[j].time);
    }
    // {{{ in this block, the arduino commands will be similar, but somewhat different!
    num_written = myFile.write((const uint8_t *)mydata, sizeof(mydata));
    myFile.close();
    Serial.println("wrote data");
    Serial.println("use this to det struct size");
    Serial.println(sizeof(mydata));
    Serial.println("this is how much I wrote");
    Serial.println(num_written);
    // }}}
}

void loop(){}



