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
