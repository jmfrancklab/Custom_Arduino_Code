////Made by chat gpt 

#include <Arduino.h>

int buttonPin = A0;  // the pin number of the button
int activator1 = 7;  // the first LED pin number
int activator2 = 8;  // the second LED pin number
int led = 4;
int buttonState = 0;       // variable for storing the button state
bool heater_is_on = false; // flag for indicating whether the LEDs are on or off

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // set the button pin as an input with a pull-up resistor
  pinMode(activator1, OUTPUT);      // set the first LED pin as an output
  pinMode(activator2, OUTPUT);  
  pinMode(led,OUTPUT);    // set the second LED pin as an output

  Serial.begin(9600); // initialize serial communication at 9600 bits per second
}

void loop() {
  buttonState = digitalRead(buttonPin);  // read the button state
  if (buttonState == LOW) {              // if the button is pressed
    Serial.print("Button pressed, state = ");
    Serial.println(buttonState);

    if (!heater_is_on) {                       // and the LEDs are off
      digitalWrite(activator1, HIGH);       // turn on the first LED
      digitalWrite(activator2, HIGH);       // turn on the second LED
      digitalWrite(led,HIGH);
      heater_is_on = true;                     // set the flag to indicate that the LEDs are on
      Serial.println("LEDs turned on");
    } else {                             // otherwise
      digitalWrite(activator1, LOW);        // turn off the first LED
      digitalWrite(activator2, LOW);        // turn off the second LED
      digitalWrite(led,LOW);
      heater_is_on = false;                    // set the flag to indicate that the LEDs are off
      Serial.println("LEDs turned off");
    }

    delay(100);                          // debounce the button with a small delay
  }
}



