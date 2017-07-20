
 // Define hardware connections
//#define PIN_GATE_IN 2
#define IRQ_GATE_IN  0
#define PIN_LED_OUT 3
#define PIN_ANALOG_IN A0

// soundISR()
// This function is installed as an interrupt service routine for the pin
// change interrupt.  When digital input 2 changes state, this routine
// is called.
// It queries the state of that pin, and sets the onboard LED to reflect that 
// pin's state.

//void soundISR()
//{
//  int pin_val;
//
//  pin_val = digitalRead(PIN_GATE_IN);
//  digitalWrite(PIN_LED_OUT, pin_val);   
//}

void setup()
{
  Serial.begin(9600);

//  //  Configure LED pin as output
//  pinMode(PIN_LED_OUT, OUTPUT);
//
//  // configure input to interrupt
//  pinMode(PIN_GATE_IN, INPUT);
//  attachInterrupt(IRQ_GATE_IN, soundISR, CHANGE);

  // Display status
  Serial.println("Initialized");
}

void loop()
{
  int value;

  // Check the envelope input
  value = analogRead(PIN_ANALOG_IN);

  // Convert envelope value into a message
  //Serial.print("Status: ");
  if(value <= 10)
  {
    Serial.println(value);
  }
  else if( (value > 10) && ( value <= 30) )
  {
    Serial.println("Moderate.");
  }
  else if(value > 30)
  {
    Serial.println("Loud.");
  }

  // pause for 1 second
  delay(100);
}

