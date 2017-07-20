#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

/*
  Button

 Turns on and off a light emitting diode(LED) connected to digital
 pin 13, when pressing a pushbutton attached to pin 2.


 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground

 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.


 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  12;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

IRsend irsend;

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
String inputString = "";
while(Serial.available() > 0){

      //Serial.write("enters while loop\n");
    char inChar = (char)Serial.read();            //read the input
    inputString += inChar;                        //make a string of the characters coming on Serial
 
    if(inputString == "1"){                       //in case of '1' turn the LED on - threshold on
       digitalWrite(ledPin, HIGH);
       for(int i=0; i<3; i++){
       irsend.sendNEC(0xa90,12);
       Serial.println("IR send");
//       Serial.print(irsend);
       delay(40);
       }
       delay(500);
       digitalWrite(ledPin,LOW);  
       
    }
    else if(inputString == "2"){                   //in case of '2' turn the LED on - threshold on
       digitalWrite(ledPin, HIGH);
       delay(500);
       digitalWrite(ledPin,LOW);  
    }
    else if(inputString == "3"){                   //in case of '3' turn the LED on - threshold on
       digitalWrite(ledPin, HIGH);
       delay(500);
       digitalWrite(ledPin,LOW);  
    }
    else if(inputString == "4"){                   //in case of '4' turn the LED on - threshold on
       digitalWrite(ledPin, HIGH);
       delay(500);
       digitalWrite(ledPin,LOW);  
    }
    else if(inputString == "5"){                   //in case of '5' turn the LED on - threshold on
       digitalWrite(ledPin, HIGH);
       delay(500);
       digitalWrite(ledPin,LOW);  
    }
    else if(inputString == "6"){                   //in case of '6' turn the LED on - threshold on
       digitalWrite(ledPin, HIGH);
       delay(500);
       digitalWrite(ledPin,LOW);  
    }
    else {
      digitalWrite(ledPin,LOW);
    }
    
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
//  if (buttonState == HIGH) {
//    // turn LED on:
//    digitalWrite(ledPin, HIGH);
//    Serial.println("LED HIGH");
//  } else {
//    // turn LED off:
//    digitalWrite(ledPin, LOW);
//    Serial.println("LED HIGH");
//  }

}
}
