// this file is a copy of the (normal C, runs on computer) write_example.ccpp -- we pulled the important parts of the setup from SD/SD-Card-Testing.cpp, which works, and then adapted the rest to use methods of the arduino File object instead of fopen, fwrite, etc
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#define SD_CHIP_SELECT_PIN 4
/* This is where  is where I start my addition of the logging code without touching the rest

THIS IS ONLY SETTING UP THE VARIABLES*/

int VRead; //Sets up a reader for Analog Voltage Reading to A0

int NumCount; //Sets up a number counter to track the amount of times VRead Before Averaging it
              // note that you don't necessarily need to average any more --
              // since you're storing binary, you could maybe just log
              // continuously (depending on how much data that would generate)
              // For now, let's leave it how it is, and see how much data is accumulated in a minute.

float VRec; //This is your average value -- because it comes from a division, let's use a floating point here

int numtarget =  10; // The value which is set for the amount of data taken before an accuracy check
                     // don't know what an accuracy check is

int astore; // The  value which holds the analog input sum until it is time to dump the average
            // I ask -- if averaging, why not just store the average in a
            // floating point number, and each time you acquire data,
            // add VRead/NumCount


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
char *filename="test.dat";
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

    // Write adds to end, so remove any preexisting file
    if(SD.exists(filename)){
        SD.remove(filename);
    }
    loopcounter = 0;
    start_time = millis();
    data_written = 0;
    // Open once, at the beginning 
    myFile = SD.open(filename, FILE_WRITE);
    // since this is inside the setup() function, I indent it for readability
    //
    /*This is where I start the set up I WILL SET THE READINGS VALUES TO ZERO TO BEGIN WITH*/

    VRead = 0; // All variables will start a zero and will change as we continue
    NumCount = 0;
    VRec = 0;
    astore = 0;

    //Setting the pin modes

    pinMode(A0, INPUT);

    //The serial begin has already been set so I will no do that
}

void loop()
{
    int j,k;
    int num_written;
    j = loopcounter % datalen;
    if(millis() - start_time < 3000) // go for 3 secs -- in reality, could be replaced with button press, etc.
    {
        //// {{{ Serial logging -- this is your code -- I just moved it inside
        ///      the code block that was intended for detection.  It doesn't
        ///      make sense for this to be in an outer block -- you want the
        ///      actual acquisition to replace the fake data!
        
        //Now starting the edit to the serial logging

        VRead = analogRead(A0);
        astore += VRead;
        NumCount += 1;

        if(NumCount == numtarget){
            VRec = (float) astore/(float) numtarget; //Calculating the average of the given data
            // previous converted to a floating point, otherwise you are losing accuracy.
            // this does meant that we will need to change the structure and
            // the file that reads the structure.
            NumCount = 0;
            astore = 0;
            Serial.print("Analog (V) of ");
            Serial.print(VRec);
            Serial.print(" at time: ");
            Serial.print(millis()/1000); // Taking the millis and dividing by 1000 to record seconds
            Serial.println(" sec");
            // I removed the delay statement here -- not sure what purpose it was serving
            
            // note that if we stick with averaging, we would only write the
            // averaged points to the binary data, so all the stuff below would
            // be added to this block, here.
        }
        // }}}
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
            num_written = myFile.write((const uint8_t *)mydata, sizeof(mydata));//Parentheses before variable declares variable 

            Serial.println("wrote a chunk");
            Serial.println(num_written);
        }
    }else if(loopcounter>0 && !data_written){//Single "&" is pointer converter double and is logical "and" operator
        // This means we're done, so go ahead and close the file
        Serial.println("binary data done");
        data_written = 1;
        myFile.close();
    }

    loopcounter += 1;
    if(loopcounter>50000 && loopcounter % datalen == 0){
        loopcounter=0;// prevent overflow by rolling over
    }
}


