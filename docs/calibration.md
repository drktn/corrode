# V/Oct Calibration

## Overview

The Patch Init's CV inputs use a 16-bit ADC with a ±5V range. For accurate pitch tracking, the V/Oct input (CV 1) needs calibration to map the ADC reading to the correct frequency.

## Calibration Procedure

*TODO: Implement calibration mode in firmware*

1. Enter calibration mode: hold button for 5 seconds on startup
2. Send exactly 0V to CV 1 → press button (stores low reference)
3. Send exactly 2V to CV 1 → press button (stores high reference)
4. Calibration values are saved to flash

## Expected ADC Values

| Voltage | Expected Note | ADC Value (approx) |
|---------|---------------|---------------------|
| -2V     | C1 (36)       | ~9830               |
| -1V     | C2 (48)       | ~16384              |
| 0V      | C3 (60)       | ~32768              |
| +1V     | C4 (72)       | ~49152              |
| +2V     | C5 (84)       | ~55706              |

## Tuning

Fine-tuning is available via CTRL 4 in Mode B (±1 semitone) for per-session adjustment.
