# Corrode User Manual

## Getting Started

### What You Need

- A Daisy Patch Init module mounted in your Eurorack case
- A USB Micro-B cable
- An external sequencer with CV/Gate outputs (or a DAW with USB MIDI)

### First Sound

1. Connect **Gate Out** from your sequencer to **Gate In 1** on the Patch Init
2. Connect **CV/Oct** from your sequencer to **CV 1** on the Patch Init
3. Connect **Audio Out L** to your mixer or audio interface
4. Power on your case
5. Set the toggle switch to **Up** (sawtooth) for the classic acid sound
6. Turn CTRL 1 (Cutoff) to about 2 o'clock
7. Turn CTRL 2 (Resonance) to about noon
8. Start your sequencer — you should hear the 303 voice

### Understanding the Dual-Mode System

The Patch Init has 4 knobs, but Corrode controls 8 parameters. Press the **button** to switch between two modes:

**Mode A (LED steady)** controls the core tone: Cutoff, Resonance, Envelope Mod, and Decay. These are the parameters you'll tweak most during a performance.

**Mode B (LED blinking)** controls the character: Accent Level, Slide Time, Drive, and Tuning. Set these to taste, then switch back to Mode A for hands-on filter work.

When you switch modes, the knobs use "pickup" behavior: a parameter only changes once the knob crosses its current value. This prevents sudden jumps when switching modes.

## Adding Accent and Slide

The magic of the 303 comes from accent and slide patterns. Connect a second gate source to **Gate In 2**:

- **Accent**: When Gate In 2 is HIGH at the same moment Gate In 1 triggers a new note, that note is accented (louder, brighter, punchier)
- **Slide**: When Gate In 2 is HIGH while a note is already playing (legato/tied notes), the pitch slides smoothly to the next note

Many sequencers (Pamela's NEW Workout, Metropolix, Varigate) can provide independent gate patterns on separate outputs, making it easy to create classic acid accent/slide patterns.

## CV Modulation

Patch external modulation sources into the CV inputs for expressive control:

- **CV 2** modulates the filter cutoff (add an LFO for wobble, or a random source for variation)
- **CV 3** modulates envelope depth (vary how much the envelope sweeps the filter per note)
- **CV 4** provides continuous accent amount (alternative to the gate-based accent on Gate In 2)

CV inputs are additive with their corresponding knob values.

## USB MIDI

Connect a USB cable from your computer to the Daisy Patch Submodule. Corrode responds to MIDI notes (pitch + gate), velocity (accent), and CC messages (all parameters). See the Control Reference for CC assignments.

When MIDI is active, it takes priority over CV 1 for pitch control. MIDI activity times out after ~2 seconds of silence, returning control to CV inputs.

## External Audio (Filter Mode)

Patch any audio signal into **Audio In L**. When an input signal is detected, it replaces the internal oscillator and passes through the 303's diode ladder filter, envelope, and VCA. This lets you process drums, other synths, or samples through the iconic 303 filter.

## Tips for Classic Acid Sounds

- Start with sawtooth, cutoff at noon, resonance at 2 o'clock, env mod at 3 o'clock
- Short decay (9 o'clock) for percussive stabs
- Long decay (3 o'clock) for sweeping, evolving lines
- Accent creates the screaming filter peaks: set accent level high and send accent triggers on every 2nd or 4th step
- Consecutive accented notes build up the accent sweep capacitor, making the filter scream progressively louder — this is the authentic 303 behavior
- Slide between notes for the classic "rubbery" 303 glide
- Add drive (Mode B, CTRL 3) for distorted, aggressive acid

## Firmware Update

1. Download the latest `corrode.bin` from GitHub Releases
2. Connect USB cable to the Daisy Patch Submodule
3. Hold the **BOOT** button on the Daisy Submodule
4. Press and release the **RESET** button
5. Release the **BOOT** button (the module is now in DFU mode)
6. Flash using the Daisy Web Programmer at https://electro-smith.github.io/Programmer/ or via command line: `make program-dfu`
