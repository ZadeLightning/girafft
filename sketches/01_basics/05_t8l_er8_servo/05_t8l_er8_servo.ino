// POC 5 — T8L + ER8 + Arduino + servo
// Stage 1 goal:
//   - Read steering from ER8 CH1
//   - Read throttle from ER8 CH3
//   - Print BOTH raw PWM values independently
//
// Stage 2 (servo integration) is intentionally commented out for now.
// Once CH1 + CH3 are proven simultaneously, uncomment the servo code.
//
// Confirmed ER8 channel mapping:
//   CH1 = T8L right joystick X (steering)
//   CH3 = T8L left joystick Y (throttle)
//
// Observed PWM ranges:
//   CH1: ~984 min, ~1493-1496 center, ~2002 max
//   CH3: ~984 min, non-self-centering middle, ~2002 max

#include <Servo.h>

const int STEERING_PIN = 2;  // ER8 CH1
const int THROTTLE_PIN = 3;  // ER8 CH3

// Servo integration — retained for the next stage.
// const int SERVO_PIN = 9;

// Initial steering calibration.
// const int STEERING_MIN = 984;
// const int STEERING_CENTER = 1493;
// const int STEERING_MAX = 2002;
// const int STEERING_DEADZONE = 30;

// Conservative initial servo limits.
// const int SERVO_LEFT = 60;
// const int SERVO_CENTER = 90;
// const int SERVO_RIGHT = 120;

// Servo steeringServo;

unsigned long readPulse(int pin) {
  return pulseIn(pin, HIGH, 25000);
}

void setup() {
  Serial.begin(9600);

  pinMode(STEERING_PIN, INPUT);
  pinMode(THROTTLE_PIN, INPUT);

  // Servo integration — retained for the next stage.
  // steeringServo.attach(SERVO_PIN);
  // steeringServo.write(SERVO_CENTER);

  Serial.println("POC 5 — raw T8L/ER8 channel test");
}

void loop() {
  // Read and report each channel independently.
  // This lets us see exactly which channel, if either, is missing.
  unsigned long steeringPulse = readPulse(STEERING_PIN);
  unsigned long throttlePulse = readPulse(THROTTLE_PIN);

  Serial.print("CH1: ");
  Serial.print(steeringPulse);
  Serial.print(" us | CH3: ");
  Serial.print(throttlePulse);
  Serial.println(" us");

  /*
  // Stage 2 — steering servo integration.
  // Keep this code here so we can restore it after Stage 1 passes.

  if (steeringPulse > 0) {
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
  }
  */

  delay(50);
}
