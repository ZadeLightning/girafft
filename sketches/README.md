# ASV Arduino Sketches

This directory contains Arduino sketches for the ASV project's incremental proof-of-concept work.

## Organization

Use one subdirectory per sketch/POC when a sketch becomes substantial. Keep early experiments small and self-contained.

Example:

```text
sketches/
├── README.md
├── 01_blink/
│   └── 01_blink.ino
├── 02_serial_test/
│   └── 02_serial_test.ino
└── ...
```

The sketches should follow the ASV project's incremental POC approach: prove one thing at a time before adding system complexity.
