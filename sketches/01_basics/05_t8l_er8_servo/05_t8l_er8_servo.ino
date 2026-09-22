// POC 5 — T8L + ER8 + Arduino + servo
// Stage 2 goal:
//   - Read ER8 CH1 and CH3 simultaneously
//   - Use CH1 (steering) to command the HiTec D646WP on Arduino D9
//   - Continue printing CH3 (throttle) only
//
// Arduino Uno interrupt pins:
//   D2 = ER8 CH1 / steering
//   D3 = ER8 CH3 / throttle
//
// Confirmed T8L / ER8 mapping:
//   CH1 = Right Joystick X (steering)
//   CH3 = Left Joystick Y (throttle)
//
// CH1 calibration from POC 4:
//   Observed low:    ~984 us
//   Observed center: 1489–1496 us
//   Observed high:   ~2002 us
//
// Steering dead zone:
//   ±30 us around the nominal center (1493 us)
//   Therefore 1463–1523 us commands servo center.
//
// Servo output uses a deliberately conservative initial range:
//   Left  = 60°
//   Center = 90°
//   Right = 120°
//
// CH3 remains Serial-only in this stage.
// No propulsion hardware is controlled.

#include <Servo.h>

const int STEERING_PIN = 2;  // ER8 CH1
const int THROTTLE_PIN = 3;  // ER8 CH3
const int SERVO_PIN = 9;     // HiTec D646WP signal

// CH1 steering calibration.
const int STEERING_MIN = 984;
const int STEERING_CENTER = 1493;
const int STEERING_MAX = 2002;
const int STEERING_DEADZONE = 30;

// Conservative initial servo limits.
// These can be characterized further after confirming behavior.
const int SERVO_LEFT = 60;
const int SERVO_CENTER = 90;
const int SERVO_RIGHT = 120;

volatile unsigned long steeringRiseTime = 0;
volatile unsigned long throttleRiseTime = 0;

volatile unsigned long steeringPulseWidth = 0;
volatile unsigned long throttlePulseWidth = 0;

volatile bool steeringNewPulse = false;
volatile bool throttleNewPulse = false;

Servo steeringServo;

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

  steeringServo.attach(SERVO_PIN);
  steeringServo.write(SERVO_CENTER);

  attachInterrupt(digitalPinToInterrupt(STEERING_PIN), steeringISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(THROTTLE_PIN), throttleISR, CHANGE);

  Serial.println("POC 5 — Stage 2: steering servo + throttle monitor");
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

  if (haveSteering) {
    int servoAngle;

    // Keep the servo centered while the transmitter signal is
    // within ±30 us of the observed CH1 center.
    if (abs((int)steering - STEERING_CENTER) <= STEERING_DEADZONE) {
      servoAngle = SERVO_CENTER;
    } else if (steering < STEERING_CENTER - STEERING_DEADZONE) {
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
  }

  // Print the latest values independently so a missing channel
  // does not hide a working channel.
  if (haveSteering || haveThrottle) {
    Serial.print("CH1: ");
    Serial.print(steering);
    Serial.print(" us | CH3: ");
    Serial.print(throttle);
    Serial.print(" us | Servo: ");

    if (haveSteering) {
      // Recalculate the displayed angle from the same calibration.
      int displayedAngle;

      if (abs((int)steering - STEERING_CENTER) <= STEERING_DEADZONE) {
        displayedAngle = SERVO_CENTER;
      } else if (steering < STEERING_CENTER - STEERING_DEADZONE) {
        displayedAngle = map(
          steering,
          STEERING_MIN,
          STEERING_CENTER - STEERING_DEADZONE,
          SERVO_LEFT,
          SERVO_CENTER
        );
      } else {
        displayedAngle = map(
          steering,
          STEERING_CENTER + STEERING_DEADZONE,
          STEERING_MAX,
          SERVO_CENTER,
          SERVO_RIGHT
        );
      }

      displayedAngle = constrain(
        displayedAngle,
        SERVO_LEFT,
        SERVO_RIGHT
      );

      Serial.println(displayedAngle);
    } else {
      Serial.println("-");
    }
  }

  delay(50);
}
