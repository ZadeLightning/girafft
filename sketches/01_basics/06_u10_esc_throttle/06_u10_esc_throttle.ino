// POC 6 — T8L + ER8 + Arduino + APISQUEEN ESC + U10
// Goal: very-low-throttle forward / neutral / reverse functional test.
//
// Signal chain:
//   T8L right joystick Y (spring-centered) -> ER8 CH2 -> Arduino D3
//   Arduino D9 -> APISQUEEN bidirectional ESC -> U10
//
// Safety behavior:
//   - Starts with ESC command at neutral (1500 us).
//   - Requires CH2 to be detected near center before enabling control.
//   - Invalid/lost receiver signal commands neutral and disarms control.
//   - First-test output is restricted to 1450–1550 us (only +/-50 us
//     from neutral). This may be too narrow to overcome the ESC's neutral
//     deadband; if the motor does not spin, do not increase it blindly.
//   - Use only with the U10 securely restrained and fully submerged.
//
// IMPORTANT:
//   1450–1550 us is a deliberately conservative initial test window, not
//   a verified U10/ESC operating range. Confirm ESC arming/neutral behavior
//   and test direction briefly. Keep clear of the propeller.

#include <Servo.h>

const uint8_t THROTTLE_INPUT_PIN = 3; // ER8 CH2: right joystick Y
const uint8_t ESC_OUTPUT_PIN = 9;     // ESC white PWM signal

// Previously documented T8L/ER8 CH2 calibration; verify in Serial Monitor.
const int THROTTLE_MIN = 984;
const int THROTTLE_CENTER = 1493;
const int THROTTLE_MAX = 2002;

// Typical bidirectional ESC servo-pulse convention; verify on this ESC.
const int ESC_NEUTRAL = 1500;

// Extremely limited first-test window: +/-50 us around neutral.
const int ESC_REVERSE_TEST = 1450;
const int ESC_FORWARD_TEST = 1550;

// Require stick to be close to center at startup before enabling.
const int ARM_DEADZONE_US = 30;

// A pulseIn timeout indicates missing/invalid receiver signal.
const unsigned long SIGNAL_TIMEOUT_US = 25000UL;
const unsigned long SERIAL_INTERVAL_MS = 200UL;

Servo esc;
bool throttleEnabled = false;
unsigned long lastSerialMs = 0;

void setup() {
  Serial.begin(115200);
  pinMode(THROTTLE_INPUT_PIN, INPUT);

  esc.attach(ESC_OUTPUT_PIN);
  esc.writeMicroseconds(ESC_NEUTRAL);

  Serial.println(F("POC 6 — U10 very-low-throttle test"));
  Serial.println(F("Input: ER8 CH2 -> D3; output: D9 -> ESC signal"));
  Serial.println(F("ESC command held at NEUTRAL (1500 us) at startup."));
  Serial.println(F("Center the spring-loaded right joystick Y to enable."));
  Serial.println(F("Test window limited to 1450–1550 us."));
}

void loop() {
  const unsigned long pulse =
      pulseIn(THROTTLE_INPUT_PIN, HIGH, SIGNAL_TIMEOUT_US);

  if (pulse == 0 || pulse < 800 || pulse > 2200) {
    esc.writeMicroseconds(ESC_NEUTRAL);
    throttleEnabled = false;
    if (millis() - lastSerialMs >= SERIAL_INTERVAL_MS) {
      Serial.println(F("NO VALID CH2 SIGNAL -> ESC NEUTRAL; control disarmed"));
      lastSerialMs = millis();
    }
    return;
  }

  if (!throttleEnabled) {
    esc.writeMicroseconds(ESC_NEUTRAL);
    if (abs((int)pulse - THROTTLE_CENTER) <= ARM_DEADZONE_US) {
      throttleEnabled = true;
      Serial.println(F("CH2 centered -> control enabled"));
    } else if (millis() - lastSerialMs >= SERIAL_INTERVAL_MS) {
      Serial.print(F("WAITING FOR CENTER; CH2="));
      Serial.print(pulse);
      Serial.println(F(" us; ESC remains neutral"));
      lastSerialMs = millis();
    }
    return;
  }

  int command = ESC_NEUTRAL;

  // Center deadband: release the stick and command neutral.
  if (abs((int)pulse - THROTTLE_CENTER) > ARM_DEADZONE_US) {
    // Right-stick up (larger pulse) is provisionally mapped to forward.
    // Right-stick down (smaller pulse) is provisionally mapped to reverse.
    command = (int)map((long)pulse,
                       THROTTLE_MIN, THROTTLE_MAX,
                       ESC_REVERSE_TEST, ESC_FORWARD_TEST);
    command = constrain(command, ESC_REVERSE_TEST, ESC_FORWARD_TEST);
  }

  esc.writeMicroseconds(command);

  if (millis() - lastSerialMs >= SERIAL_INTERVAL_MS) {
    Serial.print(F("CH2="));
    Serial.print(pulse);
    Serial.print(F(" us -> ESC="));
    Serial.print(command);
    Serial.println(F(" us"));
    lastSerialMs = millis();
  }
}
