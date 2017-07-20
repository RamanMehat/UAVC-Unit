#include <boarddefs.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <IRremote.h>
#include <IRremoteInt.h>

#include "IRremote.h"

/*-----( Declare Constants )-----*/
int receiver = 11; // pin 1 of IR receiver to Arduino digital pin 11

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'
/*-----( Declare Variables )-----*/


void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);
  Serial.println("IR Receiver Raw Data + Button Decode Test");
  irrecv.enableIRIn(); // Start the receiver

}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if (irrecv.decode(&results)) // have we received an IR signal?

  {
//  Serial.println(results.value, HEX);  UN Comment to see raw values
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */

/*-----( Declare User-written Functions )-----*/
void translateIR() // takes action based on IR code received

// Samsung TV Codes

{

  switch(results.value)

  {

  case 0xE0E008F7:  
    Serial.println(" CH-            "); 
    break;

  case 0xE0E048B7: 
    Serial.println(" CH+            "); 
    break;

  case 0xE0E0E02F:  
    Serial.println(" VOL-           "); 
    break;

  case 0xE0E0E01F:  
    Serial.println(" VOL+           "); 
    break;

  case 0xE0E0F00F:  
    Serial.println(" MUTE           "); 
    break;

  case 0xE0E040BF:  
    Serial.println(" POWER           "); 
    break;
  }

  delay(500);

} //END translateIR

