# USV Power-System Shopping List

**Status:** Working shortlist — not a purchase authorization.  
**Last updated:** 2026-11-06
**Scope:** Manual RC boat MVP; no steering servo is used in the boat. The D646WP was bench POC hardware only. The PA-14P-6-35 actuator through the SyRen 10 provides boat steering.
**Hull packaging:** central compartment approximately 16 × 16 × 6 in; a similar aft compartment is available. These are planning envelopes, not hard limits. Keep mass low and balance fore/aft.

## Design target and battery napkin math

The U10's user-provided specifications list 24 V, 900 W, and 40 A load current. Its published 24 V test point was approximately 32 A.

- Reference high-power point: 24 V × 32 A = **768 W**.
- 30 minutes at that point: 768 W × 0.5 h = **384 Wh** for the U10 alone.
- Allow roughly 10–15% for conversion losses and low-voltage loads: approximately **425–450 Wh delivered**.
- Add reserve for battery aging, voltage sag, and avoiding deep discharge. A preliminary nominal pack target is approximately **530–600 Wh**.
- A 6S LiPo pack is 22.2 V nominal and 25.2 V fully charged. 530–600 Wh ÷ 22.2 V ≈ **24–27 Ah**.

**Preliminary battery target:** 6S, about **25–30 Ah**, with a documented continuous discharge capability of at least **50 A**; prefer **60 A or more** of continuous pack capability. This is a conservative planning estimate, not a runtime guarantee. The U10 may draw less than 32 A at some operating points and more during transients; validate with current measurements during POC 6. Check pack voltage limits, low-voltage protection, charger compatibility, physical dimensions, mass, and safety features before purchase.

## Proposed power architecture

See [USV_Power_System_Architecture.svg](USV_Power_System_Architecture.svg) for the architecture diagram and napkin math.

- Main battery: 6S high-discharge pack, nominal 22.2 V.
- Propulsion: main battery → appropriately sized main/branch fuse and disconnect → existing APISQUEEN 2–6S 100 A waterproof bidirectional ESC → U10.
- Actuator: battery bus → branch fuse → regulated 12 V DC-DC converter → reversible brushed-DC motor controller → PA-14P motor wires.
- Controls: APISQUEEN ESC BEC 5.5 V / 3 A → ordered XL6019 boost module adjusted to 8.0 V → Arduino VIN; Arduino 5 V rail → ER8 and PA-14P position-feedback potentiometer. Bench-verify the boost output and loaded BEC voltage. PA-14P motor remains on the separate SyRen/actuator power path.
- Signals: Arduino control output → actuator controller input; PA-14P potentiometer feedback → Arduino.
- Grounds: establish a deliberate common signal-ground reference as required by the chosen controller and converter topology. Do not assume every power negative must be tied together without checking isolation and manufacturer instructions.
- D646WP servo is **not used in the boat**; it was used for POC testing only. The Arduino 5 V rail powers only the ER8 and the PA-14P potentiometer; the actuator motor has a separate SyRen power path.

## Shopping list / candidate BOM

Statuses:
- **BUY FIRST** = needed to progress the actuator POC after the interface is verified.
- **SHORTLIST** = plausible final-boat candidate, not yet approved for purchase.
- **REQUIRED / VERIFY** = needed, but exact part or rating depends on measurements/manuals.
- **ON HAND** = already owned.

| Priority | Item | Target / candidate | Status and verification required |
|---|---|---|---|
| 1 | Reversible brushed-motor driver for PA-14P | **Dimension Engineering SyRen 10**, purchased from RobotShop on 2026-10-13 | **ON HAND; POC 7 configuration and actuator control are functional.** 10 A continuous-class regenerative brushed-DC driver. Arduino reads PA-14P feedback and implements position control. Retain the SyRen settings/commands from the working POC 7 sketch. |
| 1 | PA-14P mating connector / breakout | Manufacturer PA-14P documentation has identified Molex Mini-Fit Jr. 6-position housing **39013063** as a mating-housing reference | **REQUIRED.** The new PA-14P being used has the Molex-style 6-position connector; use the manufacturer-identified Molex Mini-Fit Jr. mating housing (39013063) or, preferably, a verified pre-crimped mating pigtail/breakout. Preserve the actuator's factory connector. After this Molex mating pigtail, default to existing Deutsch DT connectors for detachable harness interfaces where the contact family, wire gauge, current rating, and cavity/pinout are verified. Prefer pre-crimped/pigtail breakout; no cable cutting. |
| 1 | Controller-to-actuator motor leads | Two conductors from controller motor outputs to actuator motor pair | **REQUIRED.** Terminations must match the selected controller output and actuator breakout. Confirm motor wire colors/pinout from the exact actuator datasheet; do not infer polarity from color alone. |
| 1 | Arduino-to-SyRen control lead | Arduino SoftwareSerial TX on D8 and signal ground/reference to SyRen S1 and 0V | **REQUIRED.** Use the verified POC 7 serial interface and a connector/breakout that matches the SyRen terminals. This is not a steering-servo connection. |
| 1 | 12 V actuator DC-DC converter | Input must tolerate full 6S range (up to 25.2 V); regulated 12 V output; waterproof/potted marine unit preferred; preliminary 10–20 A class pending actuator stall-current verification | **SHORTLIST.** Victron Orion IP67 24/12-20 is a candidate to evaluate, not yet approved. Confirm actual input range includes 25.2 V, output rating and transient response, lead/connector types, and whether the controller/actuator startup or stall load is within limits. |
| 2 | Arduino boost regulator | XL6019 module; 5.5 V BEC input and adjustable 8.0 V output to Uno VIN | **ORDERED; not yet received or bench-checked.** The chip datasheet's 5 V minimum leaves only 0.5 V input margin from the BEC. Verify BEC voltage under load and adjust the module to 8.0 V with a multimeter before connecting it to VIN. The 5 A label is not 5 A of guaranteed output current. |
| 1 | Actuator branch fuse + holder | Fuse near battery distribution; final fuse based on converter input current, cable ampacity, and inrush | **REQUIRED / SIZE LATER.** Do not select a fuse solely from actuator's 5 A full-load current; consider startup/stall and DC-DC converter input current. |
| 1 | Main battery fuse and disconnect | DC-rated fuse, holder, and accessible disconnect sized for propulsion wiring and prospective current | **REQUIRED / SIZE LATER.** Coordinate with ESC, battery, cable ampacity, and fault-current capability. |
| 2 | 5 V control branch fuse | Small DC-rated fuse sized for converter and control wiring | **REQUIRED / SIZE LATER.** |
| 1 | Battery pack | 6S high-discharge LiPo or engineered Li-ion pack; preliminary 25–30 Ah; continuous discharge ≥50 A, preferably ≥60 A | **SHORTLIST / NOT PURCHASE-READY.** Compare total mass, dimensions, manufacturer-specified continuous current, connector, low-voltage protection, charge limits, and charger availability. Do not use ordinary low-current 18650 cells/pack for U10 propulsion. |
| 1 | Compatible battery charger | Charger specifically compatible with selected chemistry, cell count, balance connector and capacity | **REQUIRED AFTER battery selection.** |
| 1 | Main power distribution | DC-rated distribution block/bus with protected terminals, suitable for battery current | **REQUIRED / SELECT.** Prefer covered screw terminals or preterminated high-current leads; avoid loose uninsulated connections. |
| 1 | Wiring: battery/ESC branch | Marine-suitable flexible stranded copper cable sized from current, length, voltage drop, insulation temperature, and installation | **REQUIRED / GAUGE LATER.** Do not use existing 18 AWG wire for the U10 high-current path. |
| 1 | Wiring: 12 V actuator branch | Flexible stranded copper wire sized after measuring/confirming actuator stall current and cable length; marine-grade insulation preferred | **REQUIRED / GAUGE LATER.** Existing 18 AWG may be suitable for some short, low-current control circuits, but do not assume it is suitable for motor supply. |
| 1 | Control signal wiring | ESC PWM lead/breakout and female Dupont leads for Arduino connections | **REQUIRED / VERIFY.** Keep signal wiring separate from high-current motor wiring where practical. No steering servo is used in the boat. |
| 1 | Sealed cable glands / strain relief | Cable glands matched to cable diameters and enclosure wall thickness | **REQUIRED for final installation.** |
| 1 | Enclosure / mounting | Splash-resistant or sealed enclosure, internal mounting plate, strain relief, drip loops and ventilation/thermal strategy where required | **REQUIRED.** Central compartment is approximately 16 × 16 × 6 in; similar aft compartment is available. Layout should preserve service access and balance. |
| 1 | Battery monitoring | Low-voltage alarm/monitor suitable for selected pack; cell-level monitoring for lithium pack where appropriate | **REQUIRED / SELECT.** ESC cutoff alone should not be the only battery-health strategy. |
| — | Arduino Uno Rev3 A000066 | Existing unit | **ON HAND.** USB used for programming/prototype power. |
| — | Radiomaster T8L + ER8 | Existing control chain | **ON HAND.** |
| — | NANKADF GC3010H bench supply | 0–30 V, 0–10 A | **ON HAND.** Suitable for low-current actuator/controller bench experiments if set appropriately; not sufficient to test U10 at full rated current. |
| — | APISQUEEN U10 + waterproof 100 A bidirectional ESC | U10: 24 V; ESC: 2–6S, 100 A, 5.5 V/3 A BEC | **ON HAND / ESC purchased.** Confirm actual hardware/connector details on arrival. |
| — | Klein multimeter, heat shrink, Deutsch tooling/connectors, 18 AWG wire | Inventory as recorded in master context | **ON HAND; suitability varies by circuit.** |

## No-solder and connector principles

1. Preserve the PA-14P factory cable; do not cut it.
2. Verify which physical PA-14P unit is being used. Current connector descriptions conflict (Molex Mini-Fit Jr. vs prior Deutsch DT06-6S note).
3. Use a prewired mating pigtail / breakout for the actuator connector if available; verify exact housing, terminal, cavity orientation and wire functions before purchase.
4. Verify the selected Viper controller's actual signal connector and power/motor leads from its manual/product photos. Do not assume the signal connector mates directly with an Arduino.
5. Use properly rated crimped terminals, screw terminals, or manufacturer-fitted leads rather than solder where practical.
6. Do not select fuses, wire gauges, or connectors until current, run length, inrush/stall behavior, and installation conditions are known.

## First purchase tranche: get the actuator moving

Only buy after checking the exact actuator connector and Viper specifications:
1. **Purchased:** Dimension Engineering SyRen 10 regenerative brushed-DC motor driver; awaiting delivery.
2. Matching PA-14P mating connector with factory/premade leads, if the exact unit uses the documented Molex connector.
3. Compatible controller signal breakout / servo extension / Arduino female-jumper adapter.
4. Short, appropriately sized motor-output leads and terminals.
5. Temporary appropriately rated fuse and holder, selected after verifying controller and actuator current behavior.

The 12 V DC-DC converter is required for the final 6S-powered boat architecture, but it is not necessarily needed for the initial actuator bench POC because the NANKADF GC3010H can provide regulated 12 V at up to 10 A. Keep that test current-limited and stop if the actuator demands more than the supply can provide; the bench supply is not proof of full-load/stall capability.

## Open decisions / risks

- Verify the new PA-14P's exact pinout against the matching datasheet and connector orientation before connecting the motor or potentiometer wires.
- After the Molex pigtail, prefer existing Deutsch DT connectors for detachable harness breaks where the exact contact family and electrical ratings are suitable.
- Verify actuator stall current / startup behavior and duty cycle.
- On SyRen 10 arrival, verify exact model/revision, signal interface and no-solder connection options; review its regenerative power-source requirements before bench testing.
- Choose battery chemistry and specific pack only after comparing mass, volume, current rating, protection, charger and budget.
- Verify that the 6S battery pack, converters, ESC, fuses, cables and connectors fit the hull's central/aft compartments and can be placed for reasonable fore-aft balance.
- Do not assume a 30-minute high-power runtime until verified with water tests and measured battery draw.

## Purchase update — 2026-10-13

- **SyRen 10 purchased from RobotShop**, awaiting delivery. User observed listing stock of 12 units and estimated shipping of 4–10 days at time of purchase.
- Product: https://www.robotshop.com/products/dimension-engineering-syren-10
- Manufacturer: https://www.dimensionengineering.com/products/syren10
- Waterproofing is not required for this controller; plan a protected installation inside the hull.
- Arduino will implement position feedback and software limits using the PA-14P potentiometer. SyRen 10 drives the motor in either direction; it is not itself the position controller.
- **Bench-test caution:** SyRen 10 is regenerative; use a suitable battery-based source or manufacturer-approved battery/supply arrangement. Do not assume the NANKADF GC3010H alone can absorb returned energy during braking/reversal.
- **Still open:** verify signal wiring and terminal/no-solder setup when delivered; confirm exact PA-14P connector pinout before wiring; choose appropriate fuse/wire; characterize actuator current safely without prolonged stall.
