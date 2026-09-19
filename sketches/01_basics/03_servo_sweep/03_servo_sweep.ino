// POC 1.3 — Basic servo control
// Goal: prove that the Arduino can generate a servo control signal.
// Hardware: servo signal wire connected to digital pin 9.

#include <Servo.h>

Servo testServo;
const int SERVO_PIN = 9;

void setup() {
  Serial.begin(9600);
  Serial.println("Booted this shit up!");
  testServo.attach(SERVO_PIN);
  Serial.println("Servo Attatched at Pin:");
  Serial.println(SERVO_PIN);
}

void loop() {
  testServo.write(0);
  delay(1000);

  testServo.write(90);
  delay(1000);

  testServo.write(180);
  delay(1000);
}
