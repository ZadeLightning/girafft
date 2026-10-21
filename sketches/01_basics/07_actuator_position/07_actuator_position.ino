/*
  POC 7 — T8L / ER8 / Arduino Uno / SyRen 10 / PA-14P

  Intended control chain:
    T8L left joystick X -> ER8 CH4 -> Arduino D4
    Arduino SoftwareSerial TX on D8 -> SyRen S1
    SyRen M1/M2 -> PA-14P motor leads (black/red per PA-14P manual)
    PA-14P potentiometer: yellow -> Arduino 5V, white -> Arduino GND,
                          blue wiper -> Arduino A0

  Power:
    12 V lead-acid battery -> fused SyRen B+ / B-
    Mac USB -> Arduino Uno
    ER8 is planned to be powered from SyRen 5V / 0V for this test.
    DO NOT connect SyRen 5V to Arduino 5V. Grounds must share a reference.

  SAFETY / COMMISSIONING:
    - Motor output is intentionally DISABLED by default.
    - SyRen simplified-serial command mapping is disputed between the supplied
      Quick Start and User Guide. Do not enable motor output until the stop byte
      and direction mapping are resolved/verified with the exact unit.
    - Potentiometer endpoint ADC values below are placeholders. Calibrate both
      endpoints with the actuator safely disconnected from motor power or with
      a safe measurement procedure, then enter measured values.
    - Do not enable motor control until receiver channel, feedback wiring,
      endpoints, SyRen DIP settings, and serial protocol have been checked.
    - This sketch implements target-position control, not direct motor-speed
      control. Left=fully retracted, center=half stroke, right=fully extended.
*/

#include <SoftwareSerial.h>

// ----- Pin allocation -----
const uint8_t RECEIVER_CH4_PIN = 4;   // ER8 CH4 = T8L left joystick X
const uint8_t FEEDBACK_PIN = A0;      // PA-14P potentiometer wiper
const uint8_t SYREN_RX_UNUSED_PIN = 7; // No wire required for one-way simplified serial
const uint8_t SYREN_TX_PIN = 8;      // Arduino -> SyRen S1

SoftwareSerial syrenSerial(SYREN_RX_UNUSED_PIN, SYREN_TX_PIN); // RX, TX

// ----- Commissioning interlocks -----
// Keep false until serial command mapping and feedback calibration are confirmed.
const bool ENABLE_MOTOR_OUTPUT = false;

// PA-14P feedback calibration placeholders only.
// Replace with measured analogRead() values at usable retracted and extended endpoints.
// Values must be distinct and between 0 and 1023.
const int FEEDBACK_RETRACTED_ADC = 100; // PLACEHOLDER — measure before enabling
const int FEEDBACK_EXTENDED_ADC  = 900; // PLACEHOLDER — measure before enabling

// Receiver calibration observed on previous T8L/ER8 testing; verify CH4 specifically.
const unsigned long RX_MIN_US = 1000;
const unsigned long RX_CENTER_US = 1500;
const unsigned long RX_MAX_US = 2000;
const unsigned long RX_TIMEOUT_US = 25000;
const unsigned long RX_VALID_MIN_US = 900;
const unsigned long RX_VALID_MAX_US = 2100;

// Position controller tuning — start conservatively.
const int POSITION_DEADBAND_ADC = 12;
const unsigned long LOOP_DELAY_MS = 20;
const unsigned long STATUS_INTERVAL_MS = 250;

// Simplified-serial command bytes stated by the supplied Quick Start Guide.
// The supplied User's Guide has conflicting direction/stop descriptions.
// These are not used while ENABLE_MOTOR_OUTPUT is false.
const uint8_t SYREN_REVERSE_CMD = 120;  // Conservative near-stop candidate; verify before enabling
const uint8_t SYREN_STOP_CMD = 128;     // Quick Start value; verify before enabling
const uint8_t SYREN_FORWARD_CMD = 136;  // Conservative near-stop candidate; verify before enabling

unsigned long lastStatusMs = 0;
unsigned long lastValidReceiverMs = 0;
bool receiverHasBeenValid = false;
bool printedNoReceiver = false;

bool feedbackCalibrationPlausible() {
  return FEEDBACK_RETRACTED_ADC >= 1 &&
         FEEDBACK_RETRACTED_ADC <= 1022 &&
         FEEDBACK_EXTENDED_ADC >= 1 &&
         FEEDBACK_EXTENDED_ADC <= 1022 &&
         abs(FEEDBACK_EXTENDED_ADC - FEEDBACK_RETRACTED_ADC) >= 100;
}

bool readReceiverPulse(unsigned long &pulseUs) {
  pulseUs = pulseIn(RECEIVER_CH4_PIN, HIGH, RX_TIMEOUT_US);
  return pulseUs >= RX_VALID_MIN_US && pulseUs <= RX_VALID_MAX_US;
}

int pulseToTargetAdc(unsigned long pulseUs) {
  long bounded = constrain((long)pulseUs, (long)RX_MIN_US, (long)RX_MAX_US);
  long fraction = bounded - (long)RX_MIN_US;
  long span = (long)FEEDBACK_EXTENDED_ADC - (long)FEEDBACK_RETRACTED_ADC;
  return FEEDBACK_RETRACTED_ADC + (int)(span * fraction / (long)(RX_MAX_US - RX_MIN_US));
}

int readFeedbackAdc() {
  // Small averaging reduces ADC noise. It does not replace endpoint calibration.
  long total = 0;
  for (uint8_t i = 0; i < 4; i++) {
    total += analogRead(FEEDBACK_PIN);
    delay(2);
  }
  return (int)(total / 4);
}

void sendSyRenStop() {
  if (ENABLE_MOTOR_OUTPUT) {
    syrenSerial.write(SYREN_STOP_CMD);
  }
}

void commandSyRenTowardTarget(int targetAdc, int actualAdc) {
  if (!ENABLE_MOTOR_OUTPUT) return;

  int error = targetAdc - actualAdc;
  if (abs(error) <= POSITION_DEADBAND_ADC) {
    sendSyRenStop();
    return;
  }

  // Potentiometer direction and motor direction must be confirmed empirically.
  // These assignments are placeholders until a short direction test establishes
  // which command moves the actuator toward increasing ADC readings. The candidate
  // commands are intentionally close to neutral, but must not be used until the
  // stop-byte discrepancy is resolved and a safe direction test is planned.
  if (error > 0) {
    syrenSerial.write(SYREN_FORWARD_CMD);
  } else {
    syrenSerial.write(SYREN_REVERSE_CMD);
  }
}

void setup() {
  pinMode(RECEIVER_CH4_PIN, INPUT);
  pinMode(SYREN_TX_PIN, OUTPUT);
  digitalWrite(SYREN_TX_PIN, HIGH); // UART idle state
  Serial.begin(115200);
  syrenSerial.begin(9600);

  Serial.println(F("\nPOC 7 — PA-14P position-control diagnostic"));
  Serial.println(F("T8L left joystick X -> ER8 CH4 -> Arduino D4"));
  Serial.println(F("PA-14P wiper -> A0; SyRen S1 <- SoftwareSerial TX D8"));
  Serial.println(F("Motor output is DISABLED by default."));
  Serial.println(F("Confirm SyRen stop/direction command mapping before enabling."));
  Serial.println(F("Replace feedback endpoint placeholders with measured ADC values."));

  if (!feedbackCalibrationPlausible()) {
    Serial.println(F("ERROR: feedback endpoint calibration values are not plausible."));
  }
}

void loop() {
  unsigned long pulseUs = 0;
  bool receiverValid = readReceiverPulse(pulseUs);
  int feedbackAdc = readFeedbackAdc();

  bool feedbackValid = feedbackCalibrationPlausible() &&
                       feedbackAdc > 2 && feedbackAdc < 1021;

  if (receiverValid) {
    receiverHasBeenValid = true;
    lastValidReceiverMs = millis();
    printedNoReceiver = false;
  }

  if (!receiverValid) {
    sendSyRenStop();
    if (!printedNoReceiver) {
      Serial.println(F("NO VALID ER8 CH4 SIGNAL — motor stop requested if output enabled."));
      printedNoReceiver = true;
    }
  } else if (!feedbackValid) {
    sendSyRenStop();
  } else {
    int targetAdc = pulseToTargetAdc(pulseUs);
    commandSyRenTowardTarget(targetAdc, feedbackAdc);
  }

  if (millis() - lastStatusMs >= STATUS_INTERVAL_MS) {
    lastStatusMs = millis();
    Serial.print(F("CH4="));
    if (receiverValid) Serial.print(pulseUs);
    else Serial.print(F("INVALID"));
    Serial.print(F(" us | feedback ADC="));
    Serial.print(feedbackAdc);
    Serial.print(F(" | feedback="));
    Serial.print(feedbackValid ? F("valid") : F("invalid / not calibrated"));
    if (receiverValid && feedbackValid) {
      Serial.print(F(" | target ADC="));
      Serial.print(pulseToTargetAdc(pulseUs));
    }
    Serial.print(F(" | motor output="));
    Serial.println(ENABLE_MOTOR_OUTPUT ? F("ENABLED") : F("DISABLED"));
  }

  delay(LOOP_DELAY_MS);
}
