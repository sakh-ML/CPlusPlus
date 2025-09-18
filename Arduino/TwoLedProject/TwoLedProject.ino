int firstPin = 12;
int secondePin = 8;

void setup() {

  pinMode(firstPin, OUTPUT);
  pinMode(secondePin, OUTPUT);


}

void loop() {

  digitalWrite(firstPin, HIGH);

  delay(2000);

  digitalWrite(firstPin, LOW);

  delay(2000);
  
  digitalWrite(secondePin, HIGH);

  delay(2000);

  digitalWrite(secondePin, LOW);

  delay(2000);


}
