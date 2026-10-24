/*
  POC 7-2 — PA-14P center-position testing

  Purpose:
    Command the actuator toward a manually adjustable feedback ADC target.
    Initial target is ADC 363. In the Arduino Serial Monitor:
      a = decrease target by 1 ADC count
      d = increase target by 1 ADC count

  Existing wiring is unchanged from POC 7:
    PA-14P feedback pot: yellow -> Arduino 5V,
                         white -> Arduino GND,
                         blue wiper -> Arduino A0
    Arduino D8 (SoftwareSerial TX) -> SyRen 10 S1
    Arduino GND and SyRen GND share a common reference
    Arduino powered by USB; ER8 remains powered from Arduino 5V/GND.
    SyRen BEC positive remains disconnected from ER8.
    SyRen M1/M2 remain connected to PA-14P motor leads.

  Control behavior:
    - No ADC position deadband: any reading below target commands forward,
      any reading above target commands reverse, exact equality commands stop.
    - This can cause repeated direction changes / hunting due to ADC noise.
    - The existing actuator endpoint switches reportedly stop the motor at
      both physical ends. Still supervise closely and remove 12 V power if
      behavior is unexpected.
*/

#include <SoftwareSerial.h>

// Existing SyRen serial wiring: D8 -> S1. D7 RX is unused.
const uint8_t SYREN_RX_UNUSED_PIN = 7;
const uint8_t SYREN_TX_PIN = 8;
const uint8_t FEEDBACK_PIN = A0;

SoftwareSerial syrenSerial(SYREN_RX_UNUSED_PIN, SYREN_TX_PIN);

// Same empirically tested command values as POC 7.
const uint8_t SYREN_REVERSE_CMD = 33;
const uint8_t SYREN_STOP_CMD = 127;
const uint8_t SYREN_FORWARD_CMD = 223;

const unsigned long LOOP_DELAY_MS = 20;
const unsigned long STATUS_INTERVAL_MS = 250;
const bool ENABLE_MOTOR_OUTPUT = true;

int targetAdc = 363;
unsigned long lastStatusMs = 0;

int readFeedbackAdc() {
  // Preserve the existing four-sample average to reduce single-read noise.
  long total = 0;
  for (uint8_t i = 0; i < 4; i++) {
    total += analogRead(FEEDBACK_PIN);
    delay(2);
  }
  return (int)(total / 4);
}

void commandTowardTarget(int actualAdc) {
  if (!ENABLE_MOTOR_OUTPUT) return;

  // Deliberately no ADC deadband, per request.
  if (actualAdc < targetAdc) {
    syrenSerial.write(SYREN_FORWARD_CMD);
  } else if (actualAdc > targetAdc) {
    syrenSerial.write(SYREN_REVERSE_CMD);
  } else {
    syrenSerial.write(SYREN_STOP_CMD);
  }
}

void handleSerialInput() {
  while (Serial.available() > 0) {
    char key = (char)Serial.read();
    if (key == 'a' || key == 'A') {
      if (targetAdc > 0) targetAdc--;
      Serial.print(F("Target ADC: "));
      Serial.println(targetAdc);
    } else if (key == 'd' || key == 'D') {
      if (targetAdc < 1023) targetAdc++;
      Serial.print(F("Target ADC: "));
      Serial.println(targetAdc);
    }
  }
}

void setup() {
  pinMode(SYREN_TX_PIN, OUTPUT);
  digitalWrite(SYREN_TX_PIN, HIGH); // UART idle state

  Serial.begin(115200);
  syrenSerial.begin(9600);

  Serial.println(F("\nPOC 7-2 — PA-14P center-position testing"));
  Serial.println(F("Initial target ADC: 363"));
  Serial.println(F("Press 'a' to lower target by 1; 'd' to raise target by 1."));
  Serial.println(F("No ADC feedback deadband is applied."));
}

void loop() {
  handleSerialInput();

  int feedbackAdc = readFeedbackAdc();
  commandTowardTarget(feedbackAdc);

  if (millis() - lastStatusMs >= STATUS_INTERVAL_MS) {
    lastStatusMs = millis();
    Serial.print(F("feedback ADC="));
    Serial.print(feedbackAdc);
    Serial.print(F(" | target ADC="));
    Serial.print(targetAdc);
    Serial.print(F(" | motor output="));
    Serial.println(ENABLE_MOTOR_OUTPUT ? F("ENABLED") : F("DISABLED"));
  }

  delay(LOOP_DELAY_MS);
}
