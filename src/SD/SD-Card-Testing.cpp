#include <SPI.h>
#include <SD.h>

File myFile;


struct datastore {
  int data1;
  int data2;
  int data3;

};

datastore buffer[10]; // Have created the data store buffer information

char *filename = "hello_.dat";




void setup() {
  // Open serial communications and wait for port to open:

  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");



  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(filename, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.seek(EOF);
    int num = myFile.write((const uint8_t *)buffer, sizeof(buffer));

    Serial.print("Num_written ");
    Serial.println(num);
    Serial.print("Wrote: ");
  Serial.print(num); // To check if data actually written
  Serial.print(" much data\n");
  Serial.print("Is data avalible? ");
  Serial.println(myFile.available());
  Serial.print("The name of the file is: ");
  Serial.println(myFile.name());
  Serial.print("Size of Structure: ");
  Serial.println(sizeof(buffer));
  Serial.print("Position of the file is: ");
  Serial.println(myFile.position());
  Serial.print("The file Exist? ");
  Serial.println(SD.exists(filename));
  Serial.print("The size of the file is now: ");
  Serial.println(myFile.size());
  myFile.close();
   
  
  
  }


}

void loop() {
int i = 0;
for(i; i<10;i++){

  buffer[i].data1 = i*10;
  buffer[i].data2 = i*20;
  buffer[i].data3 = i*30;
}
 myFile = SD.open(filename, FILE_WRITE);  
    int num = myFile.write((const uint8_t *)buffer, sizeof(buffer));
 myFile.seek(EOF);
      Serial.print("Num_written ");
    Serial.println(num);
    Serial.print("Wrote: ");
  Serial.print(num); // To check if data actually written
  Serial.print(" much data\n");
  Serial.print("Is data avalible? ");
  Serial.println(myFile.available());
  Serial.print("The name of the file is: ");
  Serial.println(myFile.name());
  Serial.print("Size of Structure: ");
  Serial.println(sizeof(buffer));
  Serial.print("Position of the file is: ");
  Serial.println(myFile.position());
  Serial.print("The file Exist? ");
  Serial.println(SD.exists(filename));
  Serial.print("The size of the file is now: ");
  Serial.println(myFile.size());
  myFile.close();
  delay(500);
   
}
