
int usedPin = 12;

void setup() {

  pinMode(usedPin, OUTPUT);

}

void loop() {

  digitalWrite(usedPin, HIGH);

  delay(2000);

  digitalWrite(usedPin, LOW);

  delay(2000);


}
