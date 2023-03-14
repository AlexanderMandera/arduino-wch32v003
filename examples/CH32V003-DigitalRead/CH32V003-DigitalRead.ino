void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D4, INPUT_PULLUP);

  digitalWrite(D0, LOW);
}

void loop() {
  bool in = digitalRead(D4);
  if(in == HIGH) {
    digitalWrite(D0, LOW);
  } else {
    digitalWrite(D0, HIGH);
  }
  delay(100);
}