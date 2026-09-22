# 01 — Arduino Basics

Small, independent sketches for learning the Arduino development environment and basic Arduino C++ concepts before and during the T8L/ER8 signal-chain integration.

## Sketches

Each Arduino `.ino` sketch lives inside a folder with the same name, as required by Arduino IDE.

```text
01_basics/
├── README.md
├── 01_blink/
│   └── 01_blink.ino
├── 02_serial_test/
│   └── 02_serial_test.ino
├── 03_servo_sweep/
│   └── 03_servo_sweep.ino
├── 04_receiver_signal/
│   └── 04_receiver_signal.ino
└── 05_t8l_er8_servo/
    └── 05_t8l_er8_servo.ino
```

These are intentionally incremental. Each sketch should prove one concept before the next layer is added.

### Current milestones

- **POC 4:** ER8 CH1 → Arduino D2 → Serial signal visibility. Complete.
- **POC 5:** T8L + ER8 + Arduino + D646WP steering integration, with throttle observed over Serial. Ready to test.
