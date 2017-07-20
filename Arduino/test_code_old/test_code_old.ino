//Test_Code

//Sound detector Ports


//Define Hardware connections
#define PIN_GATE_IN 2
#define IRQ_GATE_IN  0
#define PIN_LED_OUT 3
#define PIN_ANALOG_IN A0

int index = 10;   //defining index - used in sound detector loop
int average = 0;
// soundISR()
// This function is installed as an interrupt service routine for the pin
// change interrupt.  When digital input 2 changes state, this routine is called.
// It queries the state of that pin, and sets the onboard LED to reflect that pin's state.

//Function for sound detector 
void soundISR()
{
  int pin_val;

  pin_val = digitalRead(PIN_GATE_IN);
  digitalWrite(PIN_LED_OUT, pin_val);   
}

//0 (bluetooth connected)
//1 (to turn the LED on)
//2 (tor turn the LED off)
//3 (volume up)
//4 (volume down)


char junk;
String inputString = "";

void setup()                    // run once, when the sketch starts
{
 Serial.begin(9600);            // set the baud rate to 9600, same should be of your Serial Monitor
 pinMode(13, OUTPUT);           //Led output for bluetooth

 //Sound detector setup code begin
 
   //  Configure LED pin as output
    pinMode(PIN_LED_OUT, OUTPUT);
  
    // configure input to interrupt
    pinMode(PIN_GATE_IN, INPUT);
    attachInterrupt(IRQ_GATE_IN, soundISR, CHANGE);
  
    // Display status
    Serial.println("Initialized");

// sound detector setup code end

}

void loop()
{
  //Bluetooth Code - main loop

  //Threshold on (1) & off (2)
  if(Serial.available()){
  while(Serial.available())
    {
      char inChar = (char)Serial.read(); //read the input
      inputString += inChar;        //make a string of the characters coming on serial
    }
    Serial.println(inputString);
    while (Serial.available() > 0)  
    { junk = Serial.read() ; }      // clear the serial buffer
    if(inputString == "1"){         //in case of '1' turn the LED on
      digitalWrite(13, HIGH);
      
    }
    else if(inputString == "2"){   //incase of '2' turn the LED off
      digitalWrite(13, LOW);
    }
    inputString = "";
  }
  //Bluetooth code end

  //Sound detector code Start
 
  double value;
  int i;
  double sum = 0;
  double avg = 0;
  double Values[index];
  
  for (i = 0; i <= index-1; i++) {
    value = analogRead(PIN_ANALOG_IN);  // Check the envelope input
    Serial.print("Status:");     // Convert envelope value into a message
    Serial.println(value);
    delay(1000);

    Values[i + 1] = Values[i];  //write array value into the array
    Values[i] = value;
    sum += Values[i]; //sum the values of the array
    Serial.print("Sum:");
    Serial.println(sum); 
 
 }
    avg = sum/index;
    Serial.println("Average:");
    Serial.println(avg);
    delay(1000);
}
