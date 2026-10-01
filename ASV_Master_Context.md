# ASV Master Context

**Status:** Living working document  
**Purpose:** Single source of truth for the user's from-scratch Autonomous Surface Vessel (ASV) build. Update this file as hardware, architecture, wiring, tests, and decisions change.

> **Working rule:** When advising on this project, use the exact hardware and specifications documented here whenever possible. Avoid generic assumptions when the user's actual part number/spec is known. If a required detail is unknown, explicitly mark it as unknown rather than silently substituting a typical component.

---

## 1. Project Overview

The project is a from-scratch, hands-on Autonomous Surface Vessel (ASV) build.

### High-level project goal

> **What is the simplest hardware architecture that will make this hull a reliable RC boat, and what do we need to make it happen?**

The true north star is a **fully functioning, manually controlled RC boat** that can reliably operate on the water. The Radiomaster T8L → ER8 → Arduino control chain is intended to remain part of the actual on-water boat, not merely a disposable prototype.

### Current project scope

For now, deliberately table added complexity. The following are **out of scope**:

- Telemetry
- GPS
- Autonomy
- Autonomous navigation
- Computer vision
- Other advanced navigation/vehicle features

These should not drive current architecture or purchasing decisions. The immediate objective is a simple, reliable RC boat.

### Design philosophy

- Start simple.
- Build in small, bite-sized proof-of-concepts (POCs).
- Avoid rebuilding the old system's complexity.
- Prefer readily understandable, serviceable architecture.
- Avoid soldering where practical; use appropriate crimped connectors, pin/socket connections, and off-the-shelf wiring.
- Validate each subsystem independently before integrating it.
- Use the actual inventoried hardware as the design baseline.
- Prefer purchasing hardware that will be used in the final on-water RC boat, rather than buying disposable prototype hardware.
- Let small-scale POCs prove concepts, but do not treat POC completion as the project goal; the boat is the goal.

### Current MVP control concept

The initial control MVP has two operator inputs:

1. **Throttle:** power/speed command for the propulsion system.
2. **Steering:** left/right command for the boat's steering mechanism.

The steering mechanism is **not yet committed to a dedicated rudder**. A steerable propulsion unit, including an electric trolling motor, is now an explicit architecture candidate.

Preferred operator interface:

- RC transmitter with two distinct controls/knobs or equivalent independent axes.
- RC receiver provides the control signals.
- A microcontroller (initially Arduino Uno R3) will interpret receiver signals and command the actuators/motor-control hardware.

Advanced autonomy/navigation features are explicitly out of scope for the current project phase. The immediate goal is reliable manual RC control and a functioning on-water boat.

---

## 2. Hull

### Known hull specifications

- Length: **5 ft 8 in**
- Width: **20 in**
- Deck: **3 mm plywood**, epoxy finished
- Bottom/sides: **Divinycell H80 foam core**, fiberglass
- Construction: **resin infusion**
- Resin: **vinylester**
- Seller stated design was by **Morrelli & Melvin**
- Seller stated it was built by a **professional boatbuilder**
- Approximate payload capacity: **200 lb**
- A long metal tube protrudes through the top of the hull from a previous installation/use.

### Hull status

- Hull is intended to serve as the platform for the new simplified ASV system.
- The project is **not** intended to reproduce the previous system installed in the hull.
- The hull has a **finbox on the underside**, approximately amidships, that accepts the supplied stand-up-paddleboard-style fin.
- The supplied fin appears intended to provide a fixed lateral stabilizing surface rather than act as a conventional steerable rudder.
- The stern/transom has an existing **metal mounting/bracket assembly** that may be relevant to a future propulsion mounting solution; its original purpose and load capacity are not yet verified.
- A separate vertical metal tube/legacy structure is visible through the deck near the aft/mid hull area; its function remains unknown.
- Photos show enough existing structure that a **transom-mounted or stern-mounted electric trolling motor** should be investigated before designing a dedicated rudder installation.

---

## 3. Electronics / Hardware Inventory

This section is deliberately specific. Add manufacturer, exact model/part number, quantity, voltage/current ratings, connector type, and condition whenever known.

### 3.1 Radio / RC control

#### Transmitter

**Radiomaster T8L ELRS 2.4 GHz Radio Transmitter**

- Protocol: ELRS
- Frequency: 2.4 GHz
- Battery: **2 × 18650 cells**
- Intended use: manual ASV operator control
- User wants two independent controls for throttle and steering.
- Current plan is to use one control for throttle and one for steering.

#### Transmitter batteries

**Hohm LIFE⁴ 18650**

- Chemistry: Li-NMC / lithium-ion
- Nominal voltage: **3.6 / 3.65 V**
- Capacity: **3015 mAh**
- Continuous discharge rating (CDR): **22.1 A**
- Quantity: **2 intended for the T8L**
- Intended use: T8L transmitter only
- User expects these cells to be installed directly into the T8L's two 18650 battery slots.

Do not assume these batteries are for the Arduino or boat power system. The current plan is to use bench power for Arduino/receiver/servo prototyping.

#### Receiver

**Radiomaster ER8 ELRS PWM Receiver**

- Intended protocol: ELRS
- Intended use: receive T8L commands and provide PWM outputs for the control prototype.
- Exact electrical/output configuration should be verified against the actual receiver before final wiring.

---

### 3.2 Microcontroller / development

#### Arduino

**Arduino Uno Rev3 — A000066**

- Intended use: initial control-system prototyping.
- Primary role: read RC receiver PWM signals and generate control outputs.
- Development computer: Apple Mac with Apple Silicon.
- User has an Apple M1 Mac with 16 GB RAM running macOS Sequoia 15.6.1 (24G90).
- Arduino IDE being used/planned: **Arduino IDE 2.3.10**, Apple Silicon build.
- USB is intended for uploading sketches to the Uno.
- Bench power supply is available for prototype power.
- User prefers to avoid soldering and use pin headers/sockets where possible.

---

### 3.3 Servos

#### High-torque waterproof servo

**HiTec D646WP**

- High torque
- 32-bit digital
- IP67
- Operating voltage: **3.5–8.4 V DC**
- Idle current: **30 mA @ 7.4 V**
- No-load operating current: **330 mA @ 7.4 V**
- Stall current: **2,200 mA @ 7.4 V**
- Connector wire gauge: **22 AWG / 60 strand**
- Intended use: control/actuation prototype and potentially steering/control mechanisms.
- Exact quantity to be confirmed.

#### Additional servo

**Sepp 955.CR High Torque Metal Gear Servo-360**

- Quantity noted in inventory: **4**
- Condition: at least one noted as **unopened**
- Type: high-torque metal gear, continuous-rotation / 360 servo
- Exact electrical specifications should be verified from the physical unit/datasheet before designing around it.

---

### 3.4 Actuator

**Progressive Automations PA-14p-6-35**

- Voltage: **12 V DC**
- Stroke: **6 in**
- Force: **35 lb**
- CE marked
- Batch information observed:
  - Used unit: **1910**
  - Unused unit: **2006**
- Used actuator has a **Deutsch DT06-6S male connector** on its outgoing wire.
- Intended/possible use: mechanical actuation.
- The actuator should not be assumed to be part of the MVP until its role is defined.

---

### 3.5 Motor / motor-control hardware

#### L298N motor driver board

- Red PCB
- Confirmed as an **L298N motor driver**
- Intended use: available for experimentation/prototyping.
- Exact board manufacturer/model unknown.

#### Other motor-related hardware

- Item labeled **“GWIN MOTOR”** was observed in the inventory.
- Exact model/specification unknown.
- Do not assume voltage/current/propulsion suitability without identifying the motor.

---

### 3.6 Connectors

The project has standardized around **Deutsch connectors**, with a preference to minimize the number of connector/tool types.

#### Known Deutsch connector inventory

- **6 × DT04-4P ↔ DT06-4S**
- **1 × DT04-3P ↔ DT06-3S**
- **1 × DT04-6P ↔ DT06-6S**
- Additional **DT06-3S** components observed.
- Small bags of Deutsch contacts
- Rubber seals
- Metal crimp contacts/connectors

#### Connector tooling

User has multiple Jready Deutsch extraction/removal tools:

- **Jready DRK-RT1B** double-ended metal removal tool — preferred and confirmed to work well.
- Green plastic removal tool
- Light blue/orange plastic removal tool

The double-ended metal tool has been the preferred method for releasing terminals from the housings.

#### Important connector note

The used PA-14p-6-35 actuator has a **DT06-6S male** connector.

The user does not currently have the complete hull physically available for inspection. There is a good chance the hull contains additional Deutsch connectors, potentially including **6P**, **4P**, and/or **3P** housings. These must be physically inspected before assuming compatibility.

---

### 3.7 Batteries / battery packs found with the hull

Two waterproof/Tupperware-style boxes were documented as battery packs.

#### Tupperware waterproof box #3 / #4 battery packs

Labels indicate battery pack **“2”**.

Each pack was described as:

- **5 × EnrichPower Ni-MH rechargeable AA cells**
- Cell voltage: **1.2 V**
- Capacity: **2000 mAh**
- Standard charge: **14 hr at 200 mA**

These are legacy/inventory batteries and are **not currently the planned power source for the T8L or Arduino prototype**.

---

### 3.8 Bench power / electrical tools

#### Bench power supply

**NANKADF GC3010H**

- Variable DC bench power supply
- Output: **0–30 V DC**
- Maximum current: **0–10 A**
- Intended use: Arduino/receiver/servo and other early electronics prototyping.
- Has adjustable voltage/current and output enable/disable functionality.
- Model identification/specification should be treated as the current inventory baseline; verify against the physical label/manual if a future wiring decision depends on a detail not recorded here.

#### Wire

- User has **18 AWG stranded wire**
- Six colors available
- Marine-grade status is currently unknown.

#### Multimeter

- User has/will use a Klein multimeter in the **MM400/MM420 class** for electrical testing.

#### Wire stripper

- **Klein 11055EP** considered/used for wire stripping.

#### Heat shrink

- User has a heat-shrink assortment.

#### Fusing

- ATO inline fuse holders have been discussed/considered.
- ATO fuse assortment available/considered.
- Littlefuse ATO inline fuse holder with **16 AWG** leads was discussed in relation to **18 AWG** wiring.
- Final fuse architecture is TBD.

---

## 4. Bench / Prototype Power

The user has a **NANKADF GC3010H 0–30 V / 0–10 A variable DC bench power supply** available.

Current intended prototype approach:

- Arduino Uno: powered from USB while programming/testing where appropriate, or bench supply when appropriate.
- Receiver + servo prototype: bench-powered.
- T8L: powered by its dedicated **2 × 18650** cells.
- Boat propulsion/engine power: **not yet being designed** and should be treated as a separate future workstream.

Do not conflate the transmitter battery system with the boat's eventual propulsion battery system.

---

## 5. Current Control Architecture

### Working signal chain

**T8L transmitter → ER8 ELRS PWM receiver → Arduino Uno → control output**

The current manual-control prototype uses:

- **ER8 CH1 → Arduino D2:** steering signal
- **Arduino D9 → HiTec D646WP:** steering servo test output
- **ER8 CH3 → Arduino D3:** planned throttle signal for POC 5; exact T8L channel assignment will be verified experimentally
- Throttle is initially displayed over Serial only; it does not control propulsion hardware yet.

### Step 1 — Candidate propulsion/steering architecture

The current design investigation includes three broad architectures:

**A — Conventional propulsion + dedicated rudder**
```
Arduino → ESC → propulsion motor → propeller
Arduino → steering servo → rudder
```

**B — Steerable propulsion unit**
```
Arduino → motor controller → motor + propeller
Arduino → steering actuator → rotates propulsion unit
```

**C — Electric trolling motor**
```
Arduino → trolling-motor throttle/control
Arduino → steering actuator or steering interface
                 ↓
        steerable trolling motor
                 ↓
             propeller
```

Option C is now a priority investigation because:
- The naval architect who sold the hull specifically mentioned the possibility of an **electric trolling motor**.
- The hull already has an underside **finbox and removable SUP fin**, which may provide fixed lateral stability without requiring a conventional rudder.
- The stern has an existing metal bracket/mounting structure that may provide a starting point for propulsion mounting, although this must be characterized before relying on it.
- A trolling motor packages the electric motor, propeller, shaft, and basic speed-control hardware into one propulsion assembly, potentially reducing custom mechanical fabrication.

This does **not** mean a trolling motor has been selected. Before purchase, characterize:
- Available transom mounting geometry.
- Existing stern bracket dimensions and function.
- Finbox dimensions and fin geometry.
- Required propeller depth.
- Required shaft length.
- Motor thrust/power requirement for this hull.
- Trolling motor throttle/control interface and whether it can be commanded by the Arduino architecture.
- Steering method: direct mechanical actuation of the motor/shaft versus an electronically steerable motor.
- Boat battery voltage/current requirements and protection.

A current design hypothesis is:

```
                 T8L
                  ↓
                 ER8
                  ↓
               Arduino
              ↙       ↘
       steering       throttle
           ↓              ↓
   steerable trolling   trolling
       motor unit        motor
           ↓              ↓
        propeller → thrust direction
           
       fixed SUP fin
       in existing finbox
```

The SUP fin should be treated as a **candidate fixed stabilizing surface**, not assumed to be structurally adequate for propulsion loads or steering loads.

---

### Confirmed T8L / ER8 mapping

- **ER8 CH1 = T8L right joystick X axis**
- CH1 is therefore immediately useful as the planned **steering** input.
- The transmitter/receiver did not require rebinding or channel remapping to obtain the desired steering input.
- T8L and ER8 are successfully bound and communicating.

### Confirmed CH1 signal characteristics

Observed during POC 4:

| State | PWM pulse width |
|---|---:|
| Minimum | ~984 µs |
| Center | ~1493–1496 µs |
| Maximum | ~2001–2002 µs |

The center signal varies by only a few microseconds. POC 5 uses an initial **30 µs steering dead zone** around a nominal 1493 µs center so normal signal jitter does not cause servo movement.

### Power architecture used for POC 4

- NANKADF GC3010H bench supply powers the ER8.
- The HiTec D646WP remains physically connected but is not required for POC 4.
- Arduino Uno is powered solely from USB.
- ER8 ground, Arduino GND, and bench-supply negative share a common ground.
- Arduino is **not** powered from the bench-supply rail.
- ER8 RX/TX and EXT-V are unused.

---

## 6. POC Roadmap

### POC 1 — Arduino blink

Goal: establish the Arduino development environment and basic digital output.

Repository sketch:
`sketches/01_basics/01_blink/01_blink.ino`

**Status: COMPLETE**

### POC 2 — Arduino Serial

Goal: establish Arduino → computer Serial communication.

Repository sketch:
`sketches/01_basics/02_serial_test/02_serial_test.ino`

**Status: COMPLETE**

### POC 3 — Arduino → servo

Goal: establish direct Arduino control of the HiTec D646WP.

- D646WP signal → Arduino D9.
- D646WP powered from NANKADF bench supply.
- Bench-supply negative and Arduino GND share a common reference.
- Full 0–180° behavior was not assumed; safe operating limits still need characterization.

Repository sketch:
`sketches/01_basics/03_servo_sweep/03_servo_sweep.ino`

**Status: COMPLETE**

### POC 4 — ER8 receiver signal visibility

Goal: prove the ER8 is receiving T8L commands and expose one PWM channel to the Arduino.

- ER8 CH1 signal → Arduino D2.
- Arduino measures pulse width with `pulseIn()`.
- T8L → ER8 wireless link successfully established.
- CH1 confirmed as T8L right joystick X axis.
- Measured CH1 range: approximately 984–2002 µs.
- Measured center: approximately 1493–1496 µs.

Repository sketch:
`sketches/01_basics/04_receiver_signal/04_receiver_signal.ino`

**Status: COMPLETE**

### POC 5 — T8L + ER8 + Arduino + servo

Goal: integrate the wireless steering signal with the D646WP while also observing throttle.

- ER8 CH1 → Arduino D2 → steering.
- ER8 CH3 → Arduino D3 → planned throttle signal; verify the T8L assignment experimentally.
- Arduino D9 → D646WP signal.
- Steering uses the observed CH1 range and an initial 30 µs center dead zone.
- Throttle is printed to Serial only.
- No propulsion hardware is connected or controlled.

**Final POC 5 result:** COMPLETE. Both receiver channels are captured simultaneously using Arduino Uno hardware interrupts; CH1 steering is mapped through the ±30 µs dead zone to the D646WP, and CH3 throttle is observed independently over Serial.

Repository sketch:
`sketches/01_basics/05_t8l_er8_servo/05_t8l_er8_servo.ino`

**Status: COMPLETE**

User successfully completed POC 5 Stage 2. T8L steering controls the HiTec D646WP through ER8 CH1 → Arduino D2 → Arduino D9, while ER8 CH3 simultaneously provides the throttle signal to Arduino D3 for Serial monitoring. The complete wireless control → receiver → Arduino → servo chain is working.

### POC 6 — Control-output hardware

Goal: replace the simple servo test with the appropriate actuator/motor-control interface.

- Determine exact propulsion hardware.
- Determine steering actuator/mechanism.
- Select appropriate driver/interface.
- Add appropriate power distribution and protection.

### POC 7 — Bench-scale propulsion/steering

Goal: demonstrate actual propulsion and steering hardware without putting the system on the water.

### POC 8 — Hull integration

Goal: install the simplified system in the hull.

### POC 9 — Manual water test

Goal: verify basic remote-controlled operation.

### POC 10 — Autonomy

Autonomous navigation/control should be treated as a later phase after reliable manual control exists.

---

## T8L Channels

The following channel mapping was experimentally observed on the user's T8L + ER8 setup. These are the **8 PWM channels available on the ER8**. Values are pulse widths in microseconds.

| Channel # | T8L Control (min, approx. middle, max) |
|---|---|
| Channel 1 | Right Joystick X (**984, 1489–1496, 2002**) |
| Channel 2 | Right Joystick Y (**984, 1489–1496, 2002**) |
| Channel 3 | Left Joystick Y (**984, middle hard to approximate because it does not self-center, 2002**) |
| Channel 4 | Left Joystick X (**984, 1489–1496, 2002**) |
| Channel 5 | Left Toggle button "SE" (**995–1001 not pushed in, 1989–1995 pushed in**) |
| Channel 6 | Left Bumper, hold-to-press "SA" (**995–1001 not pushed in, 1989–1995 pushed in**) |
| Channel 7 | Left Three-position Switch "SB" (**995–1001 bottom, 1492–1498 middle, 1989–1996**) |
| Channel 8 | Right Three-position Switch "SC" (**995–1001 bottom, 1492–1498 middle, 1989–1996**) |

### Additional T8L controls not represented by the ER8's 8 PWM outputs

The user identified two additional controls on the T8L that may be assigned to higher-numbered channels:

- **R Bumper, hold-to-press "SD"** — expected **Channel 9** based on the observed channel pattern. Same type/location as CH6 "SA", but on the right side.
- **R Wheel Spinner "S1"** — expected **Channel 10** based on the observed channel pattern. Same general location/type as CH5 "SE", but on the right side; finite-rotation single-axis wheel. Minimum and maximum pulse widths are currently **unknown**.

These Channel 9/10 assignments are **expected, not experimentally verified**. The ER8 used in this POC exposes only CH1–CH8 PWM outputs. The separate **BATT** connection on the ER8 was not used or tested as an additional channel; it is treated as the receiver power/battery connection, not a ninth PWM output.

## Arduino Pin / PWM Capture Note

During POC 5 two-channel signal testing, the initial `pulseIn()` implementation made Arduino D3 appear to fail when CH1 was also connected. A live test showed that CH3 immediately produced valid readings when CH1 was disconnected, demonstrating that the receiver channel and Arduino input path were functional.

The actual issue was the **blocking nature of sequential `pulseIn()` calls** when trying to read two continuously generated PWM channels. D3 is therefore **not considered bad**.

POC 5 uses the Arduino Uno's hardware interrupt-capable pins:
- **D2 ← ER8 CH1 (steering)**
- **D3 ← ER8 CH3 (throttle)**

The POC 5 sketch now uses interrupt-based edge timing rather than sequential `pulseIn()` calls so both PWM channels can be captured concurrently.

POC 5 Stage 2 now also commands the HiTec D646WP from CH1 on Arduino D9 while continuing to report CH3 throttle over Serial. Steering uses the observed CH1 calibration (984 / 1493 / 2002 µs) with a ±30 µs dead zone around the nominal center. Initial servo output is conservatively limited to 60°–120°.
## Current Project Milestone

### Completed through POC 5

The project has now progressed from standalone Arduino fundamentals to a working wireless manual-control signal chain:

```
T8L
 ├── Right Joystick X
 │       ↓
 │     ER8 CH1
 │       ↓
 │   Arduino D2
 │       ↓
 │   D646WP signal → D9
 │
 └── Left Joystick Y
         ↓
       ER8 CH3
         ↓
       Arduino D3
         ↓
      Serial monitor
```

Confirmed hardware/software behavior:
- T8L successfully communicates with ER8 over ExpressLRS.
- ER8 CH1–CH8 channel mapping has been experimentally documented.
- CH1 steering range observed at approximately **984–2002 µs**, with center around **1489–1496 µs**.
- CH3 throttle is continuously readable simultaneously with CH1.
- Arduino Uno **D2 and D3** are used as interrupt-capable PWM inputs.
- Sequential `pulseIn()` was replaced with interrupt-based edge timing for reliable simultaneous channel capture.
- CH1 uses a **±30 µs steering dead zone** around a nominal 1493 µs center.
- HiTec D646WP steering servo is successfully commanded from the T8L right joystick X axis.
- Initial servo output is conservatively limited to **60°–120°**.
- Arduino is USB-powered; ER8 and servo are bench-powered; grounds are common.
- ER8 `RX/TX` and `EXT-V` remain unused for the current POC architecture.
- ER8 `BATT` is understood as the dedicated receiver power connection and is preferred for future receiver power wiring.

**Current overall status:** POCs 1–5 complete. The project is now in **Step 1 — Design** of the hardware-focused development process. Electric trolling-motor propulsion/steering (Option C) is an explicit architecture candidate and should be investigated before committing to a dedicated rudder/propulsion system.

## 7. Final RC Boat Development Process

Now that the small-scale foundational POCs are complete, the project has transitioned from primarily software/electronics fundamentals to **hardware-focused design and integration**. Future work should follow this five-step process:

### 1. Design

Define the simplest viable hardware architecture for a reliable RC boat before purchasing components. Establish what each subsystem must do and how the major components connect.

### 2. Understand what parts we have

Inventory and characterize the hardware already available. Use exact manufacturer, model/part number, electrical specifications, connector type, mechanical interfaces, and condition whenever known. Do not design around unidentified components without first confirming what they are capable of.

### 3. Understand what parts we need

From the design and existing inventory, identify the actual gaps. Produce a concrete list of required components rather than shopping opportunistically or buying generic prototype parts.

### 4. Analyze options

For each required component, evaluate realistic alternatives using compatibility, electrical requirements, mechanical fit, marine suitability, connector strategy, serviceability, cost, availability, and whether the component will remain in the final boat.

### 5. Confirm compatibility

Before purchasing, verify the complete end-to-end system. Check electrical compatibility, voltage/current requirements, signal interfaces, connector gender/pinout, physical fit, mechanical interfaces, power distribution, and environmental suitability. A component should be purchased only after its role and compatibility with the surrounding system are understood.

**Hardware purchasing principle:** The next purchases should preferentially be **final-boat hardware** that will actually remain on the vessel when it goes on the water. Do not optimize for the cheapest temporary POC if a reasonable final component can be characterized and tested on the bench first.

**Working sequence:**

Design → Understand what parts we have → Understand what parts we need → Analyze options → Confirm compatibility → Purchase/build

This process replaces the previous POC-centric mindset as the primary way of organizing the project. POCs remain useful as validation steps inside this process, but they are no longer the project's primary milestones.

---

## 7. Decisions / Constraints

### Confirmed preferences

- The true north star is a simple, reliable, fully functioning RC boat that can operate on the water.
- Build from scratch rather than reproduce the legacy electronics system.
- Treat the T8L → ER8 → Arduino chain as part of the final boat architecture, not throwaway prototype hardware.
- Prefer final-boat hardware for new purchases whenever practical.
- Use the five-step Design → Understand what parts we have → Understand what parts we need → Analyze options → Confirm compatibility process for hardware decisions.
- Keep the architecture simple.
- Prefer incremental POCs.
- Minimize soldering.
- Prefer Deutsch connectors.
- Minimize the number of connector types/tools.
- Use the user's actual hardware inventory as the design baseline.
- Use the bench power supply during early electronics prototyping.
- Keep T8L battery power separate from boat power discussions.
- Keep telemetry, GPS, autonomy, navigation, computer vision, and similar added complexity out of the current project scope.
- Develop code incrementally, starting with simple standalone sketches before integrating T8L/ER8/servo control.

### Avoid

- Designing around unidentified components without first confirming their specifications.
- Assuming a connector is electrically compatible solely because it physically mates.
- Assuming the legacy hull electronics represent the desired architecture.
- Jumping to high-power propulsion testing before validating the signal chain.
- Introducing unnecessary software complexity before the basic signal path works.

---

## 8. Open Questions / To Verify

1. Exact **ER8** model/version and PWM output specifications.
2. Exact ER8 power-input requirements.
3. Exact T8L channel/control mapping beyond the confirmed CH1 steering assignment (POC 5 will verify the throttle channel).
4. Quantity and exact configuration of HiTec D646WP servos.
5. Exact specifications of the **GWIN MOTOR**.
6. Exact specifications of the legacy motor/propulsion system in the hull.
7. Whether the hull's existing Deutsch connectors can be reused.
8. Identity/function of the long metal tube through the hull.
9. Whether the supplied finbox/SUP fin should remain as the fixed lateral stabilizer.
10. Dimensions, function, and load capacity of the existing stern metal bracket/mounting assembly.
11. Final steering mechanism: dedicated rudder, steerable propulsion unit, or trolling-motor steering.
12. Final propulsion architecture, with an electric trolling motor now an explicit candidate.
13. Exact trolling-motor control interface compatible with the Arduino architecture, if Option C is selected.
14. Marine-grade status of the user's 18 AWG wire.
12. Final fuse ratings and power-distribution architecture.
13. Boat-side battery chemistry, voltage, capacity, and BMS/protection requirements.
14. Exact connector gender/pinout for every component before integration.
15. Exact electrical requirements of the chosen servo before connecting it to external power.

---

## 9. Inventory Update Format

When a new item is acquired or identified, record it using:

| Category | Manufacturer | Exact Model / Part # | Qty | Key Specs | Connector | Condition | Intended Use | Status |
|---|---|---|---:|---|---|---|---|---|
| Example | Manufacturer | Part number | 1 | Voltage/current/etc. | Deutsch DTxx | New/Used | Purpose | TBD |

For every electrical component, capture **voltage, current, signal type, connector, polarity/pinout, and relevant environmental rating** when known.

---

## 10. Project History

Detailed chronological history is maintained in **[`ASV_CHANGELOG.md`](./ASV_CHANGELOG.md)**.

The Master Context intentionally does not retain a detailed chronological change log so that it remains focused on the current project state.

---

## 11. Previous Mistakes / Lessons Learned

This section exists specifically to prevent repeating known recommendation and verification failures. These are not merely historical notes; they are required checks when making future ASV purchasing, wiring, and architecture recommendations.

### 2026-09-16 — BOJACK 130-piece breadboard kit / connector mismatch

**Mistake:**  
Recommended the BOJACK 130-piece breadboard jumper-wire kit for the early ER8/Arduino POC without verifying its actual contents against the required physical connections. The kit contains male-to-male jumper wires only. The ER8 exposes male header pins, so female-ended wiring is required for a direct connection.

**Impact:**  
The kit does not provide the female-ended wires needed for the planned ER8 connection, despite having been recommended as part of the starter setup.

**Prevention:**  
Before recommending a wiring kit, verify:
- Exact kit contents.
- Connector gender on both ends.
- Pin pitch.
- The physical connector/interface on every component in the intended signal path.
- Whether the complete end-to-end connection can actually be assembled without soldering.

Do not assume a generic "jumper wire kit" contains the required connector types.

### 2026-08 — Hohm LIFE⁴ / Radiomaster T8L battery compatibility

**Mistake:**  
Verified the purchase of Hohm LIFE⁴ 18650 cells for the Radiomaster T8L but failed to catch the battery-voltage compatibility issue the user subsequently identified: the Hohm cell is specified at 3.6/3.65 V nominal, and the T8L was treated as though a generic 3.7 V nominal specification established compatibility without verifying the transmitter's actual supported cell and charging requirements.

**Impact:**  
The battery recommendation was made without sufficient verification of the complete battery/device compatibility requirements.

**Prevention:**  
For battery recommendations, verify:
- Chemistry.
- Nominal voltage.
- Full-charge voltage.
- Physical dimensions.
- Protection/configuration.
- Manufacturer-supported battery types.
- Connector and polarity.
- Device charging architecture.

Do not assume 3.6 V and 3.7 V nominal cells are interchangeable without checking the device requirements.


## 2026-09-29 — Trolling-motor control requirements

The current Option C investigation is looking for a trolling-motor solution that can plausibly become final-boat hardware while remaining compatible with the project's simple T8L → ER8 → Arduino architecture.

### Current requirements

1. **Saltwater friendly** — suitable for the intended marine environment and specifically rated/documented for saltwater use.
2. **Lightweight** — minimize propulsion-system weight; exact maximum acceptable motor weight is TBD until hull mounting and balance constraints are known.
3. **Investigate control methods**:
   - Mechanical servo actuation of a tiller, speed control, or other user-facing control.
   - Direct electronic control from Arduino through a documented or accessible interface.
   - Use of the motor's own remote-control/transmitter system if it provides a practical path to remote operation.
4. **Potential remote steering** — determine whether the motor can be steered remotely by mechanically rotating its built-in tiller with a servo, or by commanding a built-in electronic steering system/interface.

### Additional screening criteria

- Prefer **12 V** unless there is a compelling reason to move to 24 V/36 V.
- Thrust must be appropriate to the actual loaded hull displacement; do not select solely from marketing categories.
- Maximum current draw must support a practical battery, breaker/fuse, wiring, connector, and power-distribution design.
- Shaft length and propeller depth must fit the hull geometry and eventual waterline.
- Mounting should preferably use or reasonably adapt the existing stern/transom bracket, but its structural capacity must be verified first.
- Steering must provide sufficient authority and mechanical travel.
- Determine throttle behavior: forward/reverse, variable vs. discrete speed, neutral/off, and stop/failsafe behavior.
- Favor corrosion-resistant, serviceable hardware with available replacement parts.
- Do not make a proprietary wireless remote the primary architecture unless its integration path is practical.
- Treat reverse engineering as a higher-complexity option than a documented electrical interface or simple mechanical servo actuation.
- Prefer hardware that can remain in the final boat rather than temporary test-only hardware.

### Control paths under investigation

| Path | Concept | Main question |
|---|---|---|
| **C1 — Mechanical tiller actuation** | Arduino → servo → physical motor control | Can a servo reliably actuate steering/throttle without creating a fragile mechanism? |
| **C2 — Direct electrical interface** | Arduino → motor's accessible control interface | Does the motor expose a practical signal/interface Arduino can command? |
| **C3 — Motor's own remote** | Motor ↔ supplied wireless controller | Can the existing remote system be used without excessive reverse engineering? |
| **C4 — Custom/third-party RC interface** | T8L/ER8 → compatible motor controller/receiver path | Does a supported interface exist that preserves the current RC architecture? |

These are investigation categories, not decisions. No trolling motor should be purchased until its control path, power requirements, mounting approach, and marine suitability are sufficiently understood.


### 2026-09-29 — Refined Option C direction

The user refined the trolling-motor investigation after reviewing initial candidates:

- Avoid mechanically hacking the throttle if possible. Combining throttle hacking with steering/tiller hacking is considered unnecessarily difficult and fragile.
- Mechanical tiller/steering actuation remains acceptable. A servo physically turning the trolling motor for steering is still a viable path to investigate.
- Avoid deep reverse engineering. The project should not depend on undocumented proprietary protocols, fragile software integrations, or a large amount of reverse-engineering work merely to make the motor controllable.
- Avoid high-cost electronic-steering trolling motors. Motors in the $1,000+ class are outside the intended budget. Approximately $600 is already considered budget-stretching rather than a target price.
- Preserve the Arduino where practical. The Arduino Uno should remain part of the control architecture if a practical, robust interface can be built around it. This is both technically useful and an intentional part of the hands-on project.
- Manual override/failsafe is acknowledged as required but is intentionally tabled for later architecture work. It should not yet constrain the trolling-motor shortlist beyond noting that it will eventually need to be addressed.

A particularly interesting sub-architecture is now:

T8L → ER8 → Arduino
  ├── steering servo → physically rotates trolling motor
  └── electronic motor controller → trolling-motor propulsion

This could avoid mechanically actuating the trolling motor's throttle while retaining the Arduino and using the trolling motor primarily as a motor/propeller/shaft/steering assembly. Newport documents external PWM speed controllers for its conventional trolling motors, but its documented PWM approach uses a manual speed-control device; whether an Arduino-compatible high-current controller can replace that function must be investigated rather than assumed.


### 2026-10-01 — Alternative brushless underwater-thruster architecture

A parallel propulsion path is now under investigation alongside the conventional 12 V trolling-motor path: an inexpensive **brushless underwater thruster + external ESC + Arduino** architecture.

The user is explicitly open to designing and fabricating the propulsion mounting as part of the hands-on project, provided the mechanical solution is realistic for this hull. A dedicated rudder is acceptable and may be preferable if the thruster itself is fixed.

The most important finding so far is that this architecture can preserve the desired control chain without proprietary motor reverse engineering:

```text
T8L → ER8 → Arduino
              ├── PWM → brushless ESC → underwater thruster
              └── steering servo → rudder
```

A particularly strong candidate is the **APISQUEEN U10**:
- 24 V (6S LiPo)
- 900 W
- 10 kg stated thrust at 24 V
- 40 A stated load current
- 300 KV brushless motor
- approximately 800 g in air / 350 g in water
- aluminum construction
- stated fresh-water and sea-water operation
- M3 mounting holes with 20 × 16 mm spacing
- available with a bidirectional 100 A APISQUEEN ESC
- documented RC-style PWM control: approximately 1.0 ms full reverse, 1.5 ms stop, 2.0 ms full forward; variable speed between those limits
- published PWM frequencies include 50, 100, 200, and 500 Hz

The U10's published PWM interface makes it a materially better Arduino candidate than an inexpensive motor whose controller requires proprietary reverse engineering. The documentation explicitly describes the ESC signal line and PWM behavior. The thruster itself is listed at approximately $198.30, with the 100 A ESC offered as a variant; exact delivered system cost remains to be verified.

Another APISQUEEN candidate is the **S10**, a 24 V / 1000 W brushless pump-jet thruster listed at 8.5 kg thrust. It is listed around $352 for the thruster alone and around $537 as a set with waterproof remote/control box/base. Its control interface still needs to be characterized before it is considered preferable to the U10.

The **Blue Robotics T200** is the benchmark/reference architecture because its documentation is unusually strong. It is a 7–20 V flooded brushless thruster, rated up to 32 A / 645 W at 20 V and approximately 6.7 kgf forward thrust at 20 V. Blue Robotics explicitly documents Arduino control using its Basic ESC and standard RC-style PWM. Current published price is $230 for the T200 and $40 for the Basic ESC before applicable surcharge/shipping. It is therefore a credible POC/final-hardware reference, although its thrust is substantially lower than the U10 and its total cost is not necessarily lower.

This alternative path is **not selected over the trolling-motor path**. Both remain active architecture candidates.

### Brushless-thruster architecture screening criteria

For future candidates, verify:
1. Actual thrust data at the intended voltage, not just marketing horsepower.
2. Maximum and continuous current.
3. ESC voltage/current rating and safety margin.
4. Exact ESC command interface, preferably documented RC-style PWM or another Arduino-friendly interface.
5. Forward/reverse/neutral behavior and initialization/failsafe behavior.
6. Saltwater suitability and corrosion-resistant construction.
7. Propeller/guard design and susceptibility to weeds/debris.
8. Mechanical mounting geometry and structural load path into the hull.
9. Rudder placement and steering authority if the thruster is fixed.
10. Battery voltage/capacity/BMS requirements and resulting wiring/fusing.
11. Whether connectors can be integrated into the project's Deutsch strategy without unnecessary soldering.
12. Total cost versus the conventional trolling-motor architecture.

The user is intentionally comfortable designing and fabricating a custom thruster mount and, if needed, a dedicated rudder. This is considered worthwhile hands-on mechanical work rather than an automatic reason to reject the architecture, provided the mount can be built robustly from the hull's known materials and available structure.
