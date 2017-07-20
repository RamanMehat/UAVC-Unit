#include <Adafruit_ADS1015.h>

//Test_Code_ADC

//include statements
#include <SPI.h>
#include <Wire.h>



// potentiometer code

byte address = 0x11;                      //Bits 5 and 4 of command byte are 0 and 1 respectively
                                          //Bits 1 and 0 are 0 and 1 respectively
#define CS 53                             //CS (pin 1) connected to port 53 [determines which device to communicate with]
                                          //SCK (pin 2) connected to port 52 
                                          //SI (pin 3) connected to port 51
#define PIN_POT_IN A1                     //input value for setting potentiometer

//Define Hardware connections

#define PIN_LED_BT_G 12                     //Bluetooth Green LED (Resistor)
#define PIN_LED_BT_Y 13                     //Bluetooth Yellow LED (Resistor)

                                            //RX &TX Pin 18TX & 19RX (No connection State or EN)
                                            //Audio no connection
//ADC Pins 
                                            //SCL connect to SCL pin (first one on the end (1)), SDA connect to SDA pin (second one in (2))
                                            //A0-A3 different inputs
Adafruit_ADS1115 ads(0x48);                 //16 bit adc for ADDR -> GND is 0x48                                      
                                        
//Constants for VoidLoop

int index = 5;                              //defining index - used in sound detector loop
double Vi = 0.0005;                         // Input voltage for audio 0.5mV
double conv = 51.2;                         //ADC voltage conversion factor (256/5)
double BISR = 0;                            //Bluetooth ISR value
double RMS = 0;                             //value of BISR RMS voltage   
double RMS2 = 0;                            //value of value (sum/i) RMS voltage  
double R = 0;                               //digital potnetiometer set (difference of R1 & R2 added to R)
double R1 = 0;                              //initially used to set potentiometer (initial threshold) - gain
double R2 = 0;                              //gain of the inputs (value RMS converted)
double Voltage_ADC = 0;                     //initialize voltage value for ADC
double Gain_Const = 0.00015258789 ;         // constant for ADC gain (10/2^16) 

//bluetooth code start 

double BR = 0;                              //value of digital potnetiometer in BluetoothISR volume up (new value)
double BR1 = 0;                             //value of digital potnetiometer in BluetoothISR before volume up/down
double BRmute = 0;                          //value of digital potnetiometer in BluetoothISR mute on/off
double i3 = 0;                              //Bluetooth code increment
double global = 50;                         //global variable for potentiometer
double global2 = 0;                         //setting global to constant before change
int volchange = 5;                          //increment for potentiometer for vol up/down in wiper values
double volchangeConst = 0.09765625;         //increment for potentiometer for vol up/down in voltage

//TX input and RX output (Grey)
//RX input and TX output (Black)

//0 (bluetooth connected)
//1 (to turn the LED on - threshold on)
//2 (tor turn the LED off - threshold off)
//3 (volume up)
//4 (volume down)
//5 (mute on)
//6 (mute off)

  //potentiometer code begin

int digitalPotWrite(int value)
{
digitalWrite(CS, LOW);                        //Make CS active LOW
SPI.transfer(address);                        //call the address you are writing too
SPI.transfer(value);                          //transfer the value to the wiper terminal
digitalWrite(CS, HIGH);                       //Deselect the chip
}

  //potentiometer code end
  
double BluetoothISR(double RMSsum, int i2){
String inputString = "";
    
  while(Serial1.available() > 0){
    
    //Serial.write("enters while loop\n");
    char inChar = (char)Serial1.read();       //read the input
    inputString += inChar;                    //make a string of the characters coming on Serial
    Serial.println(inputString); 
    
    if(inputString == "1" ){                   //in case of '1' turn the LED on - threshold on
       digitalWrite(PIN_LED_BT_G, HIGH); 
       Serial.println("                                     threshold on                                                     ");
       return (RMSsum/i2);    
    }
    
    else if(inputString == "2" ){              //in case of '2' turn the LED off - threshold off
     digitalWrite(PIN_LED_BT_G, LOW);
     Serial.println("                                     threshold off                                                     "); 
     return 0;
    }  
    
    else if(inputString == "3" ){              //in case of '3' turn the LED off - volume up
     digitalWrite(PIN_LED_BT_Y, HIGH);
     delay(500);
     digitalWrite(PIN_LED_BT_Y, LOW);
     delay(10);
     if ((global + volchange) <= 90){
      global2 = global;
      BR = (global + volchange);
      digitalPotWrite(BR);
      Serial.print("BR: ");
      Serial.println(BR);
      global = BR;
      R1 = global;
      Serial.print("R1:                                                         ");
      Serial.println(R1);
      Serial.println("                                     volume up                                                     "); 
     }
     BR1 = (global2/conv) + volchangeConst;
     Serial.print("BR1: ");
     Serial.println(BR1);
     return BR1;  
    }
     
    else if(inputString == "4" ){            //in case of '4' turn the LED off - volume down
     digitalWrite(PIN_LED_BT_Y, HIGH);
     delay(500);
     digitalWrite(PIN_LED_BT_Y, LOW);
     delay(10);
     if ((global - volchange) >= 10){
      global2 = global;
      BR = abs(global - volchange);
      digitalPotWrite(BR);
      Serial.print("BR: ");
      Serial.println(BR);
      global = BR;
      R1 = global;
      Serial.print("R1:                                                         ");
      Serial.println(R1);
      Serial.println("                                     volume down                                                     "); 
     }
     BR1 = (global2/conv) - volchangeConst;
     Serial.print("BR1: ");
     Serial.println(BR1);
     return BR1;
    }
    
     else if(inputString == "5" ){             //in case of '5' turn the LED off - mute on
      digitalWrite(PIN_LED_BT_Y, HIGH);
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW); 
      BRmute = global;
      delay(10);
      for (i3 = BRmute; i3 >= 0; i3--){
       digitalPotWrite(i3);
       delay(15);
      }
      Serial.println("                                     mute on                                                     ");
      return 0; 
     }

     else if(inputString == "6" ){              //in case of '6' turn the LED off - mute off
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW); 
      for (i3 = 0; i3 <= global; i3++){
      digitalPotWrite(i3);
       delay(15);
      }
      BRmute = (BRmute/conv);
      delay(5);
      Serial.println("                                     mute off                                                     "); 
      return BRmute;
     }
        
     else if(inputString == "samsung"){      //in case of TV Brand
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
     }
     
     else if(inputString == "lg"){           //in case of TV Brand
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
     }

     else if(inputString == "sony"){         //in case of TV Brand
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
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

void setup(){                                 // run once, when the sketch starts
 
 Serial.begin(9600);                          // set the baud rate to 9600, same should be of your Serial Monitor
 Serial1.begin(9600);                         // set the rate to 9600 for Serial1 (bluetooth)
 ads.begin();                                 // ADC 
 //ads.setGain(GAIN_SIXTEEN);
     
//Potentiometer setup
   pinMode(CS, OUTPUT);                       //Make CS an output
   SPI.begin();
   
//Led output for bluetooth
   pinMode(PIN_LED_BT_G, OUTPUT); 
   pinMode(PIN_LED_BT_Y, OUTPUT);      
  
// Display status of Sound Detector
   Serial.println("Initialized");
   
   digitalPotWrite(50);                      //initally set potentiometer on startup
}
 
void loop(){
  //main code start
  
  double value;
  int i;
  double i4;
  double sum = 0;
  double RMSsum = 0;
  double avg = 0;
  double Values[index];
  double BISR2 = BISR + (BISR*0.10);                     
  double BISR3 = BISR - (BISR*0.10);
  int16_t adc0;                                       // returns 16 bit adc values for pin A0
  int16_t adc1;                                       // returns 16 bit adc values for pin A1

      
  for (i = 1; i <= (index); i++) {
    adc0 = ads.readADC_SingleEnded(0);                //reads single ended (ref to ground) input from pin A0
    value = abs(adc0*Gain_Const);                      //absolute analog from digital voltage value (V) 
    Serial.print("ADC value: ");                         // print analog value
    Serial.println(value, 7);
    delay(10);
    double valuenew = adc0*0.0001875;
    Serial.print("                       new value           ");
    Serial.println(valuenew,7);
                 
    Values[i] = pow(value, 2);                        //write new value into array after shifted
    sum += Values[i];                                 //sum the values of the array 
    RMSsum = (sqrt(sum))/2;                           //concverting sum to RMS value
    Serial.print("RMS Sum: ");
    Serial.println(RMSsum, 7); 

    BISR = BluetoothISR(RMSsum, i);
    Serial.print("Initial BISR: ");
    Serial.println(BISR);
    

      if (((BISR >= 0.000001) && ((RMSsum/i) > BISR2)) || ((BISR >= 0.000001) && ((RMSsum/i) < BISR3))){
      Serial.println("______________________________________________________________________________");
      
      if (R1 == 0) { 
        R1 = BISR*conv;                            //Vo/Vi = Rwb/Rwa (gain)
        global = R1;
        digitalPotWrite(R1);
        Serial.print("          Wiper Value (R1): ");    //Value of wiper outputted - set to initial threshold
        Serial.println(R1);
        }
      
      else {
        Serial.print("R1: ");
        Serial.println(R1);
        RMS2 = RMSsum/i;
        Serial.print("RMS2: ");
        Serial.println(RMS2, 7);   
        R2 = RMS2*conv;//*1000;                              //Current Sum
        Serial.print("R2: ");
        Serial.println(R2, 7);

        if ((R1 < R2) && ((R1 - (R2 - R1)) >= 12)) {
          R = abs(R1 - (R2 - R1));
          for (i4 = R1; i4 >= R; i4--){
          digitalPotWrite(i4);                               //write the potentiometer to the set threshold
          delay(10);
          Serial.print("-----------------Decreased Wiper Value: ");  // Value of wiper outputted
          Serial.println(i4);
          }
          R = i4;
          R1 = R;
          global = R;
          Serial.print("-----------------Decreased Wiper Value 2: ");  // Value of wiper outputted
          Serial.println(R);
        }

        else if ((R1 > R2) && ((R1 + (R1 - R2)) <= 100)) {
          R = abs(R1 + (R1 - R2));
          for (i4 = R1; i4 <= R; i4++){
           digitalPotWrite(i4);                               //write the potentiometer to the set threshold
           delay(10);
           Serial.print("++++++++++++++++++++Inceased Wiper Value: ");           // Value of wiper outputted
           Serial.println(i4);
          }
          R = i4;
          R1 = R;
          global = R;
          Serial.print("++++++++++++++++++++Inceased Wiper Value 2: ");           // Value of wiper outputted
          Serial.println(R);
       }

        if ((R1 <= 10) && ((R1 + (R1 - R2)) <= 100)){
           R = abs(R1 + (R1 - R2));
          for (i4 = R1; i4 <= global; i4++){
           digitalPotWrite(i4);                               //write the potentiometer to the set threshold
           delay(10);
           Serial.print("*******************Inceased Wiper Value: ");           // Value of wiper outputted
           Serial.println(i4);
          }
          R = i4;
          R1 = R;
          global = R;
          Serial.print("********************Inceased Wiper Value 2: ");         // Value of wiper outputted
          Serial.println(R);
        }
     }
   }
 }
    avg = RMSsum/index;                            //average the values of the array

    Serial.print("                Original Average BISR:                     ");
    Serial.println(BISR);
    
    Serial.print("                Converted Average BISR:                    ");
    Serial.println(BISR*conv);
    
    Serial.print("                      Average:                             ");
    Serial.println((avg*conv), 7);
    delay(10);
}
  //main code end
