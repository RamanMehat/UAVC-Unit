//Test_Code
//include statements
#include <SPI.h>

// potentiometer code
byte address = 0x11; //Bits 5 and 4 of command byte are 0 and 1 respectively
                     //Bits 1 and 0 are 0 and 1 respectively
#define CS 53       //CS (pin 1) connected to port 53 [determines which device to communicate with]
                    //SCK (pin 2) connected to port 52 
                    //SI (pin 3) connected to port 51


//Sound detector Ports
//Define Hardware connections
#define PIN_GATE_IN 2           //Sound detector Gate
//#define PIN_LED_SD 3          //Sound Detector LED (Resistor 1k)
#define PIN_ANALOG_IN A0        //Sound Detector Envelope
#define PIN_LED_BT_G 12         //Bluetooth Green LED (Resistor)
#define PIN_LED_BT_Y 13         //Bluetooth Yellow LED (Resistor)

                               //RX &TX Pin 18TX & 19RX (No connection State or EN)
                               // Audio no connection
//Values for VoidLoop
int index = 5;                  //defining index - used in sound detector loop

double Vi = 0.0005;           // Input voltage for audio 0.5mV
double conv = 0.004887585533; //SD voltage conversion factor 
double BISR = 44;
double RMS = 0;
double RMS2 = 0;
double R = 0;
double R1 = 0;
double R2 = 0;



//bluetooth code start 

//0 (bluetooth connected)
//1 (to turn the LED on - threshold on)
//2 (tor turn the LED off - threshold off)
//3 (volume up)
//4 (volume down)
//5 (mute on)
//6 (mute off)

double BluetoothISR(double sum, int i2){

//Serial.write("enters bluetooth works function\n");
char junk;
String inputString = "";
    
  while(Serial1.available() > 0){
    
    //Serial.write("enters while loop\n");
      char inChar = (char)Serial1.read(); //read the input
      inputString += inChar;            //make a string of the characters coming on Serial
 
      if(inputString == "1"){           //in case of '1' turn the LED on - threshold on
        digitalWrite(PIN_LED_BT_G, HIGH);
        return (sum/i2);    
    }
    
    else if(inputString == "2"){        //in case of '2' turn the LED off - threshold off
      digitalWrite(PIN_LED_BT_G, LOW);
      return 0;
    }  
    
     else if(inputString == "3"){        //in case of '3' turn the LED off - volume up
      digitalWrite(PIN_LED_BT_Y, HIGH);
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
      return (sum/i2);  
     }
     
     else if(inputString == "4"){        //in case of '4' turn the LED off - volume down
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
      return (sum/i2);
        }

      else if(inputString == "5"){        //in case of '5' turn the LED off - mute on
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      digitalPotWrite(0); 
      return 0;
        }

      else if(inputString == "6"){        //in case of '6' turn the LED off - mute off
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW); 
      return 0;
        }
        
     else if(inputString == "samsung"){        //in case of TV Brand
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
      return 0;
     }
     
     else if(inputString == "LG"){            //in case of TV Brand
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
      return 0;
     }

     else if(inputString == "Sony"){          //in case of TV Brand
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
      return 0;
     }
     
     else{ 
     return 0;
     }
     
    Serial1.print("Bluetooth:");
    Serial1.println(inputString);
    inputString = "";
  }
}
//bluetooth code end

//Potentiometer start code begin

int digitalPotWrite(int value)
{
digitalWrite(CS, LOW); //Make CS active LOW
SPI.transfer(address); //call the address you are writing too
SPI.transfer(value);   //transfer the value to the wiper terminal
digitalWrite(CS, HIGH); //Deselect the chip
}

//Potentiometer start code end

void setup(){                       // run once, when the sketch starts
 Serial.begin(9600);                // set the baud rate to 9600, same should be of your Serial Monitor
 Serial1.begin(9600);               // set the rate to 9600 for Serial1 (bluetooth)
 
 //Potentiometer setup
   pinMode (CS, OUTPUT); //Make CS an output
   SPI.begin();
   
 //Led output for bluetooth
   pinMode(PIN_LED_BT_G, OUTPUT); 
   pinMode (PIN_LED_BT_Y, OUTPUT);      
  
 //Sound detector setup code begin
   //Configure LED pin as output
  // pinMode(PIN_LED_SD, OUTPUT);
  
   // Display status
   Serial.println("Initialized");

// sound detector setup code end
}

void loop(){
  //sound detector code start
  
  double value;
  int i;
  double sum = 0;
  double avg = 0;
  double Values[index];
  double BISR2 = BISR + (BISR*0.10);
    
  for (i = 1; i <= (index); i++) {
    value = analogRead(PIN_ANALOG_IN);  // Check the envelope input
    Serial.print("Status: ");           // Convert envelope value into a message
    Serial.println(value);
    delay(50);

    Values[i + 1] = Values[i];          //write array value into the array
    Values[i] = value;                  //write new value into array after shifted
    sum += Values[i];                   //sum the values of the array
    Serial.print("Sum: ");
    Serial.println(sum); 

    BISR = BluetoothISR(sum, i);
    Serial.print("print BISR: ");
    Serial.println(BISR);
    
    if ((BISR >= 1) && (value != BISR)){

        RMS = sqrt((BISR))/2;               //converting to RMS voltage (threshold)
        Serial.print("RMS: ");
        Serial.println(RMS);
      
      if (R1 == 0) {
        R1 = (conv*RMS)/Vi;                       // Vo/Vi = Rwb/Rwa (gain)
        digitalPotWrite(R1);
        Serial.print("Wiper Value (R1): ");       // Value of wiper outputted
        Serial.println(R1);
        }
      
      else {
        R1 = (conv*RMS)/Vi;                       // Vo/Vi = Rwb/Rwa (gain)
        Serial.print("R1: ");       // Value of wiper outputted
        Serial.println(R1);
        RMS2 = sqrt((sum/i))/2;
        Serial.print("RMS2: ");
        Serial.println(RMS2);     
        R2 = (conv*RMS2)/Vi;
        Serial.print("R2: ");
        Serial.println(R2);
      
        if ( (R1 < R2) && (R > 10)) {
          R = R1 - abs(R1 - R2);
          digitalPotWrite(R);                 //write the potentiometer to the set threshold
          Serial.print("Wiper Value: ");       // Value of wiper outputted
          Serial.println(R);
        }

        else if ( (R1 > R2) && (R < 85)) {
          R = R1 + abs(R1 + R2);
          digitalPotWrite(R);                 //write the potentiometer to the set threshold
          Serial.print("Wiper Value: ");       // Value of wiper outputted
          Serial.println(R);
       }
      }
    }
  }
    avg = sum/index;                   //average the values of the array
    
    Serial.print("Average BISR: ");
    Serial.println(BISR);
    
    Serial.print("Average: ");
    Serial.println(avg);
    delay(50);
}
 
//sound detector code end
