# ASV Arduino Sketches

This directory contains Arduino sketches for the ASV project's incremental proof-of-concept work.

## Organization

Arduino IDE requires each `.ino` sketch to be inside a folder with the same name as the sketch. Use one subdirectory per sketch/POC.

```text
sketches/
├── README.md
└── 01_basics/
    ├── README.md
    ├── 01_blink/
    │   └── 01_blink.ino
    ├── 02_serial_test/
    │   └── 02_serial_test.ino
    └── 03_servo_sweep/
        └── 03_servo_sweep.ino
```

The sketches should follow the ASV project's incremental POC approach: prove one thing at a time before adding system complexity.
