#include <SoftwareSerial.h>

// ============================================================
// POC 7-1 — SyRen 10 + PA-14P pre-test jog
// RadioMaster T8L -> ER8 -> Arduino Uno -> SyRen 10
// ============================================================

// ER8 CH4 signal -> Arduino D4
const uint8_t RECEIVER_CH4_PIN = 4;

// PA-14P feedback potentiometer:
// Yellow -> Arduino 5V
// White  -> Arduino GND
// Blue   -> Arduino A0
const uint8_t FEEDBACK_PIN = A0;

// Arduino D8 TX -> SyRen S1
// Arduino D7 RX is unused; no wire required.
SoftwareSerial syrenSerial(7, 8);

// Candidate simplified-serial commands.
// IMPORTANT: SyRen documentation conflicts on neutral and direction.
// These values are not yet verified against this specific unit.
const uint8_t STOP_CMD = 127;
const uint8_t JOG_A = 160;
const uint8_t JOG_B = 96;

const unsigned long JOG_DURATION_MS = 1000;
const unsigned long RECEIVER_TIMEOUT_US = 30000;
const unsigned long STATUS_INTERVAL_MS = 1000;

unsigned long lastStatusMs = 0;

void stopMotor() {
  syrenSerial.write(STOP_CMD);
}

void jogMotor(uint8_t command) {
  Serial.println(F("Jog command sent."));
  syrenSerial.write(command);
  delay(JOG_DURATION_MS);
  stopMotor();
  Serial.println(F("Stop command sent."));
}

void printStatus() {
  unsigned long pulseUs =
      pulseIn(RECEIVER_CH4_PIN, HIGH, RECEIVER_TIMEOUT_US);

  int feedbackADC = analogRead(FEEDBACK_PIN);

  Serial.print(F("CH4 pulse: "));
  if (pulseUs == 0) {
    Serial.print(F("NO VALID SIGNAL"));
  } else {
    Serial.print(pulseUs);
    Serial.print(F(" us"));
  }

  Serial.print(F(" | PA-14P feedback ADC: "));
  Serial.println(feedbackADC);
}

void setup() {
  pinMode(RECEIVER_CH4_PIN, INPUT);

  Serial.begin(115200);
  syrenSerial.begin(9600);

  // Allow startup, then send the candidate neutral command.
  delay(1000);
  stopMotor();

  Serial.println();
  Serial.println(F("POC 7-1 pre-test jog sketch ready."));
  Serial.println(F("T8L -> ER8 CH4 -> Arduino D4."));
  Serial.println(F("PA-14P feedback -> Arduino A0."));
  Serial.println(F("Arduino D8 TX -> SyRen S1."));
  Serial.println(F("Commands: a = jog A, b = jog B, s = stop."));
  Serial.println(F("Motor jogs are limited to 250 ms."));
  Serial.println(F("SyRen command mapping is not yet verified."));
}

void loop() {
  // Manual jog commands from Serial Monitor.
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 'a' || cmd == 'A') {
      jogMotor(JOG_A);
    } else if (cmd == 'b' || cmd == 'B') {
      jogMotor(JOG_B);
    } else if (cmd == 's' || cmd == 'S') {
      stopMotor();
      Serial.println(F("Stop command sent."));
    }
  }

  // Report receiver and potentiometer readings.
  if (millis() - lastStatusMs >= STATUS_INTERVAL_MS) {
    lastStatusMs = millis();
    printStatus();
  }
}
