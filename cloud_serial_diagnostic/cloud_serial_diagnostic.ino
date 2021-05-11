void setup() {
  delay(2000);
  
  Serial.begin(57600);

  Serial.println();
  Serial.println("== Serial passthrough diagnostic ==");
  Serial.println("Initializing connection to base...");
  Serial2.begin(57600);

  Serial.println("Ready.");
}

unsigned long last_message = millis();

void loop() {
  int c = Serial2.read();
  if (c >= 0) {
    Serial.print((char)c);
  }

  c = Serial.read();
  if (c >= 0) {
    Serial2.print((char)c);
  }
}
