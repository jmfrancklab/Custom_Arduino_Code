#include <SPI.h>
#include <SD.h>

File myFile;


struct datastore {
  int data1;
  int data2;
  int data3;

};

datastore buffer[10]; // Have created the data store buffer information






void setup() {
  // Open serial communications and wait for port to open:
  int i = 0;
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

char *filename = "text.dat";
for(i; i<10;i++){

  buffer[i].data1 = i*10;
  buffer[i].data2 = i*20;
  buffer[i].data3 = i*30;
}
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(filename, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    int num = myFile.write((const uint8_t *)buffer, sizeof(buffer));

    Serial.print("Num_written ");
    Serial.println(num);
    // close the file:
    myFile.close();
  
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.dat");
  if (myFile) {
    Serial.println("test.dat:");

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

void loop() {
  // nothing happens after setup
}
