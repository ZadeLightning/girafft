// POC 6 — T8L + ER8 + Arduino + U10 ESC
// Goal:
//   - Read T8L throttle from ER8 CH3.
//   - Command the APISQUEEN waterproof bidirectional ESC.
//   - ESC drives the APISQUEEN U10 underwater thruster.
//
// Signal chain:
//   T8L throttle -> ER8 CH3 -> Arduino D3 -> Arduino D9 -> ESC -> U10
//
// Confirmed T8L / ER8 mapping:
//   CH3 = Left Joystick Y (throttle)
//
// ESC command range:
//   ~1000 us = maximum reverse
//   ~1500 us = stop / neutral
//   ~2000 us = maximum forward
//
// This sketch intentionally starts conservatively:
//   - ESC receives 1500 us (neutral) at startup.
//   - The transmitter throttle must be detected near neutral before
//     the sketch will pass throttle commands through to the ESC.
//   - If the receiver signal is lost, the ESC is commanded to neutral.
//
// IMPORTANT:
//   The U10 must be submerged in water for powered operation.
//   Do not run the U10 at meaningful power in air.

#include <Servo.h>

const int THROTTLE_INPUT_PIN = 3;  // ER8 CH3
const int ESC_OUTPUT_PIN = 9;      // ESC signal

// Observed CH3 signal range is expected to be approximately
// 1000–2000 us, consistent with the T8L/ER8 system.
const int THROTTLE_MIN = 984;
const int THROTTLE_CENTER = 1493;
const int THROTTLE_MAX = 2002;

// ESC command range.
const int ESC_REVERSE = 1000;
const int ESC_NEUTRAL = 1500;
const int ESC_FORWARD = 2000;

// Startup safety: throttle must be near center before control is enabled.
const int ARM_DEADZONE = 30;

// Receiver signal timeout.
const unsigned long SIGNAL_TIMEOUT_US = 25000;

Servo esc;

bool throttleEnabled = false;

void setup() {
  Serial.begin(9600);

  pinMode(THROTTLE_INPUT_PIN, INPUT);

  // Always start the ESC at neutral.
  esc.attach(ESC_OUTPUT_PIN);
  esc.writeMicroseconds(ESC_NEUTRAL);

  Serial.println("POC 6 — U10 ESC throttle control");
  Serial.println("ESC commanded to NEUTRAL.");
  Serial.println("Move throttle to center to enable control.");
}

void loop() {
  unsigned long throttlePulse =
    pulseIn(THROTTLE_INPUT_PIN, HIGH, SIGNAL_TIMEOUT_US);

  // No valid receiver signal: force ESC to neutral.
  if (throttlePulse == 0) {
    esc.writeMicroseconds(ESC_NEUTRAL);
    throttleEnabled = false;

    Serial.println("NO SIGNAL -> ESC NEUTRAL");
    delay(100);
    return;
  }

  // Keep startup locked until the throttle is physically near center.
  if (!throttleEnabled) {
    esc.writeMicroseconds(ESC_NEUTRAL);

    if (abs((int)throttlePulse - THROTTLE_CENTER) <= ARM_DEADZONE) {
      throttleEnabled = true;
      Serial.println("THROTTLE CENTERED -> CONTROL ENABLED");
    } else {
      Serial.print("ARMING: throttle = ");
      Serial.print(throttlePulse);
      Serial.println(" us; return throttle to center.");
    }

    delay(50);
    return;
  }

  // Convert receiver pulse width directly into the ESC's
  // bidirectional command range.
  int escCommand = map(
    throttlePulse,
    THROTTLE_MIN,
    THROTTLE_MAX,
    ESC_REVERSE,
    ESC_FORWARD
  );

  escCommand = constrain(
    escCommand,
    ESC_REVERSE,
    ESC_FORWARD
  );

  // Maintain a true 1500 us neutral point.
  if (abs((int)throttlePulse - THROTTLE_CENTER) <= ARM_DEADZONE) {
    escCommand = ESC_NEUTRAL;
  }

  esc.writeMicroseconds(escCommand);

  Serial.print("CH3: ");
  Serial.print(throttlePulse);
  Serial.print(" us -> ESC: ");
  Serial.print(escCommand);
  Serial.println(" us");

  delay(50);
}
