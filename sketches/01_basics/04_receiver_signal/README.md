# 04 — Receiver Signal

Goal: begin the ER8 receiver → Arduino signal path without connecting consequential hardware.

This POC will:

1. Establish the physical ER8 → Arduino signal connection.
2. Read one PWM receiver channel.
3. Print the measured pulse width over Serial Monitor.
4. Move the assigned T8L control and observe the value change.

## Safety / setup

- Verify the exact ER8 model, power requirements, output pinout, and connector/header configuration before wiring.
- Use appropriate female-ended wiring for the ER8's male header pins.
- Use bench power for the receiver/Arduino prototype as appropriate.
- Do not connect motor/propulsion hardware during this POC.

## Sketch

`04_receiver_signal.ino` is the starting sketch for reading one PWM channel.
