//Test_Code

//include statements
#include <SPI.h>

  // potentiometer code

byte address = 0x11;                      //Bits 5 and 4 of command byte are 0 and 1 respectively
                                          //Bits 1 and 0 are 0 and 1 respectively
#define CS 53                             //CS (pin 1) connected to port 53 [determines which device to communicate with]
                                          //SCK (pin 2) connected to port 52 
                                          //SI (pin 3) connected to port 51
//#define PIN_POT_IN A1                     //input value for setting potentiometer

//Sound detector Ports
//Define Hardware connections

#define PIN_GATE_IN 2                       //Sound Detector Gate Input (5 pins or RED)
#define PIN_ANALOG_IN A0                    //Sound Detector Analog Input (enveleope)
#define PIN_LED_BT_G 12                     //Bluetooth Green LED (Resistor) - Threshold
#define PIN_LED_BT_Y 13                     //Bluetooth Yellow LED (Resistor) - Action Occured

                                            //RX & TX Pin 18TX & 19RX (No connection State or EN)
                                            //Audio no connection
                                        
//Constants for VoidLoop

int index = 5;                              //defining index - used in sound detector loop
double Vi = 0.0005;                         // Input voltage for audio 0.5mV
double conv = 0.004887585533;               //SD voltage conversion factor (5/1023)
double BISR = 0;                            //Bluetooth ISR value
double RMS = 0;                             //value of BISR RMS voltage   
double RMS2 = 0;                            //value of value (sum/i) RMS voltage  
double R = 0;                               //digital potnetiometer set (difference of R1 & R2 added to R)
double R1 = 0;                              //initially used to set potentiometer (initial threshold) - gain
double R2 = 0;                              //gain of the inputs (value RMS converted)

  //bluetooth code start 

double BR = 0;                              //value of digital potnetiometer in BluetoothISR volume up (new value)
double BR1 = 0;                             //value of digital potnetiometer in BluetoothISR before volume up/down
double BRmute = 0;                          //value of digital potnetiometer in BluetoothISR mute on/off
double i3 = 0;
double global = 40;
int volchange = 5;

//TX input and RX output (Grey)
//RX input and TX output (Black)

//0 (bluetooth connected)
//1 (to turn the LED on - threshold on)
//2 (tor turn the LED off - threshold off)
//3 (volume up)
//4 (volume down)
//5 (mute on)
//6 (mute off)

double BluetoothISR(double RMSsum, int i2){
String inputString = "";
    
  while(Serial.available() > 0){
    
    //Serial.write("enters while loop\n");
    char inChar = (char)Serial.read();       //read the input
    inputString += inChar;                    //make a string of the characters coming on Serial
 
    if(inputString == "1"){                   //in case of '1' turn the LED on - threshold on
       digitalWrite(PIN_LED_BT_G, HIGH);  
       return (RMSsum/i2);    
    }
    
    else if(inputString == "2"){              //in case of '2' turn the LED off - threshold off
     digitalWrite(PIN_LED_BT_G, LOW);
     return 0;
    }  
    
    else if(inputString == "3"){              //in case of '3' turn the LED off - volume up
     digitalWrite(PIN_LED_BT_Y, HIGH);
     delay(500);
     digitalWrite(PIN_LED_BT_Y, LOW);
     delay(10);
     if ((global + volchange) <= 85){
      BR = (global + volchange);
      digitalPotWrite(BR);
      Serial.print("BR: ");
      Serial.println(BR);
      global = BR;
     }
     BR1 = ((global + volchange)*(Vi/conv));
     Serial.print("BR1: ");
     Serial.println(BR1);
     return BR1;  
    }
     
    else if(inputString == "4"){            //in case of '4' turn the LED off - volume down
     digitalWrite(PIN_LED_BT_Y, HIGH);
     delay(500);
     digitalWrite(PIN_LED_BT_Y, LOW);
     delay(10);
      if ((global - volchange) >= 10){
       BR = abs(global - volchange);
       digitalPotWrite(BR);
       Serial.print("BR: ");
       Serial.println(BR);
       global = BR;
      }
     BR1 = ((global - volchange)*(Vi/conv));
     Serial.print("BR1: ");
     Serial.println(BR1);
     return BR1;
    }
    
     else if(inputString == "5"){             //in case of '5' turn the LED off - mute on
      digitalWrite(PIN_LED_BT_Y, HIGH);
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW); 
      BRmute = (RMSsum/i2);
      delay(10);
          for (i3 = BRmute; i3 >= 0; i3--){
           digitalPotWrite(i3);
           delay(15);
          }
      return 0; 
     }

     else if(inputString == "6"){              //in case of '6' turn the LED off - mute off
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW); 
      for (i3 = 0; i3 <= global; i3++){
       digitalPotWrite(i3);
       delay(15);
      }
      BRmute = (BRmute *(Vi/conv));
      delay(5);
      return BRmute;
     }
        
     else if(inputString == "samsung"){      //in case of TV Brand
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
     }
     
     else if(inputString == "LG"){           //in case of TV Brand
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
     }

     else if(inputString == "Sony"){         //in case of TV Brand
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
     }
     
     else{ 
      return 0;
     }
     
    Serial.print("Bluetooth:");
    Serial.println(inputString);
    inputString = "";
  }
}
  //bluetooth code end

  //potentiometer code begin

int digitalPotWrite(int value)
{
digitalWrite(CS, LOW);                        //Make CS active LOW
SPI.transfer(address);                        //call the address you are writing too
SPI.transfer(value);                          //transfer the value to the wiper terminal
digitalWrite(CS, HIGH);                       //Deselect the chip
}

  //potentiometer code end

void setup(){                                 // run once, when the sketch starts
 
 Serial.begin(9600);                          // set the baud rate to 9600, same should be of your Serial Monitor
 //Serial1.begin(9600);                         // set the rate to 9600 for Serial (bluetooth)
 
//Potentiometer setup
   pinMode(CS, OUTPUT);                       //Make CS an output
   SPI.begin();
   
//Led output for bluetooth
   pinMode(PIN_LED_BT_G, OUTPUT); 
   pinMode(PIN_LED_BT_Y, OUTPUT);      
  
// Display status of Sound Detector
   Serial.println("Initialized");
   
    //digitalPotWrite(40);                      //initally set potentiometer on startup
}
 
void loop(){
  //main code start
  
  double value;
  int i;
  double sum = 0;
  double RMSsum = 0;
  double avg = 0;
  double Values[index];
  double BISR2 = BISR + (BISR*0.10);                     
  double BISR3 = BISR - (BISR*0.10);
    
  for (i = 1; i <= (index); i++) {
    value = analogRead(PIN_ANALOG_IN);              // Check the analog input
    Serial.print("Status: ");                       // print analog value
    Serial.println(value);
    delay(1000);
                 
    Values[i] = pow(value, 2);                      //write new value into array after shifted
    sum += Values[i];                               //sum the values of the array 
    RMSsum = ((sqrt(sum))/2);                       //concverting sum to RMS value
    Serial.print("RMS Sum: ");
    Serial.println(RMSsum); 

      if (((BISR >= 1) && ((RMSsum/i) > BISR2)) || ((BISR >= 1) && ((RMSsum/i) < BISR3))){
      
      if (R1 == 0) {
        R1 = BISR*(conv/Vi);                         //Vo/Vi = Rwb/Rwa (gain)
        global = R1;
        digitalPotWrite(R1);
        Serial.print("          Wiper Value (R1): ");    //Value of wiper outputted - set to initial threshold
        Serial.println(R1);
        }
      
      else {
        //Serial.print("            OLD BISR:");
       // Serial.println(BISR);
        R1 = BISR*(conv/Vi);                          //Vo/Vi = Rwb/Rwa (gain) Threshold
        Serial.print("R1: ");
        Serial.println(R1);
        RMS2 = RMSsum/i;    
        R2 = RMS2*(conv/Vi);                          //Current Sum
        Serial.print("R2: ");
        Serial.println(R2);
        
        if ((R1 < R2) && ((R1 - (R2 - R1)) >= 10)) {
          R = abs(R1 - (R2 - R1));
          BISR = R*(Vi/conv);
        //  Serial.print("            NEW BISR: ");
        // Serial.println(BISR);
          global = R;
          digitalPotWrite(R);                          //write the potentiometer to the set threshold
          Serial.print("        Decreased Wiper Value: ");  // Value of wiper outputted
          Serial.println(R);
        }

        else if ((R1 > R2) && ((R1 + (R1 - R2)) <= 88)) {
          R = abs(R1 + (R1 - R2));
          BISR = R*(Vi/conv);
        //  Serial.print("           NEW BISR: ");
        //  Serial.println(BISR);
          global = R;
          digitalPotWrite(R);                           //write the potentiometer to the set threshold  
          Serial.print("         Inceased Wiper Value: ");  // Value of wiper outputted
          Serial.println(R);
       }
     }
   }
 }
    avg = RMSsum/index;                                 //average the values of the array
    
    Serial.print("Converted Average BISR: ");
    Serial.println(BISR*(conv/Vi));
    Serial.print("Average BISR: ");
    Serial.println(BISR);
    
    Serial.print("Average: ");
    Serial.println(((sqrt(avg)/2)*conv)/Vi);
    delay(1000);
}
  //main code end
