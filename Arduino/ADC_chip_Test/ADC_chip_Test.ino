#include <Wire.h>
#include <Adafruit_ADS1015.h>
//You can have different addressing modes due to i2C. Currently I have ADDR pin connected to GND
//SCL connect to SCL pin, SDA connect to SDA pin
//ADDR used for the four different addressing modes
//A0-A3 different inputs

Adafruit_ADS1115 ads(0x48);    //16 bit adc for ADDR -> GND
//Adafruit_ADS1115 ads(0x49);  //16 bit adc for ADDR -> VDD
//Adafruit_ADS1115 ads(0x4A);  //16 bit adc for ADDR -> SDA
//Adafruit_ADS1115 ads(0x4B);  //16 bit adc for ADDR -> SCL

double Voltage0,Voltage1,Voltage2, Voltage3 = 0.0;  //initialize voltage 

void setup(void) 
{
  Serial.begin(9600);  
  ads.begin();
  ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 0.1875mV (default)  just to show what the default 
}

void loop(void) 
{
  int16_t adc0;  // returns 16 bit adc values for pin A0
  int16_t adc1;  // returns 16 bit adc values for pin A1
  int16_t adc2;  // returns 16 bit adc values for pin A2
  int16_t adc3;  // returns 16 bit adc values for pin A3

  adc0 = ads.readADC_SingleEnded(0); //reads single ended (ref to ground) input from pin A0
  //adc1 = ads.readADC_SingleEnded(1); //reads single ended (ref to ground) input from pin A1
  adc2 = ads.readADC_SingleEnded(2);
//adc3 = ads.readADC_SingleEnded(3);
  
  Voltage0 = (adc0 * 0.0001875) ;  //THis is the input audio voltage
  //Voltage1 = (adc0 * 0.000152597);
  Voltage2 = (adc2 * 0.0001875);     //This is the output auido voltage
//Voltage2 = (adc0 * 0.1875)/1000;
//Voltage3 = (adc0 * 0.1875)/1000;

 double wiper = Voltage2/Voltage0; // gives you the gain which we can use to convert to wiper
  


  
  Serial.print("Input A0: "); 
  Serial.println(adc0);
  Serial.print("Input A2: "); 
  Serial.println(adc2);
  
  Serial.print("\tVoltage0: ");
  Serial.println(Voltage0, 7);  
  Serial.println();
  
  Serial.print("\tVoltage2: ");
  Serial.println(Voltage2, 7);  
  Serial.println();

  Serial.print("\tWiper Value: ");
  Serial.println(wiper, 7);  
  Serial.println();
  
  delay(1000);
}
