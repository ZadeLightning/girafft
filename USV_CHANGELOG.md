# USV Project Changelog

## 2026-11-04 — Project terminology updated

- Standardized the project term to **USV (Unmanned Surface Vehicle)** because Girraft is currently manually controlled, not autonomous.
- Renamed the master context and power-system documents to use `USV` in their filenames; updated related references.
- Autonomy remains a long-term possibility and is not a current capability claim.

## 2026-10-22 — SyRen regeneration manual review / converter selection gate

- Read the manufacturer's SyRen 10/25 User's Guide, specifically page 18's packetized-serial **Max Voltage** section.
- Confirmed the manual explicitly warns that a power supply unable to sink current can experience a rising input voltage during regeneration. The SyRen can hard-brake above a configured voltage limit as a fallback, but the manual describes that behavior as inefficient; it says a battery handles the issue under the described conditions.
- The manual does not explicitly validate a generic DC-DC buck converter connected to the SyRen input. A 24 V-to-12 V converter must therefore remain an unapproved candidate until its reverse-current/transient behavior is documented or a correctly engineered energy-absorption path is designed.
- Reaffirmed that a battery-plus-supply arrangement must not be improvised: the manufacturer's specific ATX supply guidance does not automatically validate the NANKADF GC3010H or the final onboard power architecture.
- No converter selected or purchased. Next step is to obtain a manufacturer-backed answer for candidate converter behavior or investigate a suitable battery-compatible architecture.


Chronological history of notable changes, discoveries, decisions, and project milestones for the Unmanned Surface Vehicle (USV) project.

> **Source of truth split:** `USV_Master_Context.md` contains the current project state, architecture, inventory, decisions, open questions, and operational lessons. This file contains the detailed historical record.

---

## 2026-09-17

- Created a dedicated **`sketches/`** directory in the repository for Arduino/USV POC sketches.
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

- Created the initial **USV Master Context**.
- Established the document as the working source of truth for the project.
- Recorded known hull specifications.
- Recorded current RC/Arduino architecture.
- Recorded known transmitter, receiver, Arduino, servo, actuator, connector, battery, wiring, and tool inventory.
- Established the incremental POC approach.
- Separated T8L transmitter power from future boat/propulsion power.

---

## Historical lessons captured in Master Context

Known recommendation and verification failures are intentionally kept in `USV_Master_Context.md` under **Previous Mistakes / Lessons Learned** because they are operational safeguards for future decisions, rather than merely historical events.

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


## 2026-10-01 — Brushless-thruster path strengthened; hull-weight baseline

- User supplied an empty-hull baseline of approximately **20 lb** for the shell alone. Future rough propulsion/stability calculations should start from this number and add the actual fin, rudder, thruster, battery, electronics, enclosure, wiring, and mounting masses as they are characterized.
- Follow-up research confirmed that the APISQUEEN U10's bidirectional 100 A ESC accepts a standard RC-style PWM command: approximately 1.0 ms reverse, 1.5 ms stop, and 2.0 ms forward, with 50/100/200/500 Hz control frequencies documented. This is a strong fit for direct Arduino control and avoids proprietary protocol reverse engineering.
- Confirmed U10 published specifications: 24 V, 900 W, 40 A load current, 10 kg / 22 lb stated thrust, approximately 800 g in air and 350 g in water, M3 mounting with 20 × 16 mm spacing, and freshwater/seawater operation.
- Identified an important integration constraint: the U10 ESC's listed **5 V / 1 A BEC should not be assumed sufficient for the HiTec D646WP**, whose documented stall current is 2.2 A at 7.4 V. Servo power therefore remains a separate design item.
- User prefers the potential low, underwater thruster placement because it minimizes topside mass and may help avoid a high center of gravity/topside weight that could contribute to tipping. This is a design hypothesis; actual stability will depend on the complete center-of-gravity/center-of-buoyancy relationship and hull geometry.
- User is comfortable fabricating the thruster mount and is open to bottom-of-hull fiberglass work. Existing stern brackets may also be useful for a rudder post.
- The brushless-thruster architecture is now considered a **leading candidate**, but no architecture has been selected and the conventional trolling-motor path remains active.
- Next technical question: establish whether the U10's real thrust/current performance is appropriate for this specific small hull and realistic all-up displacement, rather than relying on marketing propellable-weight claims.


## 2026-10-01 — Path #1 and U10 propulsion sizing research

- User designated the inexpensive brushless underwater-thruster architecture as **Path #1** for the current investigation. The conventional trolling-motor path remains active as an alternative.
- User confirmed the empty hull shell weighs approximately **20 lb**; this is now the baseline for rough propulsion/stability calculations.
- Found a published U10 full-throttle test table showing forward thrust/current of 4.6 kg/16 A at 14 V, 5.6 kg/17 A at 16 V, 6.9 kg/20 A at 18 V, 8.1 kg/25 A at 20 V, 9.1 kg/27 A at 22 V, and 10.4 kg/32 A at 24 V. Reverse thrust is also documented but is lower.
- The U10 therefore produces approximately 10.1–22.9 lb of forward static thrust across 14–24 V in the published test data.
- Initial interpretation: the U10 appears to have substantial propulsion headroom for a small ~20 lb empty hull plus moderate equipment mass, but actual required thrust cannot yet be determined without waterline geometry, all-up displacement, hull form, and target speed.
- Rough geometric sanity check using 5 ft 8 in as waterline length, 20 in as waterline beam, and Cb≈0.4 gives an approximate draft of 6.3 cm at 50 lb all-up and 8.8 cm at 70 lb. These are preliminary estimates only.
- A classical displacement-hull speed estimate using 5 ft 8 in waterline length is approximately 3.2 knots / 5.9 km/h. This is a rule of thumb, not a hard speed limit.
- Explained **BEC (Battery Eliminator Circuit)**: a regulated low-voltage supply built into an ESC. The U10 ESC's listed BEC is 5 V / 1 A and should not be assumed sufficient for the 7.4 V, 2.2 A-stall-current HiTec D646WP servo.
- No propulsion purchase made. Next sizing inputs are actual waterline dimensions, draft/displacement at representative all-up loads, and target speed.


## 2026-10-01 — U10 purchase plan and propulsion POC

- User decided to purchase the **APISQUEEN U10** and the **APISQUEEN 2–6S 100 A Feather ESC, waterproof variant with 5.5 V / 3 A BEC** as the current single-thruster hardware candidates.
- Defined the next hardware POC as **Arduino-controlled U10 propulsion**: T8L → ER8 → Arduino → PWM → ESC → U10.
- Initial POC goals are safe ESC arming, neutral/forward/reverse behavior, PWM characterization, current measurement, ESC thermal observation, and sustained-load testing before hull integration.
- Confirmed the inventory **NANKADF GC3010H 0–30 V / 0–10 A bench supply is not sufficient for a full-power U10 test**. Published U10 data reaches 32 A at 24 V, and APISQUEEN's current guide identifies 32 A as maximum working current. The bench supply remains useful for low-power/low-throttle testing but cannot reproduce maximum-load operation.
- Full-power propulsion testing will require a suitable approximately 24 V source capable of roughly 35–40 A continuous, with appropriate fusing/wiring, plus a safe water test setup because the U10 is an underwater thruster.


## 2026-10-01 — U10 exact specification baseline and mounting hardware

- User supplied a U10 specification table and it has been recorded in USV_Master_Context.md as the exact current baseline for the U10 under consideration.
- Recorded: **117.5 × 95.5 mm**, **24 V / 6S LiPo**, **1.9 A no-load**, **40 A load current**, **300 KV**, **900 W**, **10 kg thrust at 24 V**, **100 A continuous ESC**, **5 V / 1 A BEC**, **9 mm wire diameter**, **M3 mounting holes**, **20 × 16 mm mounting-hole spacing**, **800 g in air / 350 g in water**, and fresh-/sea-water operation.
- Clarified that M3 mounting hardware should be readily available; final screw length depends on the fabricated mount/plate thickness and required thread engagement.
- Recorded a design caution not to interpret the 20 × 16 mm value as hole diameter; treat it as mounting-hole spacing/layout until the physical thruster is inspected.


## 2026-10-21 — Full electrical system diagram and power TODOs

- Added `diagrams/MVP_Full_Electrical_System.svg` as the latest connection-level full electrical system diagram. Power conversion, exact electrical ratings, wire sizing, and fusing are marked unresolved rather than assumed.
- Created [GitHub issue #1](https://github.com/ZadeLightning/girraft/issues/1) as the diagram-associated TODO list: onboard Arduino/ER8 power; 24 V lithium-ion battery choice and rough sizing/cost; 24 V to 12 V conversion for PA-14P/SyRen; 24 V to ~5 V logic/receiver power; and BEC/common-ground strategy.
- Updated `USV_Master_Context.md` to point to the diagram and checklist and capture the key BEC distinction: 5 V supplies power, 0 V/GND establishes the reference; do not parallel BEC outputs.


## 2026-10-21 — Electrical diagram workflow corrected

- Removed the rough generated SVG reconstruction after user review: it was visually poor, routed lines through boxes, and omitted detail from the original drawing.
- The user's original drawing, edited in their Google Drawings instance and uploaded when updated, remains the source of truth. Do not recreate the diagram unless explicitly asked.
- The five power-architecture TODOs remain tracked in [GitHub issue #1](https://github.com/ZadeLightning/girraft/issues/1).


## 2026-10-21 — Original full electrical system diagram uploaded

- User added the original PDF unchanged at [`diagrams/MVP Full Electrical System.pdf`](https://github.com/ZadeLightning/girraft/blob/main/diagrams/MVP%20Full%20Electrical%20System.pdf). This is the authoritative diagram; no SVG recreation is used.
- Master context and GitHub issue #1 link to the original PDF. User will edit the diagram in Google Drawings and upload new exports as needed.


## 2026-10-21 — TODO 1 BEC comparison

- Compared the Uno Rev3, ER8, APISQUEEN ESC BEC, and SyRen 10 BEC using saved project records and manufacturer documentation.
- Uno Rev3 operates at 5 V; Arduino recommends 7–12 V input via VIN/barrel and warns that 5 V pin injection bypasses its regulator. No single official board-current requirement is specified.
- ER8 input is 4.5–8.4 V; its current draw is not published in the reviewed manual/listing.
- APISQUEEN ESC BEC is 5.5 V / 3 A and is voltage-compatible with ER8. SyRen 10 BEC is limited to 10 mA when its input is above 12.6 V, so it should not power ER8 from the planned 24 V-class battery.
- Preliminary options recorded in master context: ESC BEC for ER8 plus a 24 V-to-7–9 V converter into Arduino VIN/barrel; or a suitable 24 V-to-regulated-5 V DC-DC rail for both logic devices. Do not parallel BEC positive outputs. No final wiring/purchase decision yet.

## 2026-10-22 — BEC current-capacity reasoning correction

- Added a lesson to **Previous Mistakes / Lessons Learned** in `USV_Master_Context.md`: I was too categorical in treating the SyRen 10's 100 mA low-input-voltage BEC rating as proven insufficient for the ER8 without an ER8 current specification or measurement.
- Corrected reasoning: 100 mA may or may not support the ER8/Arduino combination; actual load must be sourced or measured. At the planned >12.6 V / 24 V-class SyRen input, the documented 10 mA output is not an appropriate supply for the Arduino, and ER8 compatibility remains unverified.
- Prevention: distinguish source limits from load requirements, mark unknown current draw explicitly, measure where practical, and assess existing BEC options before recommending an extra converter or purchase.

## 2026-10-22 — Candidate onboard power architecture

- Recorded the user's candidate power plan in `USV_Master_Context.md`: 24 V battery directly to APISQUEEN ESC/U10; ESC's 5.5 V / 3 A BEC to ER8 only; 24 V-to-12 V DC-DC converter to the SyRen 10 and Arduino VIN.
- Corrected the diagram assumptions in the documentation: the ESC propulsion supply should remain on the main 24 V-class rail, while the PA-14P actuator requires a 12 V rail through the SyRen.
- Added open engineering checks: validate ESC input range against battery full-charge voltage, and verify that the 24-to-12 V converter can safely handle the SyRen's regenerative energy behavior. Architecture is provisional; no converter purchase or final wiring decision yet.


## 2026-10-22 — Scrutinized initial SyRen alternative screening

- Reaffirmed SyRen 10 as the incumbent rather than presuming replacement is preferable.
- Rejected Cytron MD10C for solving the regenerative-supply issue because its manufacturer explicitly specifies regenerative braking and says it lacks overcurrent protection.
- Flagged Pololu Simple Motor Controller G2 18v7 for further technical verification only: its 7 A continuous rating is nominally above the actuator's 5 A full-load rating, but actuator startup/stall current and exact regenerative-energy behavior/control fit remain unresolved.
- Recorded relay-based reversing as a possible simpler on/off architecture, with trade-offs and protection/interlock requirements.
- No replacement selected and no purchase authorized.


## 2026-10-22 — Record repeated unsupported actuator-current caveat

- Added a Previous Mistakes entry to `USV_Master_Context.md` documenting that repeatedly raising undocumented PA-14P startup/stall current as a generic blocker was unhelpful after the user clarified the limit of available evidence.
- Prevention: use documented specs, distinguish known facts from unknowns, and do not repeat this caveat unless a practical test becomes available and the result would materially change the decision.
- Also corrected the Pololu review: the previously linked 18v7 is an older generation not recommended for new designs. The current G2 family is the relevant reference; the appropriate 24 V-class models are the 24v12 or 24v19, which are relatively expensive and therefore not automatically attractive alternatives to the SyRen.


## 2026-10-22 — Mistake logged and Pololu G2 links reviewed

- Added a Previous Mistakes / Lessons Learned entry in `USV_Master_Context.md` for repeatedly raising undocumented PA-14P startup/stall current as a generic objection after the user had already clarified the information is unavailable at this stage.
- Reviewed the user's current-generation Pololu links: G2 24v12 (product 1364) and G2 18v15 (product 1362). Both are currently listed at about US$149 with connectors soldered, so they are expensive alternatives to the SyRen.
- The 18v15 is electrically appropriate for a 12 V controller supply based on its 6.5–30 V operating range and 15 A continuous rating. The 24v12 is a higher-voltage variant with a 12 A continuous rating and appears unnecessary if the controller is supplied from the planned 12 V rail.
- Both G2 models support TTL serial, I²C, RC pulse, analog, and USB control, plus configurable current limiting. They offer only full braking or full coasting, not variable braking. Their documentation does not by itself establish that either eliminates the supply-rail regeneration issue; this must be explicitly resolved before considering either a solution.
- Current conclusion: neither Pololu product is established as a better-value or simpler replacement for the SyRen 10. Keep the SyRen as incumbent; no purchase recommendation.
- Manufacturer pages: https://www.pololu.com/product/1364 and https://www.pololu.com/product/1362 ; G2 guide: https://www.pololu.com/docs/0J77/all


## 2026-10-22 — Narrowed SyRen power architecture to three options

- Retained the SyRen 10 as the incumbent after alternative-driver screening.
- Recorded three power architectures: (1) dedicated 12 V battery for SyRen, (2) main 24 V battery through a regenerative-compatible 24-to-12 V converter, or (3) converter plus a compatible battery buffer on the 12 V rail.
- Preliminary priority is simplicity and total cost. Option 1 is the most straightforward way to give regeneration a battery sink; Option 2 could be cheapest if the converter's behavior is explicitly verified; Option 3 has the highest integration/charging complexity.
- No final architecture or purchase decision.
