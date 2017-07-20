#include <IRremote.h>

int RED_PIN = 2;
int GREEN_PIN = 7;

int one, two, three, four, five;
int count;

IRsend sender;
IRrecv My_Receiver(8);
IRdecode My_Decoder;

void setup ()
{
  Serial.begin(9600);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
My_Receiver.enableIRIn();
count = 0;
}

void loop ()
{
  int sample = analogRead(A1);
  Serial.println(sample);  // uncomment this to view the raw sound data
  
  if (count % 5 == 0)
    one = sample;
  else if ( count % 5 == 1)
    two = sample;
  else if ( count % 5 == 2)
    three = sample;
  else if ( count % 5 == 3)
    four = sample;
  else
    five = sample;

  int average = (one + two + three + four + five) / 5;
  Serial.println(average);  // uncomment this to view the average sound data
  
  if (average >= 70)
  {
    //sender.send(0xFFA857);  // volume down
    //sender.send(NEC, 0xFFA857, 32);
    digitalWrite(RED_PIN, HIGH);
  }
  else
    digitalWrite(RED_PIN, LOW);

  if ( 10 < average && average < 40)
  {
    //sender.send(0xFF906F);  // volume up
    //sender.send(NEC, 0xFF906F, 32);
    //sender.send(NEC, 0xFF906F, 32);
    //sender.send(NEC, 0xFF906F, 32);
    digitalWrite(GREEN_PIN, HIGH) ;
  }
  else
    digitalWrite(GREEN_PIN, LOW);

  // IR receive bit here
  
  if (My_Receiver.GetResults(&My_Decoder)) {
    My_Decoder.decode();    //Decode the data
    My_Decoder.DumpResults(); //Show the results on serial monitor
    My_Receiver.resume();     //Restart the receiver
  }
 

  count++;
  
  delay(100);
}
