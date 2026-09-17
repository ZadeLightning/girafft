// POC 1.2 — Serial output
// Goal: learn how the Arduino communicates with the computer.

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Hello from the ASV Arduino!");
  delay(1000);
}
