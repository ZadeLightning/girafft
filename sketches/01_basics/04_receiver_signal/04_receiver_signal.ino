// POC 4 — Receiver signal visibility
// Goal: read one PWM signal from the ER8 and print its pulse width.

const int RECEIVER_PIN = 2;

void setup() {
  Serial.begin(9600);
  pinMode(RECEIVER_PIN, INPUT);
  Serial.println("Receiver signal test started");
}

void loop() {
  unsigned long pulseWidth = pulseIn(RECEIVER_PIN, HIGH, 25000);

  if (pulseWidth > 0) {
    Serial.println(pulseWidth);
  } else {
    Serial.println("No pulse detected");
  }

  delay(250);
}
