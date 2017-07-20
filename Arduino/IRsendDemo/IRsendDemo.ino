#include <boarddefs.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <IRremote.h>
#include <IRremoteInt.h>

/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

IRsend irsend;
//unsigned int rawCodes[] = {4550, 4400, 600, 1650, 550, 1650, 600, 1650, 550, 600, 550, 500, 600, 550, 550, 550, 600, 500, 600, 1650, 550, 1700, 550, 1650, 600, 550, 550, 550, 550, 600, 500, 550, 600, 550, 550, 1650, 600, 1650, 550, 1650, 600, 550, 550, 550, 550, 600, 550, 550, 550, 550, 550, 600, 550, 550, 550, 550, 550, 1650, 600, 1650, 550, 1700, 550, 1650, 600, 1650, 550};

decode_results results;

void setup()
{
  Serial.begin(9600);
}

void loop() {
	//for (int i = 0; i < 68; i++) {
	 //irsend.sendRaw(rawCodes[i], 1, 38);
   irsend.sendNEC(0xE0E0E01F,32);
   Serial.println(results.value, HEX);
	 delay(30);
	//}
	delay(5000); //5 second delay between each signal burst
}
