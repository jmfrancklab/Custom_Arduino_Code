//Include libraries
#include <OneWire.h>
#include <DallasTemperature.h>


//This code sets the sensors and congifures them
#define DS18B20 8

//The activators for our relay and the heating time
int Activator1 = 4;
int Activator2 = 6;
int Heatime = 10000;
double tolorance =10;
double temptarget = 30.45;
bool heater_on = false;

//PD Controls

double Kp = 7;
double Kd = 7;

//Configurating the Temperature sensing
OneWire bus(DS18B20);
DallasTemperature sensors(&bus);

double gettemp(void){

  sensors.requestTemperatures();  
  double T1 = sensors.getTempCByIndex(0);
  double T2 = sensors.getTempCByIndex(1);
  double Tf = (T1+T2)/2;
  return Tf;
//Taking 2 temperature reading and then averaging them
}

double Proportional(){

  double ontimeProp = (Heatime/2) + ((Kp*(gettemp() - temptarget))/temptarget)*(Heatime/2);
  // This calculates the residual of the amount of time it will stay on for one time vs off 

  return ontimeProp;

}

double derivitive(){

  double timei = millis();
  double Ti = gettemp();
  delay(50);
  double Tf = gettemp();
  double timef = millis();
  double ontimeDer = (Heatime/2) + ((Kd*(Tf-Ti)/(timef-timei))*(Heatime/2)); // The derivitive Term

  return ontimeDer;

}

void preheat(void){
  heater_on = true;

if(heater_on = true){
digitalWrite(Activator1,HIGH);
digitalWrite(Activator2,HIGH);
}

while( gettemp()< temptarget){
heater_on = true;
}
heater_on = false;

if(!heater_on){
digitalWrite(Activator1,LOW);
digitalWrite(Activator2,LOW);
}
}


void setup()
{
  //Begining necessary packages
  Serial.begin(9600); 
  sensors.begin();
// Making Pinmodes
  pinMode(Activator1, OUTPUT);
  pinMode(Activator2,OUTPUT);
  preheat();


}








void loop()
{ 

}


