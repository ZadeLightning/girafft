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
- The stern/transom has an existing **metal mounting/bracket/linkage assembly** that may be relevant to a future propulsion or steering solution; its original purpose and load capacity are not yet verified.
- The old-build PA-14P-6-35 actuator has now been physically matched to this stern assembly: its fixed end mounts to a small square block on the hull/deck, while its extending rod connects to the stern linkage.
- The exact function of the stern mechanism remains unknown, but the geometry strongly indicates that the actuator was intended to move a stern-mounted mechanism rather than provide propulsion itself.
- Two matching circular hull penetrations near the actuator installation are available as potential wire/cable pass-throughs. One is already identified as a convenient route for the PA-14P wiring into the hull; the other remains unused/unknown.
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
- **Legacy hull installation identified:** the user has now physically inspected the actuator in relation to the stern/transom hardware from the old build.
- The actuator's **fixed/non-extending end** attaches to a small square mounting block on the deck/transom area.
- The actuator's extending rod connects to the existing stern metal linkage/bracket assembly.
- The existing arrangement strongly suggests the PA-14P was used to provide **mechanical linear actuation of a stern-mounted mechanism**, potentially steering, but the exact controlled function (rudder/tiller, propulsion steering, throttle/shift, or other mechanism) is **not yet confirmed**.
- Two existing circular hull penetrations are located near the actuator installation. The **forward/starboard penetration** is readily usable for routing the PA-14P wiring into the otherwise largely empty hull.
- The **aft/centerline penetration** is an additional matching circular hull penetration located essentially on the boat centerline. Its original function and future use are **unknown**.
- A larger hatch farther forward provides limited internal access; internal stability walls partially isolate the stern and bow, so future internal wiring/mechanical work should favor the existing penetrations rather than relying on access through the hatch.
- **Potential reuse:** the existing PA-14P mounting geometry and stern linkage should be treated as potentially reusable hardware for the final steering architecture until the stern mechanism is positively identified.
- The actuator should not be assumed to be part of the MVP until its role and the mechanical interface are confirmed.

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

### POC 6 — Arduino-controlled U10 propulsion

Goal: bench-test the selected propulsion hardware and establish the Arduino → ESC → U10 control path before hull installation.

**Planned hardware:**
- **APISQUEEN U10** single underwater brushless thruster, 24 V / 6S target architecture.
- **APISQUEEN 2–6S 100 A Feather ESC, waterproof variant**, with 5.5 V / 3 A BEC.
- Arduino Uno Rev3 A000066 as the command source.

**Planned signal architecture:**

T8L → ER8 → Arduino → PWM → U10 ESC → U10

The initial POC should verify, in order:
1. ESC powers up and arms safely.
2. Arduino can command neutral/stop.
3. Forward and reverse commands work.
4. PWM endpoints/frequency are characterized rather than assumed.
5. Motor current and ESC temperature are observed at increasing throttle levels.
6. The ESC remains thermally stable during sustained operation at representative U10 loads.
7. The actual U10/ESC behavior is recorded before hull integration.

**Bench-power limitation:** The inventory NANKADF GC3010H is rated **0–30 V / 0–10 A**. This is sufficient for low-power/low-throttle electronics and initial control testing, but **not sufficient for a full-power U10 test**. Published U10 test data reaches 32 A at 24 V, and APISQUEEN's current U10 guide identifies 32 A as the maximum working current. The 10 A bench supply cannot reproduce the U10's maximum-load operating point. A current-limited bench test can still be useful at low throttle, but current limiting may cause the ESC to behave differently from a properly powered propulsion test.

The U10 is an underwater thruster and should not be treated as a conventional motor for unrestricted dry running; propulsion tests should provide the required water environment/cooling and a secure mechanical restraint. A future full-power test will require a suitable ~24 V source capable of approximately 35–40 A continuous, with appropriate fusing and wiring.

**Status:** Planned; U10 and waterproof 100 A Feather ESC selected for purchase by the user. No propulsion hardware has yet been installed in the hull.

### POC 6 — Control-output hardware

Goal: broader control-output hardware selection and integration planning. Immediate propulsion testing is handled by the dedicated U10 POC above.

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
10. Dimensions, function, and load capacity of the existing stern metal bracket/mounting/linkage assembly.
11. Identify exactly what the PA-14P stern linkage actuated in the legacy build (rudder/tiller, propulsion steering, throttle/shift, or other mechanism).
12. Determine whether the two matching circular hull penetrations are intended cable glands/pass-throughs and what hardware originally occupied the aft/centerline penetration.
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


### 2026-10-01 — Brushless-thruster path strengthened; hull-weight baseline added

Follow-up research materially strengthened the inexpensive brushless underwater-thruster architecture as a leading candidate, while the conventional trolling-motor architecture remains active and unselected.

The **APISQUEEN U10** documentation confirms a direct Arduino-compatible control path through its bidirectional 100 A ESC:
- 24 V / 6S LiPo
- 900 W
- 40 A stated load current
- 10 kg / 22 lb stated thrust at 24 V
- 300 KV
- approximately 800 g in air and 350 g in water
- 117.5 × 95.5 mm body
- M3 mounting holes with 20 × 16 mm spacing
- stated freshwater and seawater operation
- 14 AWG, 1 m motor lead; 4.0 mm banana plugs as supplied
- ESC BEC: 5 V, 1 A
- bidirectional PWM command: approximately 1.0 ms reverse, 1.5 ms stop, 2.0 ms forward; documented frequencies include 50/100/200/500 Hz

This is a major architecture advantage: the Arduino can generate the throttle command using the same general Servo-library/pulse-width technique already proven in the project, without reverse engineering a proprietary protocol. The U10 ESC's published interface is directly compatible in concept with the project's T8L → ER8 → Arduino chain.

Important caveat: the U10's **5 V / 1 A BEC should not be assumed adequate to power the HiTec D646WP steering servo**. The D646WP's documented stall current is 2.2 A at 7.4 V. Servo power architecture therefore remains a separate design item.

The user provided a new hull baseline: **approximately 20 lb for the empty hull shell alone**. This excludes fin, rudder, thruster, battery, electronics, enclosure, wiring, mounts, and other additions. Use 20 lb as the baseline for rough propulsion and stability calculations until a more complete all-up displacement is measured.

The user is particularly interested in the low, underwater placement of a thruster. The expected benefits are reduced topside mass and potentially a lower center of gravity compared with a conventional heavy transom-mounted trolling motor. This is a design consideration, not yet a demonstrated stability improvement: actual center of gravity, buoyancy, mounting reaction loads, and hull geometry still need to be characterized.

A possible architecture is now:

```
T8L → ER8 → Arduino
              ├── PWM → U10 100A ESC → fixed underwater thruster
              └── PWM → HiTec D646WP → conventional rudder
```

The user is comfortable designing the propulsion mounting and is open to practicing fiberglass work by mounting the thruster through/on the underside of the hull. The existing stern bracket assembly may also be useful for a rudder-post/mounting structure. Any bottom-mounted thruster must have its thrust and impact loads distributed into reinforced hull structure; the Divinycell H80 foam-core laminate should not be treated as a point-load mounting surface without appropriate reinforcement.

### Propulsion sizing note

The U10's published 22 lb maximum thrust is roughly equal to the empty hull's 20 lb weight, but **thrust-to-weight ratio is not a valid boat-sizing rule by itself**. The actual requirement depends on all-up displacement, hull resistance, wetted area, hull form, desired speed, propeller/thruster efficiency, and operating conditions.

For rough future calculations:
- Empty hull baseline: 20 lb.
- If all-up boat mass were 50 lb, 22 lb static thrust would be ~0.44 thrust/weight.
- If all-up mass were 70 lb, ~0.31.
- If all-up mass were 80 lb, ~0.28.

These ratios are only sanity checks, not performance predictions.

The U10 manufacturer also claims 7–12 km/h propulsion speed and up to 350 kg propellable weight, but those claims should be treated cautiously until independent or test data is available. The useful next engineering step is to determine whether the U10's actual thrust/current curve is appropriate for this specific 5 ft 8 in hull rather than relying on the marketing "propellable weight" figure.

### Reference benchmark

The **Blue Robotics T200** remains the strongest documentation benchmark:
- 7–20 V
- 3.71 kgf / 8.2 lb at 12 V
- 5.25 kgf / 11.6 lb at 16 V
- 6.7 kgf / 14.8 lb at 20 V
- 17 A at 12 V, 24 A at 16 V, 32 A at 20 V
- approximately 427 g in air for the standard 1 m-cable version
- 300+ m tested depth
- explicit Arduino/RC-style PWM support through the Basic ESC

The T200 is lighter and exceptionally well documented, but its maximum thrust is substantially below the U10's published 10 kg. The much larger Blue Robotics T500 produces 16.1 kgf / 35.5 lb at 24 V but currently costs about $790 before its appropriate ESC, so it is not aligned with the project's inexpensive-hardware objective.

### Current design posture

The brushless underwater-thruster architecture is now a **leading candidate to investigate**, not a final selection. The conventional saltwater trolling-motor architecture remains equally active until propulsion sizing, mounting, battery, steering, and real-world control behavior are compared.


### 2026-10-01 — Path #1 designation and initial U10 propulsion sizing

The user designated the brushless underwater-thruster architecture as **Path #1**. This is currently the leading path for investigation, but it is not a final architecture selection; the conventional trolling-motor path remains active as an alternative.

The user supplied an empty-hull shell baseline of approximately **20 lb**. This is now the starting point for rough propulsion and stability calculations. All-up displacement will be refined later after adding the fin, rudder, thruster/mount, battery, electronics, enclosure, wiring, and other hardware.

### U10 measured/published thrust data found

A published APISQUEEN U10 test table gives the following full-throttle forward/reverse data:

| Voltage | Forward current | Forward thrust | Reverse current | Reverse thrust |
|---:|---:|---:|---:|---:|
| 14 V | 16 A | 4.6 kg / 10.1 lb | 16 A | 4.3 kg / 9.5 lb |
| 16 V | 17 A | 5.6 kg / 12.3 lb | 17 A | 5.2 kg / 11.5 lb |
| 18 V | 20 A | 6.9 kg / 15.2 lb | 21 A | 6.1 kg / 13.4 lb |
| 20 V | 25 A | 8.1 kg / 17.9 lb | 26 A | 7.1 kg / 15.7 lb |
| 22 V | 27 A | 9.1 kg / 20.1 lb | 28 A | 8.1 kg / 17.9 lb |
| 24 V | 32 A | 10.4 kg / 22.9 lb | 32 A | 9.4 kg / 20.7 lb |

These values are from a published thruster test table and are more useful for engineering than the headline 10 kg thrust claim. The source also lists the U10 as 12–24 V, with 24 V as the optimal operating voltage, although another U10 manual lists 24 V / 6S LiPo as the supported configuration. Use 24 V as the current target only; final battery architecture remains TBD.


### 2026-10-03 — Stern bracket measured dimensions

The user measured the existing stern mounting bracket for potential reuse in a new rudder/steering design:

| Dimension | Measured value | Notes |
|---|---:|---|
| Pin diameter | **1/4 in** | Existing pins pull out relatively easily and lack a robust locking mechanism. Plan may be to retain the bracket but replace pins with positively retained/locking hardware. |
| Upper pin center → lower pin center | **3-3/8 in** | |
| Clear gap between pins | **3-1/8 in** | Consistent with 1/4 in pin diameter. |
| Bottom of lower pin → top of upper pin | **3-5/8 in** | |
| Inside width between side plates | **4-5/8 in** | |
| Outside width across side plates | **5 in** | |
| Side-plate thickness | **~5/32 in** | Approximate measurement. |
| Bracket plate height | **6 in** | Plates do not extend above/below the transom; they begin approximately 1 in below the transom deck/top. |
| Top pin-hole center below bracket top | **5/8 in** | |
| Bottom pin-hole center below bracket top | **4 in** | |
| Closest bracket bottom → hull bottom | **1-7/8 in** | Because the hull bottom is V-shaped. |
| Bracket bottom → bottom of hull V | **2-3/8 in** | |
| Transom → pin center | **1 in** | Measured across the small gap where the plates connect to the transom. |
| Transom-side plate connection → pin center | **~13/16 in** | More representative of the actual plate offset. |

### Preliminary hull-bottom geometry interpretation

The measurements indicate the stern hull bottom rises toward the transom in a V-shaped profile. The bracket bottom sits **1-7/8 in above the nearest hull surface** and **2-3/8 in above the bottom of the V**, giving a **5/8 in difference** between those two reference points.

This is useful for future rudder/thruster clearance calculations, but it is **not yet sufficient to uniquely determine the V angle/pitch**. We need at least one additional horizontal/fore-aft measurement between the relevant hull-bottom reference points before calculating the slope/angle.

### Rudder-envelope terminology

“Approximate maximum rudder envelope” means the **largest physical volume a new rudder could occupy without hitting the hull, bracket, ground/stand, or other hardware through its intended steering range**. It is not necessary to measure this yet.

For the next rudder-design step, more useful measurements are:
- maximum allowable rudder width/height;
- distance from the bracket/transom to the rudder pivot axis;
- available underwater depth at the intended rudder location;
- clearance to the hull bottom through full left/right steering;
- clearance to the U10 propeller/thruster if Path #1 is retained.

### 2026-10-03 — Stern bracket / steering architecture observation

The user clarified the legacy stern hardware and the intended direction for the new build:

- The second circular hull penetration is essentially **centerline**, not port-side.
- The existing stern bracket is a substantial aluminum assembly attached to the transom/hull. It includes two horizontal removable pins/rods and vertical side plates, creating a useful mechanical mounting/interface structure.
- No legacy tiller, rudder, or other obvious steering component was found with the old hardware.
- The project does **not** need to reproduce the legacy boat's exact propulsion/steering system. The old hardware is being treated as a source of useful mounting geometry and potential mechanical infrastructure, not as a specification for the new architecture.
- The current plan should therefore consider designing a **new rudder/steering mechanism** around the existing stern bracket if practical.
- The PA-14P remains a candidate steering actuator only if its stroke, mounting geometry, travel, and loads prove suitable for the newly designed rudder/linkage.
- The existing stern bracket should be measured and characterized before committing to a new rudder design. In particular, determine pin diameter, spacing, plate geometry, attachment method, available rudder envelope, and how loads are transferred into the hull.

### 2026-10-03 — Legacy PA-14P stern installation characterized

The user physically inspected the **Progressive Automations PA-14P-6-35** and its relationship to the old-build stern hardware.

Observed installation:

- The PA-14P's **fixed/non-extending end** attaches to a small square mounting block on the deck/transom area.
- The extending actuator rod connects into the existing stern metal linkage/bracket assembly.
- This confirms that the PA-14P was mechanically integrated with the stern assembly in the previous build.
- The exact function remains unconfirmed. The mechanism may have controlled a rudder/tiller, steerable propulsion, throttle/shift, or another stern mechanism.
- A matching circular hull penetration immediately forward/starboard of the actuator is an easy route for the actuator's wiring into the hull.
- A second matching circular penetration on the boat centerline is currently unused/unknown.
- The hull interior is largely empty, but internal stability walls limit access to the stern/bow from the larger forward hatch. Existing penetrations are therefore potentially valuable for future wiring.
- The existing PA-14P mounting and stern linkage should be preserved as potentially reusable hardware until the old mechanism is positively identified.

This discovery materially increases the value of the legacy stern hardware as a candidate for the final steering/mechanical architecture. It does **not** establish that the PA-14P or its linkage should be used; function, travel, load path, and compatibility with the new propulsion architecture still need to be verified.

---

### 2026-10-03 — Stern transom V geometry measured

The user measured the stern cross-section as an isosceles triangle representing the V shape:

- Long/base side: **15-3/8 in**
- Two equal sides: **7-7/8 in** each
- Half-base: **7-11/16 in**
- Calculated altitude/depth: **~1.708 in (1-45/64 in)**
- Calculated base angles: **~12.53° each**
- Calculated apex angle at the bottom of the V: **~154.94°**
- Each hull-bottom face is therefore approximately **14.42° below horizontal** in this transverse cross-section.

These calculations assume the 15-3/8 in measurement is the full width of the triangle and the two 7-7/8 in measurements run from each end to the centerline V apex. The measurements are internally consistent.

This gives us the **transverse deadrise/V geometry at the stern**, which is directly useful when designing an underside U10 mounting plate. It does not establish the fore-aft longitudinal slope of the hull bottom. For a complete U10 mount geometry, we will still need a fore-aft measurement describing the bottom slope over the intended mounting location.

---

### 2026-10-09 — Rudder steering concept from legacy actuator and stern bracket

The user wants to design a new rudder/steering system using the existing stern bracket as a potential structural mounting foundation and the **Progressive Automations PA-14P-6-35** linear actuator mounted externally on top of the hull.

Working concept:
- Mount the actuator on the upper hull/deck, with its fixed end forward and the actuator rod pointing toward the stern.
- Current visual estimate: when retracted, the actuator/rod reaches approximately the transom. Consider angling the actuator from a forward attachment point near the hull centerline toward the **port stern corner**, rather than aiming straight aft at 90 degrees to the transom.
- Two motivations for a portward angle: (1) target the actuator's **3 in midpoint of its 6 in stroke** as the nominal straight-ahead rudder position, and (2) preserve more access to a planned hatch in the starboard aft section. Mid-stroke alignment is a design target, not yet a verified geometric fact.
- Design a new rudder/rudder stock to use the existing transom bracket if feasible; no legacy rudder/tiller hardware is available.
- A tiller arm attached to the rudder stock could extend approximately toward port, with the actuator rod-end connecting near its outboard end.
- The actuator would push/pull the tiller arm to rotate the rudder stock. The required tiller-arm radius and attachment geometry must be calculated from the intended rudder angle and measured actuator endpoints; do not assume a 6 in stroke automatically gives an appropriate rudder range.
- Keep the starboard aft hatch access in mind when selecting actuator angle and routing wiring.

Important design caveats / next validation:
1. Confirm actual actuator overall length and rod-end location at fully retracted, 3 in extension, and fully extended, including pivot/clevis offsets.
2. Establish the rudder-stock axis location on the stern bracket and whether the bracket can safely support the rudder loads.
3. Choose a preliminary rudder angle range and tiller-arm radius, then solve the actuator-to-tiller geometry at both travel limits. Ensure no dead-center/toggle condition, interference, or actuator side-loading across the range.
4. The actuator's midpoint can be assigned to straight-ahead by design, but its rod end does not have to physically lie at the transom at midpoint. Prioritize a symmetric and mechanically sound steering range.
5. Test the complete linkage dry, with mechanical end stops and a way to prevent the actuator from driving against the ends of travel. Verify the PA-14P feedback/control requirements and provide a suitable reversible actuator driver; it cannot be driven directly from an Arduino pin.

This is the current steering concept, not a finalized design. Preserve flexibility to change actuator angle, tiller arm direction/length, and rudder-stock placement after measurements and a simple linkage layout are completed.

---

### 2026-10-03 — U10 mounting footprint observations

The user physically compared the APISQUEEN U10 mounting base to the hull bottom and noted that the required contact/mounting area can be relatively small:

- Approximate U10 base length: **4-1/2 in**
- Approximate U10 base width: **1 in**
- Distance from the back/outside edge of the rearmost mounting-hole region to the front/outside edge of the forwardmost mounting-hole region: **~1-3/4 in**
- Mounting holes are arranged approximately in a **kite-shaped pattern**.
- Distance across the outside ends of the two wider hole features: **a little over 3/4 in**.
- The U10 mounting surface exposed to the hull therefore does **not** need to be a large plate solely to accommodate the U10 footprint.

For initial design work, treat the transverse V geometry as effectively unchanged over the U10's small ~4-1/2 in footprint, and treat the longitudinal/front-to-back bottom slope as negligible over that same local region. This is an engineering simplification for the U10 mount, not a claim that the entire hull bottom is planar.

The current mounting concept should focus on a **small, structurally reinforced contact/mounting interface** that conforms to the local ~12.53° transverse V rather than attempting to cover a large area of the hull bottom.

---

### U10 exact specification baseline — user-provided

User supplied an APISQUEEN U10 specification table. Preserve these values as the current exact reference for the specific U10 being considered:

| Specification | U10 |
|---|---|
| Size | **117.5 × 95.5 mm** |
| Support voltage | **24 V (6S LiPo)** |
| No-load current | **1.9 A** |
| Load current | **40 A** |
| Motor KV | **300 KV** |
| Power | **900 W** |
| Stated thrust | **10 kg at 24 V** |
| Color | **Black** |
| Included ESC rating | **100 A continuous** |
| Included ESC BEC | **5 V / 1 A** |
| Wire diameter | **9 mm** |
| Mounting hole specification | **M3** |
| Mounting-hole spacing shown | **20 × 16 mm** |
| Weight | **800 g in air / 350 g in water** |
| Working environment | **Fresh water / sea water** |

**Mounting note:** M3 fasteners are a common metric hardware size and should be straightforward to source. The exact screw length should be selected after the propulsion mount/plate thickness is established so that the fastener provides adequate thread engagement without being unnecessarily long. Stainless-steel M3 hardware is a likely marine-mounting starting point, with washers/locking method selected based on the final mount design. Do not assume the listed 20 × 16 mm dimension is a hole diameter; treat it as the manufacturer's mounting-hole spacing/layout until the physical U10 is inspected.


### Rough hull-sizing interpretation

The U10's maximum published forward thrust (~22.9 lb) is slightly greater than the empty hull shell's 20 lb mass, but thrust-to-weight is **not** a boat performance criterion by itself. More useful is that the U10 can produce approximately:
- 10.1 lb at 14 V
- 12.3 lb at 16 V
- 15.2 lb at 18 V
- 17.9 lb at 20 V
- 20.1 lb at 22 V
- 22.9 lb at 24 V

Therefore the U10 has substantial thrust headroom relative to a likely small all-up boat, but actual required thrust cannot yet be calculated reliably because waterline length, waterline beam, draft, hull form, target speed, and all-up displacement are not yet known.

For a rough displacement sanity check only, if the 5 ft 8 in overall length is close to waterline length and the full 20 in beam is used as a rough waterline beam, a preliminary block-coefficient assumption of Cb≈0.4 gives approximately 6.3 cm draft at 50 lb all-up weight and approximately 8.8 cm at 70 lb. These are rough geometric estimates only and should not be used for construction or stability decisions until the actual hull geometry is measured.

The classical displacement-hull speed estimate using 5 ft 8 in as waterline length is approximately 3.2 knots / 3.7 mph / 5.9 km/h. This is a rule-of-thumb, not a hard speed limit; the actual hull may behave differently, particularly if its form permits semi-planing/planing operation.

### BEC terminology / power architecture

**BEC = Battery Eliminator Circuit.** It is a regulated low-voltage power supply built into an ESC. In the U10 ESC, the published BEC output is **5 V / 1 A**. It can potentially power low-current receiver/electronics loads, but it should **not** be assumed adequate for the HiTec D646WP steering servo because the D646WP's documented stall current is 2.2 A at 7.4 V. A separate appropriate servo/electronics regulator remains a design item.

### Current Path #1 architecture

```
T8L → ER8 → Arduino
              ├── PWM → U10 100A ESC → fixed underwater U10 thruster
              └── PWM → HiTec D646WP → rudder
```

The primary attraction is that the U10 ESC uses a documented RC-style PWM interface, so the Arduino can command propulsion without proprietary protocol reverse engineering. The low underwater motor mass is also attractive for keeping heavy mass out of the topside structure, although overall stability still depends on the complete center-of-gravity and center-of-buoyancy relationship.

The next sizing inputs to obtain are the hull's actual waterline length/beam, draft or displacement at representative loads, and a realistic target speed. No propulsion purchase is justified solely from the current rough calculations.


### 2026-10-06 — POC 6 U10 + waterproof bidirectional ESC bench/water test setup

The following propulsion hardware is now confirmed **on hand / purchased for the project**:

#### APISQUEEN U10 underwater brushless thruster
- Product: **APISQUEEN U10 Underwater Brushless Thruster**
- Specification baseline: **24 V, 900 W, 10 kg stated thrust**
- Physical status: **on hand**
- The U10 has been attached to a long piece of wood that serves as a temporary handling/positioning fixture. This allows the thruster to be submerged in water while keeping the electrical connections above/outside the water.
- The U10's supplied motor lead is the manufacturer's stated **14 AWG, 1 m, 3-core lead with 4.0 mm banana plugs**.
- Intended POC 6 purpose: demonstrate controlled underwater operation and validate the U10 + ESC + bench-power chain before integrating propulsion into the hull.

#### APISQUEEN 2–6S 100 A Feather ESC with 5.5 V / 3 A BEC
- Product: **APISQUEEN 2–6S 100 A Feather ESC**
- Purchased variant: **Waterproof 100 A ESC, Bi-directional control**
- Published electrical range: **2–6S**
- Published continuous current: **100 A**
- Published BEC: **5.5 V / 3 A**
- Physical status: **purchased; currently in transit / not yet received**
- Intended POC 6 purpose: drive the U10 from the NANKADF GC3010H bench power supply and accept a PWM command from the Arduino/control chain.
- This is distinct from the older U10 page's generic/legacy references to a 100 A ESC with a 5 V / 1 A BEC. For the project's actual POC 6 hardware, use the **purchased waterproof bi-directional ESC's published 5.5 V / 3 A BEC specification**.

The manufacturer's ESC documentation describes the bidirectional PWM command as approximately:
- **1.0 ms = maximum reverse**
- **1.5 ms = stop**
- **2.0 ms = maximum forward**
- Published compatible PWM frequencies include **50, 100, 200, and 500 Hz**

The manufacturer's U10 page identifies the operating environment as **freshwater / seawater** and specifies the U10 motor lead and 24 V / 900 W class. The current POC 6 setup therefore uses the U10 submerged in water while the ESC and bench-power connections remain out of the water.

### POC 6 — U10 underwater thruster + waterproof bidirectional ESC

**Goal:** Prove that the physical propulsion chain works safely and controllably before committing to a permanent hull installation.

Planned chain:

```
NANKADF GC3010H bench supply
          ↓
waterproof 100 A bidirectional ESC
          ↓
       U10 thruster
          ↓
       submerged in water

Arduino PWM
     ↓
ESC signal input
```

The POC 6 sequence should initially emphasize **low-risk functional verification**, not maximum-thrust testing:
1. Verify the ESC wiring, polarity, connectors, and signal connection before applying propulsion power.
2. Confirm the bench supply can provide the required voltage/current for the intended test range.
3. Start with the U10 submerged and mechanically restrained by the temporary wood fixture.
4. Establish the ESC's stop/neutral command before commanding forward or reverse.
5. Increase command gradually and observe motor response, bench-supply current, ESC behavior, and mechanical stability.
6. Keep the first runs brief and avoid unnecessary full-throttle operation.
7. Record voltage, current, PWM command, direction, and qualitative thrust/behavior observations.
8. Do not treat the bench test as proof that the eventual hull mount is structurally adequate; the permanent U10 mount remains a separate mechanical-engineering task.

**Important test constraint:** the U10 must be operated **submerged in water** for powered testing. The water provides the intended operating environment and load; do not run the thruster at meaningful power in air.

**Current POC 6 status:**
- U10: **ON HAND**
- Temporary submersion/handling fixture: **READY**
- Waterproof bidirectional 100 A ESC: **IN TRANSIT**
- NANKADF GC3010H bench supply: **ON HAND**
- Arduino Uno Rev3: **ON HAND**
- POC 6 powered test: **WAITING FOR ESC ARRIVAL**


### 2026-10-06 — POC 6 Arduino sketch created

The first POC 6 propulsion-control sketch is now committed to the repository:

- Sketch: `sketches/01_basics/06_u10_esc_throttle/06_u10_esc_throttle.ino`
- README: `sketches/01_basics/06_u10_esc_throttle/README.md`

Signal chain implemented:

```
T8L left joystick Y (throttle)
        ↓
      ER8 CH3
        ↓
   Arduino D3
        ↓
   Arduino D9
        ↓
waterproof bidirectional ESC
        ↓
       U10
```

The sketch:
- Uses Arduino D3 for ER8 CH3 throttle input.
- Uses Arduino D9 for the ESC PWM signal.
- Commands approximately 1000 µs reverse, 1500 µs neutral, and 2000 µs forward.
- Starts and remains at 1500 µs neutral.
- Requires the transmitter throttle to be near center before enabling throttle control.
- Commands neutral if the receiver signal is lost.
- Uses the existing T8L/ER8 observed calibration baseline of approximately 984 / 1493 / 2002 µs.
- Is deliberately conservative for the first physical U10 test.

The first POC 6 test should validate the signal chain and ESC behavior at low power before any full-throttle testing. The U10 is to remain submerged in water and mechanically restrained by the temporary wooden fixture during powered testing.


---

## 2026-10-09 — Power-system architecture and initial shopping list

The user clarified the final-boat packaging and runtime targets:

- Central internal compartment: approximately **16 × 16 × 6 in**.
- A second aft compartment of similar size is available. The central and aft compartments may be used separately or together to spread mass and improve fore-aft balance.
- These are planning envelopes, not hard caps; do not fill the full 6 in height by default. Weight, center of gravity, balance, service access, and water protection matter.
- The external components are the **PA-14P-6-35 actuator**, **U10 thruster**, and rudder/tiller assembly.
- Target runtime reduced from one hour to approximately **30 minutes**, with frequent high-power propulsion use assumed for conservative sizing.
- **HiTec D646WP servo is excluded from the final build**. It was used for POC testing only; the current steering direction uses the PA-14P linear actuator.

### Battery sizing — preliminary napkin math

Use the U10 user-provided published 24 V test point of approximately **32 A** as a conservative high-power reference:

- (24 V × 32 A ≈ 768 W).
- (768 W × 0.5 h ≈ 384 Wh) for the U10 alone over 30 minutes at that point.
- Allowing approximately 10–15% for conversion losses and low-voltage loads gives roughly **425–450 Wh delivered**.
- Add a reserve for battery aging, voltage sag, and avoiding deep discharge: preliminary nominal battery target **~530–600 Wh**.
- A 6S LiPo is **22.2 V nominal** and **25.2 V fully charged**. (530–600 Wh ÷ 22.2 V ≈ 24–27 Ah).

**Preliminary battery target:** 6S, approximately **25–30 Ah**, with documented continuous pack discharge capability of at least **50 A**, preferably **60 A or more**. This is a planning estimate, not a guaranteed runtime. Confirm pack current/thermal specifications, mass, dimensions, chemistry, charger, protection, and measured U10 draw before purchasing. Do not use ordinary low-current 18650 cells/packs for U10 propulsion.

### Proposed power architecture

A visual diagram and its calculation notes are in:
- `docs/power-system/ASV_Power_System_Architecture.svg`
- Shopping list: `docs/power-system/ASV_Power_System_Shopping_List.md`

Working architecture:

- **Main battery:** 6S high-discharge pack; main DC-rated disconnect, fuse, and distribution.
- **Propulsion branch:** main battery → branch fuse/wiring → existing APISQUEEN 2–6S waterproof 100 A bidirectional ESC → U10.
- **Actuator branch:** battery bus → branch fuse → regulated 12 V DC-DC converter → reversible brushed-DC motor controller → PA-14P motor leads.
- **Control branch:** battery bus → regulated 5 V converter → Arduino Uno and ER8, subject to their input specifications and the final grounding/power-path design.
- **Control signals:** Arduino output → actuator motor controller; PA-14P potentiometer feedback → Arduino.
- Establish common signal ground where required by the chosen controller/converter topology. Do not tie isolated power negatives together without checking device documentation.
- Do not use an ESC BEC and a separate 5 V converter in parallel without an intentional, verified power-path design.

### Initial actuator-controller shortlist

- **Mtroniks Viper Marine25 HV2** is the current waterproof marine brushed-controller candidate for the PA-14P. It remains a **shortlist item, not approved for purchase** until its exact input voltage range, LiPo/low-voltage behavior, current protection, startup/stall suitability for a linear actuator, failsafe/neutral behavior, and actual connector/lead types are verified.
- **Cytron MD20A** remains an alternative for a dry/sealed-enclosure installation, with enclosure and cable-entry work required.
- The smaller MicroViper Marine10 is not the preferred candidate for the planned 6S-battery architecture because it is a 12 V maximum controller and specifies “LiPo safe: No.”
- The 12 V actuator supply converter must tolerate the full 6S battery range (up to 25.2 V for LiPo) and be sized for actuator startup/stall behavior. **Victron Orion IP67 24/12-20** is a candidate to evaluate, not a finalized purchase.
- Select a separate regulated 5 V converter for Arduino + ER8 after checking input range, current capacity, ripple, enclosure/IP rating, and power-path arrangement.

### Connector and no-solder requirements

- Preserve the PA-14P factory cable; do not cut it.
- The current discussion identified a five-wire PA-14P cable (red, black, white, yellow, blue) and a Molex-style six-position connector matching the product image. Manufacturer documentation was cited as identifying Molex Mini-Fit Jr. mating housing **39013063**.
- **Important unresolved inventory conflict:** older inventory notes describe the used PA-14P unit as having a Deutsch DT06-6S connector, while the current photo/product-image discussion shows a Molex-style connector. There may be two different actuator units or a prior connector identification error. Inspect the exact actuator unit being tested and verify pin orientation/pinout before ordering any mating connector or applying power.
- Prefer a verified prewired mating pigtail/breakout and factory-lead controller/converter products. Avoid generic Dupont jumpers unless their connector gender and pitch are verified end-to-end.
- The Viper's receiver-style signal lead is not assumed to connect directly to the Arduino; verify its connector and use a compatible premade signal breakout/extension if needed.
- Final fuse ratings, wire gauges, and connector ratings remain TBD until cable lengths, current/transient behavior, branch loads, and installation conditions are established. Existing 18 AWG wire must not be assumed suitable for the U10 high-current path.

### Shopping list tracking

A detailed candidate BOM, purchase priorities, open verification items, and a staged “get the actuator moving first” list are maintained in `docs/power-system/ASV_Power_System_Shopping_List.md`. Treat it as a working shortlist, not authorization to buy all items. Initial purchasing should focus on the actuator controller and verified mating/signal/motor wiring; the 12 V converter is needed for the final 6S-powered architecture but is not necessarily needed for a current-limited actuator bench POC because the NANKADF GC3010H can supply regulated 12 V up to 10 A.



### 2026-10-10 — Connector policy clarified for the new PA-14P

The user explicitly clarified the following connector decisions:

- The actuator being used for this build is a **new PA-14P with a Molex-style 6-position connector**. Ignore the earlier inventory note about a Deutsch connector on the used actuator; it refers to a different/older unit.
- The actuator-side mating interface must therefore be the correct **Molex Mini-Fit Jr. 6-position mating housing**, manufacturer part reference **39013063**, or a verified pre-crimped mating pigtail/breakout. Do not modify or cut the factory actuator cable.
- The project already has a useful stock of Deutsch connectors, compatible contacts/seals and a working crimping tool; the user has successfully crimped with the tool.
- **Default to existing Deutsch DT connectors for detachable low-/medium-current wiring interfaces whenever practical**, after the Molex actuator pigtail/breakout. This should be the default for future connector recommendations rather than introducing new connector families unnecessarily.
- Before using Deutsch on any specific circuit, verify the exact contact family, conductor gauge, connector/cavity arrangement, and current rating against that circuit's expected continuous/transient current. Do not assume all Deutsch housings/contacts or all wire gauges are interchangeable.
- Do not force Deutsch connectors onto the high-current battery/propulsion path if a purpose-built, appropriately rated high-current connector is more suitable.
- Maintain the project's broader rule: minimize soldering, avoid unnecessary tool/connector purchases, and check exact connector gender, pitch, terminal family, pinout, and end-to-end mating compatibility before recommending a part.

The shopping list at `docs/power-system/ASV_Power_System_Shopping_List.md` has been updated to reflect this connector policy and to remove the false conflict about the new actuator.

### 2026-10-13 — SyRen 10 motor driver purchased

- **Purchased:** Dimension Engineering **SyRen 10 regenerative brushed-DC motor driver**, ordered from RobotShop (user reports listing showed 12 units in stock and estimated shipping of 4–10 days).
- Product listing: https://www.robotshop.com/products/dimension-engineering-syren-10
- Manufacturer documentation: https://www.dimensionengineering.com/products/syren10
- Intended use: bidirectional control of the Progressive Automations PA-14P-6-35 12 V linear actuator, commanded by the Arduino Uno. The Arduino will read the actuator's potentiometer and implement steering position control/software limits; the SyRen drives motor power/direction and does not itself interpret the PA-14P potentiometer as a target position.
- Published actuator current is 1 A no-load and 5 A full-load; exact startup/stall current remains unknown. SyRen 10 is a 10 A continuous-class driver with overcurrent and thermal protection, but do not assume its protection makes prolonged stall safe or that its current limit is user-adjustable.
- The SyRen is **regenerative**. Manufacturer guidance warns that a typical bench power supply cannot absorb returned energy; use a suitable battery-based test supply or a manufacturer-approved supply/battery arrangement for initial powered testing. Do not assume the NANKADF GC3010H alone is suitable during braking/reversal.
- Leave the SyRen's 5 V BEC unused for now. Power Arduino/ER8 from the planned separate regulated supply; avoid paralleling 5 V sources.
- Next steps when it arrives: inspect exact board/revision and included connectors; verify no-solder signal hookup and terminal requirements; read the exact manual; confirm PA-14P pinout before wiring; design a fused, current-aware bench setup; test short extension/retraction commands first; then develop feedback control and fault handling.
- Waterproofing is no longer a requirement for the motor driver; final installation can use a protected enclosure inside the hull.

### 2026-10-16 — POC 6 throttle input and conservative test sketch updated

The POC 6 sketch was updated in the repository before the first powered U10 test.

- Sketch: `sketches/01_basics/06_u10_esc_throttle/06_u10_esc_throttle.ino`
- Control chain: T8L **right joystick Y (spring-centered)** → ER8 **CH2** → Arduino **D3** → Arduino **D9** → APISQUEEN bidirectional ESC → U10.
- Steering is intentionally out of scope for this test.
- Arduino remains USB-powered; the ESC BEC powers the ER8. The ER8, Arduino, and ESC control signal share a common ground.
- Sketch starts at 1500 µs neutral, requires valid centered CH2 input before enabling, commands neutral on invalid/lost receiver signal, and reports CH2 pulse width and ESC command over Serial at 115200 baud.
- Initial ESC command range is deliberately limited to **1450–1550 µs** (±50 µs around nominal neutral) to attempt only a very-low-throttle forward/neutral/reverse functional check. This range may fall inside the ESC's neutral deadband; if the thruster does not spin, do not increase the range blindly.
- The CH2 calibration baseline (approximately 984 / 1493 / 2002 µs) and assumed direction mapping are provisional for this test and should be checked in Serial Monitor and with brief, restrained tests.
- Before powering: verify ESC input polarity, correct ER8 power-pin orientation and BEC compatibility, secure the U10 and fully submerge it. NANKADF GC3010H is limited to 10 A; this is not a full-power U10 test.
- Status: **Sketch updated; physical test not yet reported as run.**



### 2026-10-16 — POC 6 receiver signal timeout diagnostic update

During the first Serial Monitor check, the Arduino correctly read ER8 CH2 across the stick's travel (roughly 970–1986 µs observed), but intermittent `NO VALID CH2 SIGNAL` messages caused repeated disarm/re-arm transitions while the stick was held still or moved.

- Increased `SIGNAL_TIMEOUT_US` in `sketches/01_basics/06_u10_esc_throttle/06_u10_esc_throttle.ino` from **25,000 µs (25 ms)** to **50,000 µs (50 ms)** as a diagnostic to allow more time for `pulseIn()` to synchronize with the repeating receiver PWM signal.
- The existing fail-safe remains: a timeout or out-of-range pulse commands ESC neutral and disarms control.
- The existing centered-stick interlock and 1450–1550 µs ESC command limits are unchanged.
- This is a diagnostic change, not a confirmed root-cause fix. Re-test with the U10 disconnected; if dropouts persist, inspect wiring/ground/power and consider a more robust receiver-pulse capture implementation.
- Status: **Code updated; revised sketch has not yet been physically tested.**
