#include <SPI.h>

byte address = 0x11; //Bits 5 and 4 of command byte are 0 and 1 respectively
                     //Bits 1 and 0 are 0 and 1 respectively
int CS= 53;         //CS (pin 1) connected to port 53 [determines which device to communicate with]
                    //SCK (pin 2) connected to port 52 
                    //SI (pin 3) connected to port 51
                    //Input (pin 5)
                    //Output (pin 6) - where resistor is connected

//int threshold; //will store threshold value

void setup()
{
Serial.begin(9600);            // set the baud rate to 9600
pinMode (CS, OUTPUT); //Make CS an output
SPI.begin();
}

void loop()
{                    
digitalPotWrite(0);
                    //Loops wiper resistance from 0 to max (10K) increments of 391ohms
for (int i = 10; i <= 85; i++)  //Will dim LED as resistance increases
{
digitalPotWrite(i);                                   // Write the value to the wiper terminal 
    Serial.print("Wiper Value increment:");           // Value of wiper outputted
    Serial.println(i);
delay(100);
}
delay(2000);
                                                      //Loops wiper resitance from max to 0
for (int i = 85; i >= 10; i--)                        //Will brighten LED as resistance decreases
{
digitalPotWrite(i);                                   //write the value to the wiper terminal
    Serial.print("Wiper Value Decrement:");           // Value of wiper outputted
    Serial.println(i);
delay(100);
}
delay(2000);
}

int digitalPotWrite(int value)
{
digitalWrite(CS, LOW); //Make CS active LOW
SPI.transfer(address); //call the address you are writing too
SPI.transfer(value);   //transfer the value to the wiper terminal
digitalWrite(CS, HIGH); //Deselect the chip
}

