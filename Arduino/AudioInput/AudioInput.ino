int x = 0;
int sound;
int led1 = 5;
int led2 = 6;
int led3 = 7;
int led4 = 8;
int led5 = 9;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  Serial.begin(9600);
}
 
void loop() {
  
  sound = analogRead(x);
  Serial.println(sound);
    if (sound > 30) {
    digitalWrite(led1, HIGH);
    delay(20);
  }
    if (sound < 30) {
    digitalWrite(led1, LOW);
  }
    if (sound > 100) {
    digitalWrite(led2, HIGH);
    delay(20);
  }
    if (sound < 100) {
    digitalWrite(led2, LOW);
  }
    if (sound > 150) {
    digitalWrite(led3, HIGH);
    delay(20);
  }
    if (sound < 150) {
    digitalWrite(led3, LOW);
  }
    if (sound > 200) {
    digitalWrite(led4, HIGH);
    delay(20);
  }
    if (sound < 200) {
    digitalWrite(led4, LOW);
  }
    if (sound > 250) {
    digitalWrite(led5, HIGH);
    delay(20);
  }
    if (sound < 250) {
    digitalWrite(led5, LOW);
  }
  delay(10);
}
