//Test_Code_SoundDetector_Final

//include statements
#include <SPI.h>

  // potentiometer code

byte address = 0x11;                        //Bits 5 and 4 of command byte are 0 and 1 respectively
                                            //Bits 1 and 0 are 0 and 1 respectively
#define CS 53                               //CS (pin 1) connected to port 53 [determines which device to communicate with]
                                            //SCK (pin 2) connected to port 52 
                                            //SI (pin 3) connected to port 51
//Define Hardware connections for Sound Detector

#define PIN_GATE_IN 2                       //Sound Detector Gate Input (5 pins or RED)
#define PIN_ANALOG_IN A0                    //Sound Detector Analog Input (enveleope)

//Define Hardware connections for Bluetooth
#define PIN_LED_BT_G 12                     //Bluetooth Green LED (Resistor)
#define PIN_LED_BT_Y 13                     //Bluetooth Yellow LED (Resistor)

                                            //RX &TX Pin 18TX & 19RX (No connection State or EN)
                                            //Audio no connection
                     
//Constants for VoidLoop
int index = 50;                             //defining index - used in sound detector loop
double Vi = 0.0005;                         // Input voltage for audio 0.5mV
double conv = 0.004887585533;               //SD voltage conversion factor (5/1023)
double PlusMinus = 0.12;                    //constant vlaue in the BISR2 and BISR3 (maybe changed in theatremode)
double BISR = 0;                            //Bluetooth ISR value
double BISR2 = BISR + (BISR*PlusMinus);     //BISR2 is the upper threshold before changing (entering if statement)               
double BISR3 = BISR - (BISR*PlusMinus);     //BISR3 is the upper threshold before changing (entering if statement)
double RMS = 0;                             //value of BISR RMS voltage   
double R = 0;                               //digital potnetiometer set (difference of R1 & R2 added to R)
double R1 = 0;                              //initially used to set potentiometer (initial threshold) - gain

//Mux control pins input
int s0 = 11;
int s1 = 10;
int s2 = 9;
int s3 = 8;
int controlPin[] = {s0, s1, s2, s3};

//Mux control pins output
int s00 = 7;
int s11 = 6;
int s22 = 5;
int s33 = 4;
int controlPin2[] = {s00, s11, s22, s33};

//Mux in "SIG" pin
int SIG_pin_in = 1;                         //connected to A0 on ADC
int SIG_pin_out = 1;                        //connected to A2 ON ADC

  //potentiometer code begin

int digitalPotWrite(int value)
{
digitalWrite(CS, LOW);                        //Make CS active LOW
SPI.transfer(address);                        //call the address you are writing too
SPI.transfer(value);                          //transfer the value to the wiper terminal
digitalWrite(CS, HIGH);                       //Deselect the chip
}

  //potentiometer code end

  //bluetooth code start 

double BR = 0;                              //value of digital potnetiometer in BluetoothISR volume up (new value)
double BR1 = 0;                             //value of digital potnetiometer in BluetoothISR before volume up/down
double BRmute = 0;                          //value of digital potnetiometer in BluetoothISR mute on/off
double i2 = 0;
double global = 35;
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
  
double BluetoothISR(double Value){
String inputString = "";
    
  while(Serial1.available() > 0){
    
    //Serial.write("enters while loop\n");
    char inChar = (char)Serial1.read();       //read the input
    inputString += inChar;                    //make a string of the characters coming on Serial
    Serial.println(inputString); 
    
    if(inputString == "1" ){                   //in case of '1' turn the LED on - threshold on
       digitalWrite(PIN_LED_BT_G, HIGH); 
       Serial.println("                                     threshold on                                                     ");
       global = Value*(conv/Vi);
       Serial.println(global);
       if (R1 != 0) {
        R1 = global;
       }
       return Value;    
    }
    
    else if(inputString == "2" ){              //in case of '2' turn the LED off - threshold off
     digitalWrite(PIN_LED_BT_G, LOW);
     Serial.println("                                     threshold off                                                     "); 
     R1 = 0;
     return 0;
    }  
    
    else if(inputString == "3" ){              //in case of '3' turn the LED off - volume up
     digitalWrite(PIN_LED_BT_Y, HIGH);
     delay(500);
     digitalWrite(PIN_LED_BT_Y, LOW);
     delay(10);
     if ((global + volchange) <= 90){
      BR = (global + volchange);
      digitalPotWrite(BR);
      Serial.print("BR: ");
      Serial.println(BR);
      global = BR;
      if (R != 0){
       R1 = R1 + volchange;
      }
      else{
         R1 = global;
      }
      Serial.print("R1:                                                         ");
      Serial.println(R1);
      Serial.println("                                     volume up                                                     "); 
     }
     BR1 = ((global)*(Vi/conv));
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
      BR = abs(global - volchange);
      digitalPotWrite(BR);
      Serial.print("BR: ");
      Serial.println(BR);
      global = BR;
      if (R != 0){
       R1 = R1 - volchange;
      }
      else{
         R1 = global;
      }
      Serial.print("R1:                                                         ");
      Serial.println(R1);
      Serial.println("                                     volume down                                                     "); 
     }
     BR1 = ((global)*(Vi/conv));
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
      for (i2 = BRmute; i2 >= 0; i2--){
       digitalPotWrite(i2);
       delay(10);
      }
      Serial.println("                                     mute on                                                     ");
      return 0; 
     }

     else if(inputString == "6" ){              //in case of '6' turn the LED off - mute off
      digitalWrite(PIN_LED_BT_Y, HIGH); 
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW); 
      for (i2 = 0; i2 <= global; i2++){
       digitalPotWrite(i2);
       delay(10);
      }
      BRmute = (BRmute *(Vi/conv));
      delay(5);
      Serial.println("                                     mute off                                                     "); 
      return BRmute;
     }

     else if(inputString == "A" ){                   //in case of 'A' turn the LED on - EQ default
      digitalWrite(PIN_LED_BT_Y, HIGH);
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
      delay(10); 
      Serial.println("                                     EQ Default                                           ");
      PlusMinus = 0.12;      
      Serial.print("                                        PlusMinus                                           ");
      Serial.println(PlusMinus, 3);    
      return 0;    
     }
     
     else if(inputString == "B" ){                   //in case of 'D' turn the LED on - EQ Theatre Mode
      digitalWrite(PIN_LED_BT_Y, HIGH);
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
      delay(10);       
      Serial.println("                                     EQ Theatre Mode                                                ");
      PlusMinus = 0.25;      
      Serial.print("                                        PlusMinus                                           ");
      Serial.println(PlusMinus, 3);    
      return 0;    
     }
     
     else if(inputString == "D" ){                   //in case of 'A' turn the LED on - RCA
      digitalWrite(PIN_LED_BT_Y, HIGH);
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
      delay(10); 
      Serial.println("                                     Input Device RCA                                           ");   
      
      //Channel 1 Input
      digitalWrite(s0, HIGH);
      digitalWrite(s1, LOW);
      digitalWrite(s2, LOW);
      digitalWrite(s3, LOW);
      Serial.print("Input Mux:");
      Serial.println(controlPin[0]);
      
      //Channel 1 Output 
      digitalWrite(s00, HIGH);
      digitalWrite(s11, LOW);
      digitalWrite(s22, LOW);
      digitalWrite(s33, LOW);
      Serial.print("Output Mux:");
      Serial.println(controlPin2[0]);
      return 0;  
     }   

     else if(inputString == "E" ){                   //in case of 'E' turn the LED on - 3mm Audio
      digitalWrite(PIN_LED_BT_Y, HIGH);
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
      delay(10); 
      Serial.println("                                     Input Device 3mm Audio                                        ");
      
      //Channel 4 Input
      digitalWrite(s0, LOW);
      digitalWrite(s1, LOW);
      digitalWrite(s2, HIGH);
      digitalWrite(s3, LOW);
      Serial.print("Input Mux:");
      Serial.println(controlPin2[2]);

      //Channel 4 Output
      digitalWrite(s00, LOW);
      digitalWrite(s11, LOW);
      digitalWrite(s22, HIGH);
      digitalWrite(s33, LOW);
      Serial.print("Output Mux:");
      Serial.println(controlPin2[2]);
      return 0;  
     }  

     else if(inputString == "F" ){                   //in case of 'A' turn the LED on - Optic
      digitalWrite(PIN_LED_BT_Y, HIGH);
      delay(500);
      digitalWrite(PIN_LED_BT_Y, LOW);
      delay(10); 
      Serial.println("                                     Input Device Optic                                           ");   
     
      //Channel 6 Input
      digitalWrite(s0, LOW);
      digitalWrite(s1, HIGH);
      digitalWrite(s2, HIGH);
      digitalWrite(s3, LOW);
      Serial.print("Input Mux:");
      Serial.print(controlPin[1]);

      //Channel 6 Output
      digitalWrite(s0, LOW);
      digitalWrite(s1, HIGH);
      digitalWrite(s2, HIGH);
      digitalWrite(s3, LOW);
      Serial.print("Output Mux:");
      Serial.print(controlPin2[1]);
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

void setup(){                                 // run once, when the sketch starts
 
 Serial.begin(9600);                          // set the baud rate to 9600, same should be of your Serial Monitor
 Serial1.begin(9600);                         // set the rate to 9600 for Serial1 (bluetooth)
 
//Potentiometer setup
   pinMode(CS, OUTPUT);                       //Make CS an output
   SPI.begin();
   
//Led output for bluetooth
   pinMode(PIN_LED_BT_G, OUTPUT); 
   pinMode(PIN_LED_BT_Y, OUTPUT);      
  
//Display status of Sound Detector
   Serial.println("                                 Initialized Sound Detector                  ");
   Serial.println();

//MUX Setup
  pinMode(s0, OUTPUT); 
  pinMode(s1, OUTPUT); 
  pinMode(s2, OUTPUT); 
  pinMode(s3, OUTPUT); 

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

//Potentiometer Setup   
    digitalPotWrite(35);                      //initally set potentiometer on startup
}

 //main code start
void loop(){
  
  double value;
  int i;
  double i3;
  double R2=0;                              //gain of the inputs (value RMS converted)
  double sum = 0;
  double RMSsum = 0;
  double RMSavg = 0;
  double avg = 0;
  double Values[index];
    
  for (i = 1; i <= (index); i++) {
    value = analogRead(PIN_ANALOG_IN);                // Check the analog input
    Serial.print("Status: ");                         // print analog value
    Serial.println(value);
    delay(10);
                 
    Values[i] = pow(value, 2);                        //write new value into array after shifted
    sum += Values[i];                                 //sum the values of the array 
    
    RMSsum = (sqrt(sum))/2;                           //concverting sum to RMS value
    Serial.print("RMS Sum: ");
    Serial.println(RMSsum);
    
    RMSavg = RMSsum/i; 
    Serial.print("RMS Avg: ");
    Serial.println(RMSavg);
   
    Serial.print("BISR2: ");
    Serial.println(BISR2, 2);
    Serial.print("BISR3: ");
    Serial.println(BISR3, 2);
    
    BISR = BluetoothISR(value);
    Serial.print("Initial BISR: ");
    Serial.println(BISR);
    

      if (((BISR >= 1) && ((RMSavg) > BISR2)) || ((BISR >= 1) && ((RMSavg) < BISR3))) {
      Serial.println("______________________________________________________________________________");
      
      if (R1 == 0) {
        R1 = BISR*(conv/Vi);                                                      //(Vo/Vi) = Rwb/Rwa (gain) - wiper value
        digitalPotWrite(R1);
        Serial.print("          Wiper Value (R1): ");                             //Value of wiper outputted - set to initial threshold
        Serial.println(R1);
        }
      
      else {
        Serial.print("R1: ");
        Serial.println(R1);    
        R2 = (RMSavg)*(conv/Vi);                                                    //Current Sum
        Serial.print("R2: ");
        Serial.println(R2);

        if ((R1 < R2) && ((R1 - (R2 - R1)) >= 10) && (R1 < 120)) {
          R = abs(R1 - (R2 - R1));
          for (i3 = global; i3 >= R; i3--){
          digitalPotWrite(i3);                                                    //write the potentiometer to the set threshold
          delay(10);
          Serial.print("-----------------Decreased Wiper Value: ");               //Value of wiper outputted
          Serial.println(i3);
          }
          global = (i3 + 1);
          Serial.print("----------------- R -------------------: ");                //Value of wiper outputted
          Serial.println(R);
        }

        else if ((R1 > R2) && ((R1 + (R1 - R2)) <= 120) && (R1 > 10)) {
          R = abs(R1 + (R1 - R2));
          for (i3 = global; i3 <= R; i3++){
           digitalPotWrite(i3);                                                   //write the potentiometer to the set threshold
           delay(10);
           Serial.print("++++++++++++++++++++Inceased Wiper Value: ");           //Value of wiper outputted
           Serial.println(i3);
          }
          global = (i3 - 1);
          Serial.print("+++++++++++++++++++++ R ++++++++++++++++++: ");              //Value of wiper outputted
          Serial.println(R);
       }

        if (R1 <= 10) {                                                           //if a problem occurs so the digital potentiometer wont go to low
           R = abs(R1 + (R1 - R2));
          for (i3 = R1; i3 <= 15; i3++){
           digitalPotWrite(i3);                                                   //write the potentiometer to the set threshold
           delay(10);
           Serial.print("*******************Inceased Wiper Value: ");             //Value of wiper outputted
           Serial.println(i3);
          }
          global = (i3 - 1);
          R1 = global;
          Serial.print("global: ");                                                 //Value of global
          Serial.println(global);
        }

        else if (R1 >= 120) {                                                     //if a problem occurs so the digital potentiometer wont go to low
          R = abs(R1 - (R2 - R1));
          for (i3 = R1; i3 >= 115; i3--){
          digitalPotWrite(i3);                                                    //write the potentiometer to the set threshold
          delay(10);
          Serial.print("*_*_*_*_*_*_*_*_*_*_*Decreased Wiper Value: ");           //Value of wiper outputted
          Serial.println(i3);
          }
          global = (i3 + 1);
          R1 = global;
          Serial.print("global: ");                                               //Value of global
          Serial.println(global);
        }
     }
   }
 }
    avg = RMSavg;                                                                 //average the values of the array

    Serial.print("                Converted Average BISR:                    ");
    Serial.println(BISR);
    
    Serial.print("                      Average:                             ");
    Serial.println(avg);
    delay(10);
}
  //main code end
