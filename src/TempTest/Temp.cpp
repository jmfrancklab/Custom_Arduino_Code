//12-bit default resolution; external power supply
#include<OneWire.h>
#define OneWireBusPin 3
OneWire ds(OneWireBusPin);  //3
byte addr[8];         //to hold 64-bit ROM Codes of DS18B20 sensor

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  ds.reset();         //sensor is reset
  ds.search(addr);    //collect 64-bit (8-byte) ROM/Addres code Sensor (DS1)
  Serial.print("Address of DS18B20: ");
  for (int i = 0; i < 8; i++)
  {
    Serial.print(addr[i], HEX);   //show on SM
  }
  Serial.println();
  delay(2000);
}