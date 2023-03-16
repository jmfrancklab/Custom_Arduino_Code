#include <Arduino.h>
#include <SPI.h>

//The variables 
int pcontrol = 3;
int pdown = 7;
int pup = 6;

int inter = 1;






void setup(){
// Setting the pinmodes
pinMode(pcontrol,OUTPUT);
pinMode(pdown,INPUT_PULLUP);
pinMode(pup,INPUT_PULLUP);

}



void loop(){
//Defining the control values

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

