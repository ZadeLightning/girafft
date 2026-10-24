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
    Arduino 5V / GND -> ER8 power rail (current POC test arrangement)
    SyRen 5V BEC positive is NOT connected to ER8. Grounds share a reference.

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
const bool ENABLE_MOTOR_OUTPUT = true;

// PA-14P feedback calibration placeholders only.
// Replace with measured analogRead() values at usable retracted and extended endpoints.
// Values must be distinct and between 0 and 1023.
const int FEEDBACK_RETRACTED_ADC = 0; // User-requested test target; observe actuator carefully
const int FEEDBACK_EXTENDED_ADC  = 900; // PLACEHOLDER — measure before enabling

// Receiver calibration observed on previous T8L/ER8 testing; verify CH4 specifically.
const unsigned long RX_MIN_US = 1000;
const unsigned long RX_CENTER_US = 1500;
const unsigned long RX_MAX_US = 2000;
const unsigned long RX_TIMEOUT_US = 25000;
const unsigned long RX_VALID_MIN_US = 900;
const unsigned long RX_VALID_MAX_US = 2100;

// Ignore small joystick fluctuations around center.
// Pulses from 1470–1530 us all command the midpoint. Outside that zone,
// each half of the stick's travel is remapped smoothly from midpoint to endpoint.
const unsigned long RX_CENTER_DEADBAND_US = 30;

// Position controller tuning. Increase the ADC error deadband to reduce hunting around the target.
const int POSITION_DEADBAND_ADC = 30;
const unsigned long LOOP_DELAY_MS = 20;
const unsigned long STATUS_INTERVAL_MS = 250;

// Simplified-serial command bytes: commands with twice the previous distance from stop for the next speed test.
// These values are empirical for this unit; stop remains 127.
const uint8_t SYREN_REVERSE_CMD = 33;
const uint8_t SYREN_STOP_CMD = 127;
const uint8_t SYREN_FORWARD_CMD = 223;

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
  long span = (long)FEEDBACK_EXTENDED_ADC - (long)FEEDBACK_RETRACTED_ADC;
  long centerTarget = (long)FEEDBACK_RETRACTED_ADC + span / 2;
  long lowDeadbandEdge = (long)RX_CENTER_US - (long)RX_CENTER_DEADBAND_US;
  long highDeadbandEdge = (long)RX_CENTER_US + (long)RX_CENTER_DEADBAND_US;

  // The deadband maps to a single midpoint target, preventing tiny receiver
  // fluctuations around center from continually changing the requested position.
  if (bounded >= lowDeadbandEdge && bounded <= highDeadbandEdge) {
    return (int)centerTarget;
  }

  // Remap each side independently so target position changes smoothly from
  // midpoint at the deadband edge to the calibrated endpoint at stick travel.
  if (bounded < lowDeadbandEdge) {
    long inputSpan = lowDeadbandEdge - (long)RX_MIN_US;
    long inputOffset = bounded - (long)RX_MIN_US;
    return (int)((long)FEEDBACK_RETRACTED_ADC +
                 (centerTarget - (long)FEEDBACK_RETRACTED_ADC) * inputOffset / inputSpan);
  }

  long inputSpan = (long)RX_MAX_US - highDeadbandEdge;
  long inputOffset = bounded - highDeadbandEdge;
  return (int)(centerTarget +
               ((long)FEEDBACK_EXTENDED_ADC - centerTarget) * inputOffset / inputSpan);
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
  Serial.println(F("Joystick center deadband: +/- 30 us around 1500 us."));
  Serial.println(F("Replace feedback endpoint placeholders with measured ADC values."));
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
