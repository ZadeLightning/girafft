// POC 5 — T8L + ER8 + Arduino + servo
// Stage 1 goal:
//   - Read ER8 CH1 and CH3 simultaneously
//   - Print both raw PWM pulse widths
//
// Stage 2 (servo integration) is retained below in comments.
// Once simultaneous CH1 + CH3 capture is proven, we will enable it.
//
// Arduino Uno interrupt pins:
//   D2 = interrupt 0 = ER8 CH1 / steering
//   D3 = interrupt 1 = ER8 CH3 / throttle
//
// Confirmed T8L / ER8 mapping:
//   CH1 = Right Joystick X (steering)
//   CH3 = Left Joystick Y (throttle)
//
// Observed ranges:
//   CH1: ~984 min, ~1493-1496 center, ~2002 max
//   CH3: ~984 min, non-self-centering middle, ~2002 max
//
// pulseIn() is intentionally NOT used here.
// It is blocking, so reading two continuously generated PWM channels
// sequentially can cause one channel's pulse to be missed.
// Interrupt-based edge timing lets the Arduino watch both inputs.

#include <Servo.h>

const int STEERING_PIN = 2;  // ER8 CH1
const int THROTTLE_PIN = 3;  // ER8 CH3

// Shared ISR state. volatile is required because these values are
// modified inside interrupt routines.
volatile unsigned long steeringRiseTime = 0;
volatile unsigned long throttleRiseTime = 0;

volatile unsigned long steeringPulseWidth = 0;
volatile unsigned long throttlePulseWidth = 0;

volatile bool steeringNewPulse = false;
volatile bool throttleNewPulse = false;

void steeringISR() {
  unsigned long now = micros();

  if (digitalRead(STEERING_PIN) == HIGH) {
    steeringRiseTime = now;
  } else {
    if (steeringRiseTime != 0) {
      steeringPulseWidth = now - steeringRiseTime;
      steeringNewPulse = true;
    }
  }
}

void throttleISR() {
  unsigned long now = micros();

  if (digitalRead(THROTTLE_PIN) == HIGH) {
    throttleRiseTime = now;
  } else {
    if (throttleRiseTime != 0) {
      throttlePulseWidth = now - throttleRiseTime;
      throttleNewPulse = true;
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(STEERING_PIN, INPUT);
  pinMode(THROTTLE_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(STEERING_PIN), steeringISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(THROTTLE_PIN), throttleISR, CHANGE);

  Serial.println("POC 5 — simultaneous CH1 + CH3 receiver test");
}

void loop() {
  unsigned long steering;
  unsigned long throttle;
  bool haveSteering;
  bool haveThrottle;

  // Copy ISR values safely, then release interrupts.
  noInterrupts();
  steering = steeringPulseWidth;
  throttle = throttlePulseWidth;
  haveSteering = steeringNewPulse;
  haveThrottle = throttleNewPulse;
  steeringNewPulse = false;
  throttleNewPulse = false;
  interrupts();

  if (haveSteering || haveThrottle) {
    Serial.print("CH1: ");
    if (haveSteering) {
      Serial.print(steering);
    } else {
      Serial.print("-");
    }

    Serial.print(" us | CH3: ");
    if (haveThrottle) {
      Serial.print(throttle);
    } else {
      Serial.print("-");
    }

    Serial.println(" us");
  }

  /*
  // Stage 2 — steering servo integration.
  // Retained here so it can be enabled after Stage 1 passes.

  const int SERVO_PIN = 9;

  const int STEERING_MIN = 984;
  const int STEERING_CENTER = 1493;
  const int STEERING_MAX = 2002;
  const int STEERING_DEADZONE = 30;

  const int SERVO_LEFT = 60;
  const int SERVO_CENTER = 90;
  const int SERVO_RIGHT = 120;

  Servo steeringServo;

  // Move these declarations to global scope and initialize/attach
  // in setup() before enabling this stage.

  int servoAngle;

  if (abs((int)steering - STEERING_CENTER) <= STEERING_DEADZONE) {
    servoAngle = SERVO_CENTER;
  } else if (steering < STEERING_CENTER) {
    servoAngle = map(
      steering,
      STEERING_MIN,
      STEERING_CENTER - STEERING_DEADZONE,
      SERVO_LEFT,
      SERVO_CENTER
    );
  } else {
    servoAngle = map(
      steering,
      STEERING_CENTER + STEERING_DEADZONE,
      STEERING_MAX,
      SERVO_CENTER,
      SERVO_RIGHT
    );
  }

  servoAngle = constrain(servoAngle, SERVO_LEFT, SERVO_RIGHT);
  steeringServo.write(servoAngle);
  */
  
  delay(50);
}
