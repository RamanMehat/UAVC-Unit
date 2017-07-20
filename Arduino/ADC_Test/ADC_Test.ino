


//#define PIN_ANALOG_IN A7 //Left
#define PIN_ANALOG_IN2 A6 //Right

int value = 0;
int value2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //pinMode(A1 , OUTPUT);
  Serial.println("Initialized");
}

void loop() {
  // put your main code here, to run repeatedly:

  // Check the envelope input
//  value = analogRead(PIN_ANALOG_IN);
//  Serial.print("1st Value:");
//  Serial.println(value);
//  
  value2 = analogRead(PIN_ANALOG_IN2);
   Serial.print("2nd Value:");
   Serial.println(value2);
   
   delay(500);
}
