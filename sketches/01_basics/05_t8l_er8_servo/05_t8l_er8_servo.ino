// POC 5 — T8L + ER8 + Arduino + servo
// Goal:
//   - Read steering from ER8 CH1
//   - Read throttle from ER8 CH3
//   - Print both PWM values
//   - Use steering to command the HiTec D646WP on Arduino D9
//
// Current observed ER8 CH1 steering range:
//   Min:    ~984 us
//   Center: ~1493 us
//   Max:    ~2002 us
//
// The steering dead zone is intentionally wider than the observed
// center jitter so small transmitter noise does not move the servo.

#include <Servo.h>

const int STEERING_PIN = 2;  // ER8 CH1
const int THROTTLE_PIN = 3;  // ER8 CH3 — verify against T8L control

const int SERVO_PIN = 9;

const int STEERING_MIN = 984;
const int STEERING_CENTER = 1493;
const int STEERING_MAX = 2002;
const int STEERING_DEADZONE = 30;

// Conservative initial servo limits.
// Adjust only after verifying the D646WP's safe mechanical range.
const int SERVO_LEFT = 60;
const int SERVO_CENTER = 90;
const int SERVO_RIGHT = 120;

Servo steeringServo;

unsigned long readPulse(int pin) {
  return pulseIn(pin, HIGH, 25000);
}

void setup() {
  Serial.begin(9600);

  pinMode(STEERING_PIN, INPUT);
  pinMode(THROTTLE_PIN, INPUT);

  steeringServo.attach(SERVO_PIN);
  steeringServo.write(SERVO_CENTER);

  Serial.println("POC 5 — T8L + ER8 + Arduino + servo");
}

void loop() {
  unsigned long steeringPulse = readPulse(STEERING_PIN);
  unsigned long throttlePulse = readPulse(THROTTLE_PIN);

  if (steeringPulse > 0 && throttlePulse > 0) {
    int servoAngle;

    if (abs((int)steeringPulse - STEERING_CENTER) <= STEERING_DEADZONE) {
      servoAngle = SERVO_CENTER;
    } else if (steeringPulse < STEERING_CENTER) {
      servoAngle = map(
        steeringPulse,
        STEERING_MIN,
        STEERING_CENTER - STEERING_DEADZONE,
        SERVO_LEFT,
        SERVO_CENTER
      );
    } else {
      servoAngle = map(
        steeringPulse,
        STEERING_CENTER + STEERING_DEADZONE,
        STEERING_MAX,
        SERVO_CENTER,
        SERVO_RIGHT
      );
    }

    servoAngle = constrain(servoAngle, SERVO_LEFT, SERVO_RIGHT);
    steeringServo.write(servoAngle);

    Serial.print("Steering: ");
    Serial.print(steeringPulse);
    Serial.print(" us | Throttle: ");
    Serial.print(throttlePulse);
    Serial.print(" us | Servo: ");
    Serial.println(servoAngle);
  } else {
    Serial.println("Missing receiver pulse");
  }

  delay(50);
}
