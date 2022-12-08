#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
//Tried to define the pin select stuck here with initalizing the pin
#define PIN_SPI_CS 4
File myFile;

// change this to match your SD shield or module; possilbe helpful link here https://arduinogetstarted.com/reference/library/arduino-sd.begin



void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  digitalWrite(10,HIGH);
/*SD.begin();
The stuff above did not change the initalization failed! check meanign the code is not connecting with the SD card
*/ 

  Serial.print("Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop()
{
  // nothing happens after setup
}



