// this file is a copy of the (normal C, runs on computer) write_example.ccpp -- we pulled the important parts of the setup from SD/SD-Card-Testing.cpp, which works, and then adapted the rest to use methods of the arduino File object instead of fopen, fwrite, etc
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

File myFile;

//A structure which two elements
struct Datastore {
  // note that the types on the arduino are slightly different, and you should
  // make sure that you match the type of the data that you're trying to
  // store!!
  unsigned int Voltage; // this should just be a raw arduino reading DO NOT DO THE OD MATH ON THE ARDUINO!!!
  double time; // I'm making a very general example where we are mixing types.  I don't know what the native type of time is on the arduino!
};

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  digitalWrite(10,HIGH);

  Serial.print("Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.dat", FILE_WRITE);
}

int main(void)
{
    int j;
    int datalen = 10;
    Datastore mydata[datalen];
    for(j=0;j<datalen;j++){
        mydata[j].Voltage = j*10; // generate fake voltage data that's just 0, 10, 20... 90
        mydata[j].time = 1.0*j + 0.1; // generate fake time data that's 0.1, 1.1, 2.1, etc.
        //printf("wrote %d %f\n",mydata[j].Voltage, mydata[j].time);
    }
    // I do the following because I don't know offhand how many bytes of
    // memory/filespace are needed to store each type.  this tells me
    // I need this for numpy.
    //printf("sizeof voltage %d time %d overall %d\n",sizeof mydata[0].Voltage,
    //        sizeof mydata[0].time, sizeof mydata[0]);
    // {{{ in this block, the arduino commands will be similar, but somewhat different!
    size_t r1 = myFile.write(mydata, sizeof(mydata));
    //in the example, he does this, which doesn't seem right to me --
    //mydata without an array index is already a pointer, so what's up
    //with this?
    //size_t r1 = myFile.write((const uint8_t *)&mydata, sizeof(mydata));
    //printf("wrote %zu elements out of %d requested\n", r1,  datalen);
    myFile.close();
    // }}}
}



