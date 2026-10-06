# POC 6 — T8L + ER8 + Arduino + U10 ESC

## Goal

Prove the complete manual throttle signal chain:

**T8L throttle → ER8 CH3 → Arduino → ESC → U10**

The U10 is physically mounted to a temporary wooden fixture so it can be submerged in water during powered testing.

## Arduino connections

| Connection | Arduino |
|---|---|
| ER8 CH3 signal | D3 |
| ESC signal | D9 |
| Common ground | GND |

The ESC and receiver power arrangement should be verified against the actual ESC wiring before connecting the U10.

## ESC command

The purchased APISQUEEN waterproof bidirectional ESC is documented as:

- ~1000 µs = maximum reverse
- ~1500 µs = stop / neutral
- ~2000 µs = maximum forward

## Safety behavior

- Arduino commands **1500 µs neutral** immediately at startup.
- Throttle control remains locked until CH3 is detected near center.
- Loss of receiver signal commands **1500 µs neutral**.
- The sketch does not automatically command forward or reverse at startup.
- First powered testing should use low throttle and short-duration runs.
- The U10 must be submerged in water for powered operation.

## Current calibration

The sketch uses the existing T8L/ER8 calibration baseline:

- Minimum: ~984 µs
- Center: ~1493 µs
- Maximum: ~2002 µs

These values can be refined after observing the actual CH3 signal.

## Next step

When the ESC arrives:

1. Verify ESC wiring and common ground.
2. Upload this sketch.
3. Power the Arduino/receiver.
4. Confirm Serial output shows the throttle and a 1500 µs ESC command at center.
5. Connect the ESC signal.
6. Follow the ESC manufacturer's startup/arming behavior if it requires an additional procedure.
7. With the U10 submerged and securely restrained, test at low throttle first.
