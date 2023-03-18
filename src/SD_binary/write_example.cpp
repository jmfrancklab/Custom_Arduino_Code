// this file is a copy of the (normal C, runs on computer) write_example.ccpp -- we pulled the important parts of the setup from SD/SD-Card-Testing.cpp, which works, and then adapted the rest to use methods of the arduino File object instead of fopen, fwrite, etc
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define SD_CHIP_SELECT_PIN 4
#define SPI_CHIP_SELECT 10 // Defining the slave pin
/* This is where  is where I start my addition of the logging code without touching the rest

THIS IS ONLY SETTING UP THE VARIABLES*/
int checker = 8;


unsigned long int lastlog;
unsigned long int log_interval = 1000;// log every minute
bool buttonToggleState;
bool buttonDown;
unsigned long lastButtonPress;
int slavecom = 10;
int VRead;    // Sets up a reader for Analog Voltage Reading to A0
int NumCount; // Sets up a number counter to track the amount of times VRead Before Averaging it
              //  note that you don't necessarily need to average any more --
              //  since you're storing binary, you could maybe just log
              //  continuously (depending on how much data that would generate)
              //  For now, let's leave it how it is, and see how much data is accumulated in a minute.
float VRec;   // This is your average value -- because it comes from a division, let's use a floating point here

int numtarget = 10; // The value which is set for the amount of data taken before an accuracy check
                    // don't know what an accuracy check is
int astore;         // The  value which holds the analog input sum until it is time to dump the average
                    // I ask -- if averaging, why not just store the average in a
                    // floating point number, and each time you acquire data,
                    // add VRead/NumCount

// Pump Mechanics
int pup = 7;
int pdown = 6;
int pcontrol = 3;
int grace;
int inter;
// Eli Response 2/18 : I don't understand your idea

// A structure which two elements
struct Datastore
{
    // note that the types on the arduino are slightly different, and you should
    // make sure that you match the type of the data that you're trying to
    // store!!
    float Voltage;                // this should just be a raw arduino reading DO NOT DO THE OD MATH ON THE ARDUINO!!!
    unsigned long int time;       // similarly, just the result of millis
    unsigned long int time_short; // What is the purpose of this JF answers -- to get more accurate time!
};

File myFile;
File logFile;
// note that I renamed totalDatapointCounter to totalDatapointCounter, since it represents the total number of (averaged) datapoints that we've stored
int totalDatapointCounter; // I had thought about making this long, but if we do that, and do math with regular numbers, everything goes crazy!
const int datalen = 10;    // So ten data points will be stored at a time? 2/18/ --Eli Paster JF answers -- YES! but we can increase the size of this buffer just by changing this number
char *filename = "test.dat";
Datastore mydata[datalen]; // this is the "buffer" -- we want to hold as many datapoints in memory as possible, since writing to disk is the slow step

int halter; // do not set the value when declaring -- that's what you do for constants
unsigned long int start_time;



 
void Pump_Control(void){
                grace = millis();
Serial.println("You may control pump flow");




            while(millis() - grace <= 10000){
                if(digitalRead(pup) == LOW){ 
                    if(inter<255){
                    inter++;
                    analogWrite(pcontrol,inter);
                    delay(50);    
                    }
                }
                    if(digitalRead(pdown) == LOW){ 
                        if(inter>0){
                            inter --;
                            analogWrite(pcontrol,inter);
                            delay(50);    
                        };

                    ///While holding down the button the value increases 


                    }
            }       
            Serial.println("Pump Control Secession Over");
            delay(500);


}





void setup()
{
    // Open serial communications and wait for port to open:

    Serial.begin(9600);
    digitalWrite(10, HIGH);

    Serial.print("Initializing SD card...");

    if (!SD.begin(SD_CHIP_SELECT_PIN))
    {                                             // even though this argument is technically optional, we do need to select chip 4 for this to work!!
        Serial.println("initialization failed!"); // Shows a problem if the SD never acually is reconigized by arduino
        return;
    }
    Serial.println("initialization done."); // Confirms Arduino SD error is not the problem

    // Write adds to end, so remove any preexisting file
    if (SD.exists(filename))
    {                        // If already a file
        SD.remove(filename); // Remove the file to stop weird confusing
    }
    totalDatapointCounter = 0; // This is the total number of datapoints that have been acquired
    start_time = millis();
    halter = 0; // this is a logical variable that tells us whether or not our acquisition is done.  Once file_is_closed is 1, acquisition stops.  I changed the name to better reflect what it's for.
    // Open once, at the beginning
    myFile = SD.open(filename, FILE_WRITE); // Making an iterator which stores an instance to access the file class SHOULD LEARN MORE ABOUT AN INSTANCE
    // since this is inside the setup() function, I indent it for readability
    //
    /*This is where I start the set up I WILL SET THE READINGS VALUES TO ZERO TO BEGIN WITH*/

    VRead = 0;                 // All variables will start a zero and will change as we continue
    NumCount = 0;              // The number counter which helps with making sure no extranous points are written withing the structure ex random 1024 jumps somtimes may happen
    VRec = 0;                  // The actual data point which is averaged
    astore = 0;                // The consectutive reading of the analog will be added here and then divided once the numcount reaches its target
    buttonToggleState = false; // button has not been pressed
    buttonDown = false;        // button actively being pressed
    // Setting the pin modes

    pinMode(A0, INPUT);
    pinMode(A2, INPUT);
    pinMode(slavecom, OUTPUT);

    /// Pump Mechanics
pinMode(7,INPUT_PULLUP);
pinMode(6,INPUT_PULLUP);
pinMode(8,OUTPUT); // is this the LED pin?
inter = 1;
grace = 0;
 


    digitalWrite(slavecom, LOW);
    SPI.transfer(0);
    SPI.transfer(10);
    digitalWrite(slavecom, HIGH);
    // This block turned the led to the right combination

    // The serial begin has already been set so I will no do that
    lastButtonPress = millis();


    //Setting pump speed
    Pump_Control();
    
    lastlog = 0;
   
}

void loop()
{
    
    int j, k; //
    int num_written;
    if(millis()/log_interval > lastlog+1){
        logFile = SD.open("log.txt", FILE_WRITE);
        logFile.seek(EOF);// go to end of file to append
        logFile.println("\neverything is running!\nat millis:");
        logFile.println(millis());
        logFile.println("\nbuttonDown ");
        logFile.println(buttonDown);
        logFile.println("\nfile_is_closed ");
        logFile.println(halter);
        logFile.println("\ntotalDatapointCounter ");
        logFile.println(totalDatapointCounter);
        logFile.println("\nposition within datafile ");
        logFile.println(myFile.position());
        logFile.close();
        lastlog = millis() / log_interval;
    }

    
if (analogRead(A2) > 500) // is the button currently down?
    
    {
        if (!buttonDown && millis() - lastButtonPress > 1000) // b/c we don't want the following if we're continuously holding down the button
        {
            buttonToggleState = buttonToggleState ^ true; // flip state of buttonToggleState
            Serial.println("you pressed the button!");
            Serial.println(millis() - lastButtonPress);
            lastButtonPress = millis();
        }
        buttonDown = true;
    }
    else
    {
        buttonDown = false;
    }
    j = totalDatapointCounter % datalen; // this line does not have to do with fake data -- it's the position of the current datapoint in the "buffer"

    if (buttonToggleState && !halter)
    {

        // The pump module

        //// {{{ Serial logging -- this is your code -- I just moved it inside
        ///      the code block that was intended for detection.  It doesn't
        ///      make sense for this to be in an outer block -- you want the
        ///      actual acquisition to replace the fake data!

        // Now starting the edit to the serial logging

        VRead = analogRead(A0); // VRead is an integer, so it doesn't make sense to cast to a float!
        astore += VRead;
        NumCount += 1;

        if (NumCount == numtarget)
        {
            VRec = (float)astore / (float)numtarget; // Calculating the average of the given data
            // previous converted to a floating point, otherwise you are losing accuracy.
            // this does meant that we will need to change the structure and
            // the file that reads the structure.
            NumCount = 0;
            totalDatapointCounter += 1; // increment this here, since if we're averaging, we only want a new datapoint for every averaged point that we generate
            if (totalDatapointCounter > 50000 && totalDatapointCounter % datalen == 0)
            {
                totalDatapointCounter = 0; // prevent overflow by rolling over
            }
            astore = 0;
            Serial.print("Analog (V) of ");
            Serial.print(VRec);
            Serial.print(" at time: ");
            Serial.print(millis() / 1000); // Taking the millis and dividing by 1000 to record seconds
            Serial.println(" sec");
            // I removed the delay statement here -- not sure what purpose it was serving

            // note that if we stick with averaging, we would only write the
            // averaged points to the binary data, so all the stuff below would
            // be added to this block, here.
            mydata[j].Voltage = VRec; // Attempting to actually log data
            mydata[j].time = millis();
            mydata[j].time_short = micros(); // you are not understanding the purpose of the two times -- leave them
            if (totalDatapointCounter > 0 && j == datalen - 1)
            { // write data only once we've filled up mydata
            
            
        
                for (k = 0; k < datalen; k++)
                {
                    Serial.print("datapoint: ");
                    Serial.print(k);
                    Serial.print(": ");
                    Serial.print(mydata[k].Voltage);
                    Serial.print(",");
                    Serial.println(mydata[k].time);
                }
                num_written = myFile.write((const uint8_t *)mydata, sizeof(mydata)); // Parentheses before variable declares variable
                Serial.println("wrote a chunk");
                Serial.println(num_written); // this checks if the data is being written!!  also, we need to know this number to understand how to read it! report this number
                // JF removed lines to close file here -- closing and reopening is inefficient, and I don't think is going to help much
                // if we want to prioritize data "safety" then it's better to write a series of datafiles
                Serial.println("\nWaiting for Next Data Point Collection Cycle: Hold Button to End Data Collection (Wait Until Teminal (binary data done) is stated");
                Serial.println("If Unable to Reconnect to the Terminal Hold the Button Down for 30 seconds to halt Data Collection");
                digitalWrite(8,HIGH);
                delay(5000);
                digitalWrite(8,LOW);
                // we're not going to worry about data overflowing -- more measurements means more SNR!
            }
        }
    }
    else if (totalDatapointCounter > 0 && !halter)
    { // Single "&" is pointer converter double and is logical "and" operator
        // This means we're done, so go ahead and close the file
        Serial.println("binary data done");
        halter = 1; // Assuing a logical operator
        myFile.close();
    }
}
