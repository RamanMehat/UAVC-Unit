#include <boarddefs.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <IRremote.h>
#include <IRremoteInt.h>

#define ir_led 3
int address = 7;
int data = 2;        //2 = power button 

boolean bin[8],Data[32];

void setup()
{
  Serial.begin(9600);
  pinMode(ir_led,OUTPUT);
  pinMode(7,INPUT);        //a toggle switch
  digitalWrite(7,HIGH);
  
  dec_to_bin(address);      //convert the address into binary
  for(int i=0;i<8;i++)
  {
    Data[i]=bin[i];
    Data[i+8]=bin[i];
  }
}

void loop()
{
  if(digitalRead(7)==0)    //transmit data when a swtich is pressed
  {
    data=2;
    dec_to_bin(data);      //convert data into binary
    for(int i=0;i<8;i++)
    {
      Data[i+16]=bin[i];
      Data[i+24]=!bin[i];
    }

/*  for(int i=0;i<32;i++)
  {
    Serial.print(Data[i],BIN);
  }
  Serial.println();
*/
  
    transmit();          //transmit
    delay(54);
  }
}

void dec_to_bin(int dec)
{
  for(int i=0;i<8;i++)
  {
    bin[i]=dec%2;
    dec=dec/2;
  }
}

void transmit()                  //carrier = 37.9 KHz
{
  burst(173);                    // start bit = 4500/26=170
  digitalWrite(ir_led,LOW);
  delayMicroseconds(4500);       //start bit
  for(int i=0;i<32;i++)
  {
    if(Data[i]==1)
    {
      burst(22);
      digitalWrite(ir_led,LOW);
      delayMicroseconds(1690);      //high bit
    }
    else
    {
      burst(22);
      digitalWrite(ir_led,LOW);
      delayMicroseconds(560);      //low bit
    }
  }
  burst(22);
  digitalWrite(ir_led,LOW);        //stop bit
  delayMicroseconds(560);      
}

void burst(int pulses)
{
  for(int i=0;i<pulses; i++)
  {
    digitalWrite(ir_led,1);
    delayMicroseconds(9);
    digitalWrite(ir_led,0);
    delayMicroseconds(17);
  }
}
