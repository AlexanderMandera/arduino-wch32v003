void setup() {
    Serial.begin(115200);
}

void loop() {
    if(Serial.available()) {
        Serial.print("I have received: ");

        while(Serial.available()) {
            char c = Serial.read();
            Serial.print(c);
        }

        Serial.println();
    }
    delay(500);
}
