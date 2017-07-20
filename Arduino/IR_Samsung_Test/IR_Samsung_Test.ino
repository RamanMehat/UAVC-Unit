#include <boarddefs.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <IRremote.h>
#include <IRremoteInt.h>

#define POWER         0xE0E040BF            //from IRrecvDemo, Ken Shirriff
#define SAMSUNG_BITS  32 

IRsend irsend;

void setup()
{
  pinMode (3, OUTPUT);  //output as used in library
}


void loop() {
  // put your main code here, to run repeatedly:
for (int i = 0; i < 3; i++) 
{
    irsend.sendSAMSUNG(POWER, SAMSUNG_BITS); // Sony TV power code
    delay(4000);
}
}
