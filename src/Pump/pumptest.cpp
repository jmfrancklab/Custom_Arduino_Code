#include <Arduino.h>
#include <SPI.h>

//The variables 
int pcontrol = 3;
int pdown = 2;
int pup = 4;

int inter = 0;






void setup(){
// Setting the pinmodes
pinMode(pcontrol,OUTPUT);
pinMode(pdown,INPUT_PULLUP);
pinMode(pup,INPUT_PULLUP);

}



void loop(){
//Defining the control values

 pup = digitalRead(2);
 pdown = digitalRead(4);

if(inter< 255 && pup == LOW){ 
    analogWrite(pcontrol,inter++);
    delay(50);    
if(inter> 0 && pdown == LOW){ 
        analogWrite(pcontrol,inter--);
        delay(50);    
    };

///While holding down the button the value increases 
 }
}
