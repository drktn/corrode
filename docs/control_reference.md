# Corrode — Control Reference Card

## Mode A: Tone (LED steady)

| Knob   | Parameter     | Range          |
|--------|---------------|----------------|
| CTRL 1 | Cutoff        | 50 Hz – 12 kHz |
| CTRL 2 | Resonance     | 0 – 100%       |
| CTRL 3 | Envelope Mod  | 0 – 100%       |
| CTRL 4 | Decay         | 30 ms – 3 s    |

## Mode B: Character (LED blinking)

| Knob   | Parameter     | Range          |
|--------|---------------|----------------|
| CTRL 1 | Accent Level  | 0 – 100%       |
| CTRL 2 | Slide Time    | 10 – 200 ms    |
| CTRL 3 | Drive         | Clean – Sat.   |
| CTRL 4 | Tuning        | ±1 semitone    |

## Toggle Switch

| Position | Waveform  |
|----------|-----------|
| Up       | Sawtooth  |
| Down     | Square    |

## CV Inputs (±5V bipolar)

| Jack | Function       | Combines with |
|------|----------------|---------------|
| CV 1 | V/Oct Pitch    | —             |
| CV 2 | Cutoff CV      | CTRL 1 (A)    |
| CV 3 | Env Mod CV     | CTRL 3 (A)    |
| CV 4 | Accent CV      | CTRL 1 (B)    |

## Gate Inputs

| Jack      | Function                                    |
|-----------|---------------------------------------------|
| Gate In 1 | Note gate (rising = on, falling = off)      |
| Gate In 2 | Accent (high + note-on) / Slide (high + legato) |

## Outputs

| Jack       | Function              |
|------------|-----------------------|
| Audio L    | Main output           |
| Audio R    | Main output (copy)    |
| CV Out     | Filter env follower   |
| Gate Out 1 | Accent echo           |
| Gate Out 2 | Note echo             |

## Audio Input

| Jack      | Function                                  |
|-----------|-------------------------------------------|
| Audio In L | External audio through 303 filter/VCA    |

## USB MIDI CC Map

| CC  | Parameter     |
|-----|---------------|
| 74  | Cutoff        |
| 71  | Resonance     |
| 70  | Envelope Mod  |
| 75  | Decay         |
| 76  | Accent Level  |
| 5   | Slide Time    |
| 77  | Drive         |
| 94  | Tuning        |

Velocity > 100 triggers accent. MIDI overrides CV 1 for pitch when active.
