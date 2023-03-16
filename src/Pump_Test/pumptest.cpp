#include <Arduino.h>
#include <SPI.h>

//The variables 
int pcontrol = 3;
int pdown = 7;
int pup = 6;

int inter = 1;

int grace;




void setup(){
// Setting the pinmodes
pinMode(pcontrol,OUTPUT);
pinMode(pdown,INPUT_PULLUP);
pinMode(pup,INPUT_PULLUP);
grace = 0;
inter = 0;
Serial.begin(9000);

}



void loop(){
//Defining the control values

grace = millis();
Serial.println("\n You may adjust the pump speed now ");
            while(millis()- grace <= 10000){
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
Serial.println("Pump Switch Time Oover wait until next que");
delay(4000);  
}

