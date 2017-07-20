#include <boarddefs.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <IRremote.h>
#include <IRremoteInt.h>


// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
IRsend irsend;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status


void setup() {
  
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  //Serial1.begin(9600);               // set the rate to 9600 for Serial1 (bluetooth)
    
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  //buttonState = digitalRead(buttonPin);

  //Serial.write("enters bluetooth works function\n");
    char junk;
    String inputString = "";
    
   while(Serial.available() > 0){
    
    //Serial.write("enters while loop\n");
      char inChar = (char)Serial.read(); //read the input
      inputString += inChar;            //make a string of the characters coming on Serial
 
    if (buttonState == HIGH){           //in case of '1' turn the LED on - threshold on
     for (int i = 0; i < 9; i++){
      irsend.sendSony(0xE0E0E01F, 32); // SAMSUNG TV Volume Up
      //digitalWrite(ledPin, HIGH);
      delay(100);
        }   
     }
      if(inputString == "4"){           //in case of '1' turn the LED on - threshold on
        for (int i = 0; i < 9; i++){
      irsend.sendSony(0xE0E0E02F, 32); // SAMSUNG TV Volume Down
      delay(100);
        }   
     }
   }
}
