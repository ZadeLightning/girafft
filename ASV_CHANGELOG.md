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


## 2026-09-29

- Began **Step 1 — Design** of the hardware-focused RC boat architecture.
- User identified the hull's existing **underside finbox and supplied SUP-style fin** as potentially useful for fixed lateral stabilization.
- Photos documented an existing **stern/transom metal bracket/mounting assembly** whose original purpose and load capacity are not yet verified.
- Added **Option C — electric trolling motor** as an explicit propulsion/steering architecture candidate.
- Rationale for investigating Option C:
  - The naval architect who sold the hull suggested an electric trolling motor.
  - A trolling motor can package motor, propeller, shaft, and speed-control hardware into one propulsion assembly.
  - A steerable trolling motor could eliminate the need for a separate rudder and rudder linkage.
  - The existing finbox/SUP fin may provide fixed lateral stability while the propulsion unit provides directional thrust.
- No trolling motor has been selected or purchased. Required next checks include mounting geometry, finbox/fin dimensions, propeller depth, shaft length, thrust requirement, control interface, steering mechanism, battery requirements, and compatibility with the existing Arduino architecture.


## 2026-09-29 — Trolling-motor control requirements and screening criteria

- Saved the user's initial Option C requirements: saltwater friendly, lightweight, investigation of mechanical servo control, direct Arduino/electrical control, and the motor's own remote-control architecture, plus potential remote steering via a servo-actuated tiller or electronic steering interface.
- Added screening criteria covering voltage, thrust versus actual hull displacement, maximum current draw, battery/wiring implications, shaft length, propeller depth, mounting compatibility, steering authority, throttle/forward/reverse behavior, stop/failsafe behavior, marine serviceability, proprietary-interface risk, reverse-engineering complexity, and final-boat suitability.
- Established C1–C4 as investigation categories rather than decisions: mechanical tiller actuation, direct electrical interface, motor's own wireless remote, and custom/third-party RC interface.
- Began online investigation of commercially available saltwater trolling motors and their control interfaces. No motor selected or purchased.


## 2026-09-29 — Refined trolling-motor direction

- User clarified that mechanically hacking the trolling motor throttle should generally be avoided; steering/tiller actuation alone remains acceptable.
- User is opposed to deep reverse engineering of proprietary motor controls unless the interface is unusually well documented and robust.
- User considers $1,000+ electronic-steering motors outside the budget; approximately $600 is already a stretch rather than a target.
- User wants to retain the Arduino Uno as part of the boat control architecture where practical.
- Manual override/failsafe remains a required future design topic but is intentionally tabled for now.
- Added a promising sub-architecture to investigate: Arduino-controlled steering servo plus a separate electronic high-current motor controller for propulsion, potentially avoiding mechanical throttle hacking while retaining the Arduino.
- Newport's documented external PWM approach is noted as a lead, but Arduino compatibility and high-current implementation still require verification.


## 2026-10-01 — Alternative brushless underwater-thruster path

- Opened a parallel propulsion investigation for inexpensive **brushless underwater thrusters** aimed at ROV/USV/kayak/unmanned-boat applications.
- Confirmed that the architecture can preserve the desired **T8L → ER8 → Arduino** chain by using an external brushless ESC with a documented RC-style PWM input and a separate servo-operated rudder.
- Identified the **APISQUEEN U10** as a particularly relevant candidate: 24 V, 900 W, 10 kg stated thrust, 40 A stated load current, approximately 800 g in air, sea/fresh-water operation, and a documented bidirectional 100 A ESC with standard 1–2 ms PWM-style control.
- Identified the **APISQUEEN S10** as another candidate: 24 V, 1000 W, 8.5 kg stated thrust, pump-jet configuration, with a thruster-only and complete control-box/remote package available.
- Retained the **Blue Robotics T200 + Basic ESC** as a well-documented benchmark architecture: 7–20 V, up to 32 A / 645 W at 20 V, approximately 6.7 kgf forward thrust at 20 V, and explicitly documented Arduino/PWM control.
- User is comfortable designing and fabricating a custom propulsion mount and potentially a dedicated rudder if the thruster architecture is selected.
- No propulsion hardware has been selected or purchased. The conventional trolling-motor path remains equally active.
- New screening emphasis: actual thrust/current data, documented Arduino-compatible ESC input, marine suitability, mounting load path, rudder feasibility, battery/fusing requirements, and total delivered cost.
