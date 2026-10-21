# SyRen 10 — Project Documentation

**Purpose:** Practical reference for using the Dimension Engineering SyRen 10 with the PA-14P actuator in POC 7. This document focuses on the facts that affect our wiring, configuration, power safety, and Arduino control. It is not a general robotics guide.

**Project use:** Arduino Uno → SyRen 10 → Progressive Automations PA-14P-6-35 actuator. The T8L left joystick's horizontal **X axis** supplies the target position through the ER8 receiver. The Arduino is intended to implement closed-loop position control using the actuator's built-in potentiometer feedback.

## 1. Official source material

- [SyRen 10 product page](https://www.dimensionengineering.com/products/syren10)
- [Dimension Engineering Arduino libraries](https://www.dimensionengineering.com/info/arduino)
- [Using a SyRen with an ATX or similar DC power supply](https://www.dimensionengineering.com/appnotes/atxguide/atxguide.htm)
- [SyRen 10 / SyRen 25 User's Guide (July 2007 PDF)](https://www.dimensionengineering.com/datasheets/SyRen10-20.pdf)
- [SyRen 10 Quick Start Guide (April 2005 PDF)](https://www.dimensionengineering.com/datasheets/SyRen10-20.pdf)
- The user's uploaded reference copies: `SyRen10QuickStart.pdf` and `SyRen10-25.pdf`.
- [SyRen DIP switch wizard](https://www.dimensionengineering.com/products/syren10) (linked from the manufacturer's product page).

This document summarizes the manufacturer's product page, the uploaded Quick Start Guide and User's Guide, and the linked Arduino and ATX-supply resources. If the physical unit's revision or the DIP wizard gives a different configuration, pause and reconcile it against the exact unit/manual before applying power.

## 2. What the SyRen does

The SyRen 10 is a **single brushed-DC motor driver**. It controls motor direction and drive level; it does not by itself implement target-position control for a linear actuator.

Relevant manufacturer specifications:

- Input range: **6–24 V nominal**, **30 V absolute maximum**.
- Current rating depends on input voltage and cooling:
  - Up to **10 A continuous / 15 A peak** at input voltages up to 18 V under the manual's stated conditions.
  - At **24 V**, the product page specifies **8 A continuous**, or **10 A continuous with additional heatsinking/airflow**, and **15 A peak**.
- Built-in overcurrent and thermal protection.
- Synchronous regenerative drive.
- 32 kHz switching frequency.
- Screw terminals; DIP switches select operating mode and options.

These are driver ratings, not a statement that the actuator or wiring can safely draw those currents. Do not treat thermal/overcurrent protection as a substitute for correctly rated wiring, fusing, or a suitable power source.

## 3. Terminal reference

Use the labels printed on the actual board. In the orientation previously described by the user:

| Terminal | Function |
|---|---|
| `B+` | Positive motor-driver supply |
| `B−` | Negative motor-driver supply |
| `M1`, `M2` | The two brushed-motor leads |
| `S1` | Primary control input |
| `S2` | Secondary input; use depends on mode/options |
| `0V` | Signal ground; internally connected to `B−` |
| `5V` | Regulated 5 V output for small control loads, within its limits |

**Critical polarity warning:** The manual says reversing the supply connections can destroy the SyRen and void the warranty. Verify B+ and B− against the silkscreen before connecting power.

The manual says S1 is the primary input and must always be connected. S2 is mode-dependent. In analog mode, control wiring must be connected before applying power because the motor may start unexpectedly otherwise. For our intended serial setup, connect Arduino TX to S1 and share signal ground between Arduino and SyRen.

## 4. Regenerative operation and bench-supply safety

### What regeneration means

The SyRen returns energy toward its supply when decelerating or reversing, and also returns inductive energy during switching. A battery can absorb this energy. A typical bench/ATX supply may only source current and may not be able to sink returned current, which can cause its output voltage to rise or trigger protection.

The manufacturer's product page explicitly recommends **using a battery, or at least putting a battery in parallel with a DC supply**. Its ATX application note explains that the battery absorbs regenerated energy that could otherwise trip the supply's protection.

### What this means for our NANKADF GC3010H

- The NANKADF is a **test supply**, not part of the final boat; we do not need to build an elaborate permanent bench-power system.
- However, the supplied documentation does **not** establish that the NANKADF can sink current.
- The manufacturer's guidance does **not** endorse a plain, unbuffered supply as the power source for a regenerative SyRen. The product page says a battery or supply-plus-battery arrangement should be used.
- Therefore, **do not run the SyRen + actuator from the NANKADF alone until a safe, manufacturer-consistent arrangement is confirmed**.
- Do not improvise by paralleling a battery and supply. The manufacturer ATX note gives a specific 12 V sealed lead-acid battery arrangement for an ATX supply; that is not automatically a verified wiring recipe for the NANKADF, a different voltage, or our exact test setup.
- Our inventoried legacy NiMH packs are not established as a suitable 12 V buffer for this setup. Do not use them without checking their actual voltage, condition, capacity, current capability, and the manufacturer's intended arrangement.

**Practical decision:** Keep the POC simple, but do not compromise the power arrangement to avoid a small purchase. First ask whether we can safely use a suitable battery already available or whether we need a modest, temporary test solution. If no appropriate battery/source arrangement is available, defer the powered motor test while continuing non-powered wiring and software work. We do not need to solve the final boat battery system now.

### Voltage limit option in packetized serial mode

The manual documents a packetized-serial maximum-voltage setting intended for supplies that cannot sink current. If input voltage rises above the configured threshold, the SyRen hard-brakes the motor until voltage drops below the threshold. The manual warns this is inefficient and heats the motor instead of returning energy to a battery. This is **not a substitute for confirming the correct power arrangement**, and it is only relevant if packetized serial is selected. We should not switch protocols solely to use this feature without a reason.

## 5. POC 7 control mode

Our current preference is **simplified serial control from the Arduino**, unless the DIP wizard or a deliberate design choice points us elsewhere.

Why it fits:
- It is a simple one-way serial command interface.
- The Arduino's TTL serial TX output can connect directly to S1; a PC-style RS-232 level converter is not needed for an Arduino TTL UART.
- It avoids needing the Arduino to generate a motor-driver RC pulse signal.
- Only S1 is needed for ordinary simplified serial; S2 is used only if selecting the slave-select option.

Manual's simplified serial settings:
- Protocol: TTL-level serial, **8 data bits, no parity, 1 stop bit (8N1)**.
- Baud rate selected by DIP switches 4 and 5: **2400, 9600, 19200, or 38400**.
- Manufacturer recommends starting at **9600 or 19200 baud**; lower the baud rate if communication is unreliable.
- Maximum effective update speed is approximately 2000 commands/second.
- Standard simplified serial uses a single command byte; switch 6 selects standard mode versus slave-select behavior.

### Important command-byte discrepancy to resolve

The supplied Quick Start Guide (page 2) says simplified serial uses **0 = full reverse, 128 = stop, 255 = full forward**. The supplied July 2007 User's Guide is internally inconsistent: page 14 describes 0 as reverse and 255 as forward, while page 15 describes 0 as forward, 255 as reverse, and 127 as stop.

Do **not** silently assume a direction mapping from these conflicting passages. Before writing the final actuator-control sketch, use the manufacturer's DIP wizard/current library documentation and then perform a low-risk, verified direction test with the actuator mechanically clear and a safe power setup. Use a confirmed stop command appropriate to the selected protocol.

## 6. DIP switches

The user has observed all six switches currently in the **ON** direction. Prior notes indicate this may correspond to analog independent mode, not the desired serial mode, but do not rely on memory or a generic switch diagram.

The user has the manufacturer's **DIP switch wizard** and prefers to use it to configure the board. That is a good next step.

Before changing switches:
1. Decide the intended input mode (currently simplified serial is the leading candidate).
2. Complete the wizard questions for this exact SyRen 10 use.
3. Record the resulting switch positions, including which side the wizard calls ON/up/down.
4. Compare with the manual's mode table.
5. With power off, set the switches and photograph/record the final configuration before wiring or energizing.

Do not guess the configuration from a generic DIP-switch illustration. Switch 3 is the lithium cutoff option: the manual says it should be **up/off when powered by a power supply**, and lithium cutoff should only be enabled for an appropriate lithium battery setup.

## 7. Arduino library

Dimension Engineering provides Arduino libraries and examples for SyRen/Sabertooth serial control, plus separate examples for R/C mode.

- [Arduino library page](https://www.dimensionengineering.com/info/arduino)
- The page lists a SyRen/Sabertooth Serial library and examples.
- It describes simplified serial as beginner-friendly and fast, but without checksumming.
- Packetized serial is more robust (checksummed packets) and supports multiple drivers, but has additional protocol/setup steps.

For one SyRen and this small POC, **start with the simplest documented serial option that the wizard/manual confirms**. Do not add a library or packet protocol just because it exists. We can use the library if its documented protocol matches the chosen DIP configuration; otherwise, a small direct serial implementation may be sufficient.

One practical Arduino detail: the Uno's hardware serial pins are shared with USB serial, so using the hardware UART for the SyRen can complicate Serial Monitor debugging. If using a software UART or a different serial arrangement, verify the library's timing and pin requirements before settling the sketch. Do not connect an output pin to S1 until the selected protocol and wiring are confirmed.

## 8. 5 V output and receiver power

The User's Guide (page 5) specifies that the SyRen 5 V terminal can provide:
- **100 mA** when the source battery is **12.6 V or less**.
- **10 mA** when the source battery is **above 12.6 V**.

It recommends a separate regulator (examples given are ParkBEC or SW05) if more 5 V current is required. The 0V terminal is signal ground and is internally connected to B−.

The earlier conservative 10 mA limit in our notes applies when the SyRen input source is above 12.6 V. If we power the SyRen at 12 V for this actuator POC, the manual's stated limit is 100 mA, but we still must confirm the actual input voltage and receiver current before deciding whether to power the ER8 from this output. Do not connect multiple 5 V sources together. The Arduino is currently planned to be powered by USB; decide how to power the ER8 without accidentally paralleling its supply with another 5 V rail.

## 9. PA-14P actuator integration

The PA-14P-6-35 is a **12 V DC** actuator with a 6-inch stroke and built-in potentiometer feedback. The SyRen drives only the actuator's motor conductors. The Arduino must separately read the potentiometer feedback and implement position control.

POC 7 intended behavior:
- T8L left joystick **horizontal X axis** maps left → 0% retracted, center → 50%, right → 100% extended.
- Arduino reads the receiver's PWM on the ER8 channel identified during calibration.
- Arduino reads actuator position from an analog input, after confirming the factory connector pinout and feedback wiring.
- Arduino compares desired and measured position, commands extend/retract through the SyRen, and stops within a calibrated position tolerance.
- Invalid receiver signal or invalid feedback must stop motor commands.
- Do not command blind motion at startup if position feedback is invalid.
- Calibrate actual potentiometer readings at the usable endpoints; do not assume 0–5 V or ADC 0–1023 endpoints.

The unused actuator's factory connector is recorded as a 6-position Molex-style connector with five populated conductors, previously recorded as red, black, white, green, and yellow. The exact pin/function mapping is **not yet verified**. Preserve the factory cable and do not infer motor or potentiometer functions from wire colors alone.

## 10. Recommended next steps

Keep the work bite-sized:

1. Use the manufacturer's DIP wizard to identify the desired serial configuration; save its switch settings.
2. Confirm the appropriate power arrangement for regenerative operation before connecting actuator motor power. NANKADF alone is not yet approved by the source material.
3. Verify actuator motor and potentiometer pins from the exact PA-14P documentation/measurements.
4. Verify which ER8 channel follows the T8L left joystick X axis.
5. Implement a minimal SyRen command test that starts at stop, then briefly tests each direction with a safe supply setup and the actuator mechanically clear.
6. Add closed-loop potentiometer position control only after motor direction, feedback range, and stop behavior are verified.

## 11. Source-specific notes and limitations

- The uploaded Quick Start Guide is dated **April 2005**; the uploaded User's Guide is dated **July 2007**.
- The User's Guide specifies the supply-plus-battery recommendation and explains regeneration and the ATX approach. The ATX guide is written for an ATX computer supply and its recommended 12 V SLA buffer; it does not explicitly validate the NANKADF GC3010H.
- The supplied documents disagree on the simplified serial byte mapping/stop value. Resolve this using the exact board configuration and current manufacturer resources before using the command values in a powered test.
- No exact PA-14P connector pinout is established in this SyRen document; actuator-specific wiring must be verified separately.
