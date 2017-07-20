//Test_Code

//Sound detector Ports


//Define Hardware connections
#define PIN_GATE_IN 2         //Sound detector Gate
#define PIN_LED_SD 3          //Sound Detector LED (Resistor 1k)
#define PIN_ANALOG_IN A0      //Sound Detector Envelope
#define PIN_LED_BT 13         //Bluetooth LED (Resistor)

                               //RX &TX Pin 18TX & 19RX (No connection State or EN)
                               // Audio no connection
int index = 100;   //defining index - used in sound detector loop
double average;

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
        digitalWrite(PIN_LED_BT, HIGH);
        return average = (sum/i2);    
    }
    
    else if(inputString == "2"){        //in case of '2' turn the LED off - threshold off
      digitalWrite(PIN_LED_BT, LOW);
      return 0;
    }  
    
     else if(inputString == "3"){        //in case of '3' turn the LED off - volume up
      digitalWrite(PIN_LED_BT, HIGH);
      delay(500);
      digitalWrite(PIN_LED_BT, LOW);  
     }
     
     else if(inputString == "4"){        //in case of '4' turn the LED off - volume down
      digitalWrite(PIN_LED_BT, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT, LOW); 
        }

      else if(inputString == "5"){        //in case of '5' turn the LED off - mute on
      digitalWrite(PIN_LED_BT, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT, LOW); 
        }

      else if(inputString == "6"){        //in case of '6' turn the LED off - mute off
      digitalWrite(PIN_LED_BT, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT, LOW); 
        }
        
     else if(inputString == "samsung"){        //in case of TV Brand
      digitalWrite(PIN_LED_BT, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT, LOW);
     }
     
     else if(inputString == "LG"){            //in case of TV Brand
      digitalWrite(PIN_LED_BT, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT, LOW);
     }

     else if(inputString == "Sony"){          //in case of TV Brand
      digitalWrite(PIN_LED_BT, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT, LOW);
     }
     
    Serial1.print("Bluetooth:");
    Serial1.println(inputString);
    inputString = "";
  }

}
//bluetooth code end

void setup(){                       // run once, when the sketch starts
 Serial.begin(9600);                // set the baud rate to 9600, same should be of your Serial Monitor
 Serial1.begin(9600);               // set the rate to 9600 for Serial1 (bluetooth)
 
 pinMode(PIN_LED_BT, OUTPUT);       //Led output for bluetooth 
  
 //Sound detector setup code begin
 
   //  Configure LED pin as output
   pinMode(PIN_LED_SD, OUTPUT);
  
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
  double BISR;

  for (i = 1; i <= (index); i++) {
    value = analogRead(PIN_ANALOG_IN);  // Check the envelope input
    Serial.print("Status:");           // Convert envelope value into a message
    Serial.println(value);
    delay(50);

    Values[i + 1] = Values[i];          //write array value into the array
    Values[i] = value;                  //write new value into array after shifted
    sum += Values[i];                   //sum the values of the array
    Serial.print("Sum:");
    Serial.println(sum); 
    
    BISR = BluetoothISR(sum, i);
    Serial.print("print BISR:");
    Serial.println(BISR);
 }
    avg = sum/index;                   //average the values of the array
    
    Serial.print("Average BISR:");
    Serial.println(BISR);
    
    Serial.print("Average:");
    Serial.println(avg);
    delay(50);
}
 
//sound detector code end
