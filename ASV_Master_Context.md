# ASV Master Context

**Status:** Living working document  
**Purpose:** Single source of truth for the user's from-scratch Autonomous Surface Vessel (ASV) build. Update this file as hardware, architecture, wiring, tests, and decisions change.

> **Working rule:** When advising on this project, use the exact hardware and specifications documented here whenever possible. Avoid generic assumptions when the user's actual part number/spec is known. If a required detail is unknown, explicitly mark it as unknown rather than silently substituting a typical component.

---

## 1. Project Overview

The project is a from-scratch, hands-on Autonomous Surface Vessel (ASV) build.

### Design philosophy

- Start simple.
- Build in small, bite-sized proof-of-concepts (POCs).
- Avoid rebuilding the old system's complexity.
- Prefer readily understandable, serviceable architecture.
- Avoid soldering where practical; use appropriate crimped connectors, pin/socket connections, and off-the-shelf wiring.
- Validate each subsystem independently before integrating it.
- Use the actual inventoried hardware as the design baseline.

### Current MVP control concept

The initial control MVP has two operator inputs:

1. **Throttle:** power/speed command for the propulsion motor/engine.
2. **Steering:** left/right command for the rudder/prop steering mechanism.

Preferred operator interface:

- RC transmitter with two distinct controls/knobs or equivalent independent axes.
- RC receiver provides the control signals.
- A microcontroller (initially Arduino Uno R3) will interpret receiver signals and command the actuators/motor-control hardware.

Autonomy comes later. The immediate goal is reliable manual control and subsystem understanding.

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

#### High-torque waterproof servos

**HiTec D646WP**

- High torque
- 32-bit
- IP67
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

### 3.8 Wire / electrical tools

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

The user has a **bench power supply** available.

Current intended prototype approach:

- Arduino Uno: powered from USB while programming/testing where appropriate, or bench supply when appropriate.
- Receiver + servo prototype: bench-powered.
- T8L: powered by its dedicated **2 × 18650** cells.
- Boat propulsion/engine power: **not yet being designed** and should be treated as a separate future workstream.

Do not conflate the transmitter battery system with the boat's eventual propulsion battery system.

---

## 5. Current Control Architecture

### Planned signal chain

**T8L transmitter → ER8 ELRS PWM receiver → Arduino Uno → control output → actuator/motor-control system**

Initial POC should establish:

1. T8L transmits commands.
2. ER8 receives them.
3. Arduino reads the receiver PWM channels.
4. Arduino maps the channels to understandable control values.
5. Arduino produces a test output.
6. A servo can be commanded from that output.

Only after this works should the system be connected to more consequential hardware.

### MVP control mapping

- Channel/input A: throttle
- Channel/input B: steering

Exact ER8 channel numbers and T8L control assignments should be recorded after the actual hardware is configured and tested.

---

## 6. POC Roadmap

### POC 1 — Arduino basics

Goal: establish the development environment.

- Install/open Arduino IDE 2.3.10.
- Connect Arduino Uno Rev3 via USB-B.
- Confirm board/port selection.
- Upload a basic sketch.
- Confirm serial output / basic I/O.

**Status:** In progress / setup phase.

### POC 2 — Receiver signal visibility

Goal: prove the ER8 is receiving the T8L commands.

- Power ER8 from an appropriate bench supply.
- Bind ER8 to T8L.
- Connect one PWM output to an Arduino input.
- Read pulse width.
- Print pulse width over Serial Monitor.
- Move the assigned transmitter control and confirm the measured signal changes.

### POC 3 — Receiver → servo

Goal: command a servo based on an RC channel.

- Read ER8 PWM input.
- Map PWM to servo command.
- Connect a known servo.
- Verify center/min/max behavior.
- Establish safe limits.

### POC 4 — Two-channel manual control

Goal: demonstrate simultaneous throttle + steering inputs.

- Read two ER8 PWM channels.
- Display both values.
- Verify independent operation.
- Define neutral/min/max values.

### POC 5 — Control-output hardware

Goal: replace the simple servo test with the appropriate actuator/motor-control interface.

- Determine exact propulsion hardware.
- Determine steering actuator/mechanism.
- Select appropriate driver/interface.
- Add appropriate power distribution and protection.

### POC 6 — Bench-scale propulsion/steering

Goal: demonstrate actual propulsion and steering hardware without putting the system on the water.

### POC 7 — Hull integration

Goal: install the simplified system in the hull.

### POC 8 — Manual water test

Goal: verify basic remote-controlled operation.

### POC 9 — Autonomy

Autonomous navigation/control should be treated as a later phase after reliable manual control exists.

---

## 7. Decisions / Constraints

### Confirmed preferences

- Build from scratch rather than reproduce the legacy electronics system.
- Keep the architecture simple.
- Prefer incremental POCs.
- Minimize soldering.
- Prefer Deutsch connectors.
- Minimize the number of connector types/tools.
- Use the user's actual hardware inventory as the design baseline.
- Use the bench power supply during early electronics prototyping.
- Keep T8L battery power separate from boat power discussions.

### Avoid

- Designing around unidentified components without first confirming their specifications.
- Assuming a connector is electrically compatible solely because it physically mates.
- Assuming the legacy hull electronics represent the desired architecture.
- Jumping directly to high-power propulsion testing before validating the signal chain.

---

## 8. Open Questions / To Verify

1. Exact **ER8** model/version and PWM output specifications.
2. Exact ER8 power-input requirements.
3. Exact T8L channel/control mapping after setup.
4. Quantity and exact configuration of HiTec D646WP servos.
5. Exact specifications of the **GWIN MOTOR**.
6. Exact specifications of the legacy motor/propulsion system in the hull.
7. Whether the hull's existing Deutsch connectors can be reused.
8. Identity/function of the long metal tube through the hull.
9. Final steering mechanism.
10. Final propulsion architecture.
11. Marine-grade status of the user's 18 AWG wire.
12. Final fuse ratings and power-distribution architecture.
13. Boat-side battery chemistry, voltage, capacity, and BMS/protection requirements.
14. Exact connector gender/pinout for every component before integration.

---

## 9. Inventory Update Format

When a new item is acquired or identified, record it using:

| Category | Manufacturer | Exact Model / Part # | Qty | Key Specs | Connector | Condition | Intended Use | Status |
|---|---|---|---:|---|---|---|---|---|
| Example | Manufacturer | Part number | 1 | Voltage/current/etc. | Deutsch DTxx | New/Used | Purpose | TBD |

For every electrical component, capture **voltage, current, signal type, connector, polarity/pinout, and relevant environmental rating** when known.

---

## 10. Change Log

### 2026-09-16
- Created initial ASV Master Context.
- Established the document as the working source of truth for the project.
- Recorded known hull specifications.
- Recorded current RC/Arduino architecture.
- Recorded known transmitter, receiver, Arduino, servo, actuator, connector, battery, wiring, and tool inventory.
- Established incremental POC approach.
- Separated T8L transmitter power from future boat/propulsion power.
