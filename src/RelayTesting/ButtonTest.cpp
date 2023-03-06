#include <Arduino.h>


int buttonsenser = A0;








void setup(){
pinMode(A0, INPUT);
pinMode(2,OUTPUT);



}





void loop(){

if(buttonsenser>500){
    digitalWrite(2,HIGH);
    delay(5000);
    digitalWrite(2,LOW);
    

}


}