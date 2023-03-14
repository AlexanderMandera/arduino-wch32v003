uint32_t count;

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(C0, OUTPUT);
}

void loop() {
	digitalWrite(D0, LOW);
	delay(250);
	digitalWrite(D0, HIGH);
	delay(250);
    digitalWrite(C0, LOW);
	delay(100);
	digitalWrite(C0, HIGH);
	delay(100);
	count++;
}
