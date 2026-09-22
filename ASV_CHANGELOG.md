# ASV Project Changelog

Chronological history of notable changes, discoveries, decisions, and project milestones for the Autonomous Surface Vessel (ASV) project.

> **Source of truth split:** `ASV_Master_Context.md` contains the current project state, architecture, inventory, decisions, open questions, and operational lessons. This file contains the detailed historical record.

---

## 2026-09-17

- Created a dedicated **`sketches/`** directory in the repository for Arduino/ASV POC sketches.
- Added **`sketches/README.md`** documenting the intended organization and incremental POC approach.
- Restructured project documentation so the Master Context remains focused on current state while historical changes are maintained separately in this changelog.
- Created **`sketches/01_basics/`** as the first Arduino sketch group.
- Added `01_blink.ino` to establish basic Arduino program structure and digital output.
- Added `02_serial_test.ino` to establish basic Serial communication.
- Added `03_servo_sweep.ino` to establish basic servo output using the Arduino Servo library.
- Corrected the Arduino sketch layout so each `.ino` file is inside a same-named sketch folder, as required by Arduino IDE.
- Current Arduino sketch paths are `01_blink/01_blink.ino`, `02_serial_test/02_serial_test.ino`, and `03_servo_sweep/03_servo_sweep.ino` under `sketches/01_basics/`.
- These sketches are intentionally hardware-light and can be written before the USB-C-to-USB-B cable and female-ended connectors arrive.

## 2026-09-16

- Created the initial **ASV Master Context**.
- Established the document as the working source of truth for the project.
- Recorded known hull specifications.
- Recorded current RC/Arduino architecture.
- Recorded known transmitter, receiver, Arduino, servo, actuator, connector, battery, wiring, and tool inventory.
- Established the incremental POC approach.
- Separated T8L transmitter power from future boat/propulsion power.

---

## Historical lessons captured in Master Context

Known recommendation and verification failures are intentionally kept in `ASV_Master_Context.md` under **Previous Mistakes / Lessons Learned** because they are operational safeguards for future decisions, rather than merely historical events.

### BOJACK 130-piece breadboard kit / connector mismatch

The BOJACK 130-piece breadboard jumper-wire kit was recommended without verifying its actual contents against the required physical connections. It contains male-to-male jumper wires, while the ER8 exposes male header pins and therefore requires female-ended wiring for a direct connection.

**Prevention:** Verify exact kit contents, connector gender, pin pitch, physical interfaces, and the complete no-solder end-to-end connection before recommending wiring components.

### Hohm LIFE⁴ / Radiomaster T8L battery compatibility

The Hohm LIFE⁴ 18650 cells were recommended for the Radiomaster T8L without adequately verifying the transmitter's exact battery-voltage requirements against the cell's specified 3.6/3.65 V nominal rating.

**Prevention:** Verify chemistry, nominal voltage, full-charge voltage, dimensions, protection/configuration, manufacturer-supported battery types, connector/polarity, and charging architecture. Do not assume 3.6 V and 3.7 V nominal cells are interchangeable without checking the device requirements.


## 2026-09-21

- Completed the receiver-signal POC using the Radiomaster T8L and ER8.
- Confirmed the T8L and ER8 are successfully bound and communicating.
- Confirmed **ER8 CH1 maps directly to the T8L right joystick X axis**, making CH1 the planned steering input without rebinding.
- Measured CH1 PWM steering signal at approximately **984 µs minimum, 1493–1496 µs center, and 2001–2002 µs maximum**.
- Confirmed the ER8 is powered from the NANKADF GC3010H bench supply while the Arduino Uno remains powered solely by USB.
- Confirmed ER8 ground, Arduino GND, and bench-supply negative share a common ground.
- Created `sketches/01_basics/05_t8l_er8_servo/05_t8l_er8_servo.ino` for the next integrated POC.
- POC 5 will use CH1 for steering, initially apply a 30 µs center dead zone, command the HiTec D646WP from Arduino D9, and read a second receiver channel for throttle without controlling propulsion hardware.


## 2026-09-21

- Completed **POC 4 — ER8 receiver signal visibility / channel characterization**.
- Experimentally mapped all eight ER8 PWM outputs to the T8L controls:
  - CH1 = Right Joystick X
  - CH2 = Right Joystick Y
  - CH3 = Left Joystick Y
  - CH4 = Left Joystick X
  - CH5 = SE toggle
  - CH6 = SA hold-to-press bumper
  - CH7 = SB three-position switch
  - CH8 = SC three-position switch
- Confirmed CH1 steering pulse range of approximately 984–2002 µs with a center around 1489–1496 µs.
- Confirmed CH3 is a continuously variable left-joystick Y input and does not self-center.
- Completed **POC 5 Stage 1** by replacing sequential `pulseIn()` reads with interrupt-based PWM capture so CH1 and CH3 can be observed simultaneously.
- Completed **POC 5 Stage 2** by mapping CH1 steering to the HiTec D646WP on Arduino D9 while continuing to monitor CH3 throttle over Serial.
- Implemented a ±30 µs steering dead zone around the nominal 1493 µs CH1 center.
- Used a conservative initial D646WP command range of 60°–120°.
- Verified the complete working chain: **T8L → ER8 → Arduino → D646WP**, with throttle simultaneously visible on Serial.
- Confirmed Arduino D2 and D3 are both suitable interrupt-capable PWM inputs; the earlier apparent D3 failure was caused by blocking sequential `pulseIn()` calls, not a bad Arduino pin.
- Established ER8 BATT as the preferred dedicated receiver power connection for future wiring; RX/TX and EXT-V remain unused.
- POC 5 is now **COMPLETE**.
