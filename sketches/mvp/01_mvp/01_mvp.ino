/*
  Girraft USV manual MVP — first integrated sketch

  Receiver inputs:
    T8L right joystick Y / ER8 CH2 -> Arduino D2 (external interrupt)
    T8L left joystick X  / ER8 CH4 -> Arduino D4 (pin-change interrupt)

  Outputs / feedback:
    Arduino D11 -> APISQUEEN ESC signal -> U10 propulsion
    Arduino TX1 / D1 -> SyRen S1 (hardware UART, 9600 baud)
    PA-14P potentiometer wiper -> Arduino A0

  Power:
    ESC BEC 5.5 V -> XL6019 boost module adjusted to 8.0 V -> Uno VIN
    Uno 5 V rail -> ER8 and PA-14P potentiometer only
    PA-14P motor -> SyRen 10, powered by its separate actuator supply
    Establish a common signal reference between Arduino, ER8, ESC, and SyRen.

  Safety behavior:
    - ESC starts at 1500 us neutral and stays disarmed until CH2 is centered.
    - Throttle output spans 1000-2000 us, with 1500 us neutral.
    - Missing, stale, or out-of-range CH2 or CH4 commands ESC neutral,
      disarms throttle, and sends SyRen stop.
    - Invalid PA-14P feedback commands SyRen stop, ESC neutral, and throttle
      disarm.
    - Receiver loss stops actuator movement at its current position; it does
      not send the steering target back to center.
    - When valid steering returns, CH4 resumes the POC 7 position target map.

  IMPORTANT:
    - This sketch uses D1/TX1 for SyRen serial. Disconnect the SyRen S1 signal
      (or remove actuator motor power) while uploading over USB; bootloader
      serial traffic shares this pin.
    - The Arduino can only detect RF loss if the ER8 stops producing PWM or
      produces invalid pulses. If the ER8 outputs valid stored failsafe pulses,
      configure its failsafe behavior accordingly; valid held pulses look like
      live transmitter commands to this sketch.
    - Keep the U10 submerged and mechanically restrained during powered tests.
    - No steering servo is used in the boat.
*/

#include <Servo.h>
#include <avr/interrupt.h>

// ----- Pin allocation -----
const uint8_t THROTTLE_INPUT_PIN = 2;  // ER8 CH2; ATmega328P INT0 / PD2
const uint8_t STEERING_INPUT_PIN = 4;  // ER8 CH4; ATmega328P PCINT20 / PD4
const uint8_t ESC_OUTPUT_PIN = 11;    // Servo library output to U10 ESC
const uint8_t FEEDBACK_PIN = A0;      // PA-14P potentiometer wiper
const uint8_t STATUS_LED_PIN = LED_BUILTIN;

// TX1 is D1 on the Uno R3. Hardware UART avoids SoftwareSerial interrupt
// timing interference while both receiver channels are being captured.
const unsigned long SYREN_BAUD = 9600;

// ----- Receiver calibration -----
// CH2 calibration from POC 6 / prior T8L + ER8 observations.
const int THROTTLE_MIN_US = 984;
const int THROTTLE_CENTER_US = 1493;
const int THROTTLE_MAX_US = 2002;
const int THROTTLE_ARM_DEADBAND_US = 30;

// CH4 position mapping retained from POC 7.
const int STEERING_MIN_US = 1000;
const int STEERING_CENTER_US = 1500;
const int STEERING_MAX_US = 2000;
const int STEERING_CENTER_DEADBAND_US = 30;

const int RX_VALID_MIN_US = 900;
const int RX_VALID_MAX_US = 2100;
const unsigned long RX_TIMEOUT_US = 100000UL;

// ----- U10 ESC calibration -----
const int ESC_NEUTRAL_US = 1500;
const int ESC_REVERSE_LIMIT_US = 1000;
const int ESC_FORWARD_LIMIT_US = 2000;

// ----- PA-14P / SyRen settings from the working POC 7 sketch -----
const int FEEDBACK_RETRACTED_ADC = 0;
const int FEEDBACK_EXTENDED_ADC = 900;
int steeringCenterTargetAdc = 366;  // Fine-tune this value after integration.
const int POSITION_DEADBAND_ADC = 30;

const uint8_t SYREN_REVERSE_CMD = 33;
const uint8_t SYREN_STOP_CMD = 127;
const uint8_t SYREN_FORWARD_CMD = 223;

// ----- Control timing -----
const unsigned long CONTROL_INTERVAL_MS = 20;
const unsigned long STATUS_BLINK_INTERVAL_MS = 250;
const unsigned long STATUS_FAULT_BLINK_INTERVAL_MS = 100;

Servo esc;

// CH2 pulse capture. D2 is PD2 / INT0.
volatile uint32_t throttleRiseTimeUs = 0;
volatile uint16_t throttlePulseWidthUs = 0;
volatile uint32_t throttleLastPulseTimeUs = 0;
volatile bool throttleInputHigh = false;
volatile bool throttleHasPulse = false;

// CH4 pulse capture. D4 is PD4 / PCINT20.
volatile uint32_t steeringRiseTimeUs = 0;
volatile uint16_t steeringPulseWidthUs = 0;
volatile uint32_t steeringLastPulseTimeUs = 0;
volatile bool steeringInputHigh = false;
volatile bool steeringHasPulse = false;
volatile uint8_t previousPortD = 0;

bool throttleArmed = false;
unsigned long lastControlUpdateMs = 0;
unsigned long lastStatusBlinkMs = 0;
bool statusLedState = false;

struct ReceiverSnapshot {
  uint16_t throttlePulseUs;
  uint16_t steeringPulseUs;
  uint32_t throttleLastPulseUs;
  uint32_t steeringLastPulseUs;
  bool throttleHasPulse;
  bool steeringHasPulse;
};

void throttleEdgeISR() {
  const uint32_t nowUs = micros();
  const bool isHigh = (PIND & _BV(PD2)) != 0;

  if (isHigh) {
    throttleRiseTimeUs = nowUs;
    throttleInputHigh = true;
  } else if (throttleInputHigh) {
    const uint32_t widthUs = nowUs - throttleRiseTimeUs;
    throttlePulseWidthUs = (widthUs <= 65535UL) ? (uint16_t)widthUs : 0;
    throttleLastPulseTimeUs = nowUs;
    throttleHasPulse = true;
    throttleInputHigh = false;
  }
}

ISR(PCINT2_vect) {
  const uint8_t portD = PIND;
  const bool wasHigh = (previousPortD & _BV(PD4)) != 0;
  const bool isHigh = (portD & _BV(PD4)) != 0;
  previousPortD = portD;

  if (isHigh && !wasHigh) {
    steeringRiseTimeUs = micros();
    steeringInputHigh = true;
  } else if (!isHigh && wasHigh && steeringInputHigh) {
    const uint32_t nowUs = micros();
    const uint32_t widthUs = nowUs - steeringRiseTimeUs;
    steeringPulseWidthUs = (widthUs <= 65535UL) ? (uint16_t)widthUs : 0;
    steeringLastPulseTimeUs = nowUs;
    steeringHasPulse = true;
    steeringInputHigh = false;
  }
}

ReceiverSnapshot readReceiverSnapshot() {
  ReceiverSnapshot snapshot;
  noInterrupts();
  snapshot.throttlePulseUs = throttlePulseWidthUs;
  snapshot.steeringPulseUs = steeringPulseWidthUs;
  snapshot.throttleLastPulseUs = throttleLastPulseTimeUs;
  snapshot.steeringLastPulseUs = steeringLastPulseTimeUs;
  snapshot.throttleHasPulse = throttleHasPulse;
  snapshot.steeringHasPulse = steeringHasPulse;
  interrupts();
  return snapshot;
}

bool pulseIsFreshAndValid(uint16_t pulseUs,
                          uint32_t lastPulseUs,
                          bool hasPulse,
                          uint32_t nowUs) {
  return hasPulse &&
         pulseUs >= RX_VALID_MIN_US &&
         pulseUs <= RX_VALID_MAX_US &&
         (uint32_t)(nowUs - lastPulseUs) <= RX_TIMEOUT_US;
}

bool feedbackCalibrationPlausible() {
  return FEEDBACK_RETRACTED_ADC >= 0 &&
         FEEDBACK_RETRACTED_ADC <= 1022 &&
         FEEDBACK_EXTENDED_ADC >= 1 &&
         FEEDBACK_EXTENDED_ADC <= 1022 &&
         abs(FEEDBACK_EXTENDED_ADC - FEEDBACK_RETRACTED_ADC) >= 100;
}

int readFeedbackAdc() {
  long total = 0;
  for (uint8_t i = 0; i < 4; i++) {
    total += analogRead(FEEDBACK_PIN);
    delay(2);
  }
  return (int)(total / 4);
}

bool feedbackIsValid(int feedbackAdc) {
  return feedbackCalibrationPlausible() &&
         feedbackAdc > 2 &&
         feedbackAdc < 1021;
}

int pulseToSteeringTargetAdc(uint16_t pulseUs) {
  const long boundedPulse = constrain((long)pulseUs,
                                      (long)STEERING_MIN_US,
                                      (long)STEERING_MAX_US);
  const long lowDeadbandEdge =
      (long)STEERING_CENTER_US - STEERING_CENTER_DEADBAND_US;
  const long highDeadbandEdge =
      (long)STEERING_CENTER_US + STEERING_CENTER_DEADBAND_US;

  if (boundedPulse >= lowDeadbandEdge &&
      boundedPulse <= highDeadbandEdge) {
    return steeringCenterTargetAdc;
  }

  if (boundedPulse < lowDeadbandEdge) {
    const long inputSpan = lowDeadbandEdge - STEERING_MIN_US;
    const long inputOffset = boundedPulse - STEERING_MIN_US;
    const long targetSpan =
        (long)steeringCenterTargetAdc - FEEDBACK_RETRACTED_ADC;
    return (int)(FEEDBACK_RETRACTED_ADC + targetSpan * inputOffset / inputSpan);
  }

  const long inputSpan = STEERING_MAX_US - highDeadbandEdge;
  const long inputOffset = boundedPulse - highDeadbandEdge;
  const long targetSpan =
      (long)FEEDBACK_EXTENDED_ADC - steeringCenterTargetAdc;
  return (int)(steeringCenterTargetAdc + targetSpan * inputOffset / inputSpan);
}

int throttlePulseToEscCommand(uint16_t pulseUs) {
  if (abs((int)pulseUs - THROTTLE_CENTER_US) <=
      THROTTLE_ARM_DEADBAND_US) {
    return ESC_NEUTRAL_US;
  }

  const long boundedPulse = constrain((long)pulseUs,
                                      (long)THROTTLE_MIN_US,
                                      (long)THROTTLE_MAX_US);
  const long command = map(boundedPulse,
                           THROTTLE_MIN_US,
                           THROTTLE_MAX_US,
                           ESC_REVERSE_LIMIT_US,
                           ESC_FORWARD_LIMIT_US);
  return (int)constrain(command,
                        (long)ESC_REVERSE_LIMIT_US,
                        (long)ESC_FORWARD_LIMIT_US);
}

void sendSyRenStop() {
  Serial.write(SYREN_STOP_CMD);
}

void commandSyRenTowardTarget(int targetAdc, int actualAdc) {
  const int error = targetAdc - actualAdc;
  if (abs(error) <= POSITION_DEADBAND_ADC) {
    sendSyRenStop();
  } else if (error > 0) {
    Serial.write(SYREN_FORWARD_CMD);
  } else {
    Serial.write(SYREN_REVERSE_CMD);
  }
}

void updateStatusLed(bool receiverValid,
                     bool feedbackValid,
                     bool throttleIsArmed) {
  if (receiverValid && feedbackValid && throttleIsArmed) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    return;
  }

  const unsigned long intervalMs =
      (receiverValid && !feedbackValid)
          ? STATUS_FAULT_BLINK_INTERVAL_MS
          : STATUS_BLINK_INTERVAL_MS;

  if (millis() - lastStatusBlinkMs >= intervalMs) {
    lastStatusBlinkMs = millis();
    statusLedState = !statusLedState;
    digitalWrite(STATUS_LED_PIN, statusLedState ? HIGH : LOW);
  }
}

void setup() {
  // Put propulsion at neutral and actuator control at stop as early as possible.
  esc.attach(ESC_OUTPUT_PIN);
  esc.writeMicroseconds(ESC_NEUTRAL_US);

  Serial.begin(SYREN_BAUD);
  sendSyRenStop();

  pinMode(THROTTLE_INPUT_PIN, INPUT);
  pinMode(STEERING_INPUT_PIN, INPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(THROTTLE_INPUT_PIN),
                  throttleEdgeISR,
                  CHANGE);

  // D4 is PCINT20. Capture it with the Port D pin-change interrupt group.
  previousPortD = PIND;
  PCIFR = _BV(PCIF2);          // Clear any pending Port D pin-change flag.
  PCMSK2 |= _BV(PCINT20);     // Enable only D4 / PCINT20.
  PCICR |= _BV(PCIE2);        // Enable pin-change interrupts for Port D.

  lastControlUpdateMs = millis();
}

void loop() {
  if (millis() - lastControlUpdateMs < CONTROL_INTERVAL_MS) {
    return;
  }
  lastControlUpdateMs = millis();

  const ReceiverSnapshot rx = readReceiverSnapshot();
  const uint32_t nowUs = micros();
  const bool throttleValid = pulseIsFreshAndValid(rx.throttlePulseUs,
                                                  rx.throttleLastPulseUs,
                                                  rx.throttleHasPulse,
                                                  nowUs);
  const bool steeringValid = pulseIsFreshAndValid(rx.steeringPulseUs,
                                                  rx.steeringLastPulseUs,
                                                  rx.steeringHasPulse,
                                                  nowUs);
  const bool receiverValid = throttleValid && steeringValid;

  if (!receiverValid) {
    throttleArmed = false;
    esc.writeMicroseconds(ESC_NEUTRAL_US);
    sendSyRenStop();
    updateStatusLed(false, true, false);
    return;
  }

  const int feedbackAdc = readFeedbackAdc();
  const bool feedbackValid = feedbackIsValid(feedbackAdc);

  if (!feedbackValid) {
    sendSyRenStop();
    esc.writeMicroseconds(ESC_NEUTRAL_US);
    throttleArmed = false;
    updateStatusLed(receiverValid, false, false);
    return;
  } else {
    const int targetAdc = pulseToSteeringTargetAdc(rx.steeringPulseUs);
    commandSyRenTowardTarget(targetAdc, feedbackAdc);
  }

  if (!throttleArmed) {
    esc.writeMicroseconds(ESC_NEUTRAL_US);
    if (abs((int)rx.throttlePulseUs - THROTTLE_CENTER_US) <=
        THROTTLE_ARM_DEADBAND_US) {
      throttleArmed = true;
    }
  } else {
    const int escCommand = throttlePulseToEscCommand(rx.throttlePulseUs);
    esc.writeMicroseconds(escCommand);
  }

  updateStatusLed(receiverValid, feedbackValid, throttleArmed);
}
