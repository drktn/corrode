# Architecture

## Overview

Corrode runs on the STM32H750 ARM Cortex-M7 at 480 MHz via the Electro-Smith Daisy Patch Submodule. The firmware is structured around an interrupt-driven audio callback that processes 48 samples per block at 48 kHz, giving approximately 1 ms of audio latency.

## Signal Flow

See the ASCII diagram in the README for the complete signal flow. The processing chain per sample is:

1. **Pitch calculation**: CV 1 (V/Oct) or MIDI note → frequency, with optional slide (exponential portamento)
2. **Oscillator**: BlepOscillator generates sawtooth or square wave at the target frequency
3. **Drive** (optional): Soft-clipping waveshaper applied pre-filter
4. **Filter**: TeeBeeFilter processes the signal with modulated cutoff (base + envelope + accent + CV)
5. **VCA**: Output scaled by amplitude envelope, boosted during accent
6. **Output**: Mono signal sent to both Audio Out L and R

## Module Layers

### `src/main.cpp`
Entry point. Initializes hardware (DaisyPatchSM), creates Corrode engine, Controls, and MidiHandler instances. Starts the audio callback and runs the main loop for LED updates.

### `src/corrode.h/cpp`
Top-level synth voice. Wraps the Open303 DSP engine with parameter management and external audio routing. Exposes a clean interface for Controls and MidiHandler to drive.

### `src/controls.h/cpp`
Hardware abstraction for knobs, CV inputs, gates, toggle switch, and button. Implements the dual-mode system, knob pickup behavior, parameter smoothing, and accent/slide gate logic.

### `src/midi_handler.h/cpp`
USB MIDI processing. Parses Note On/Off (with velocity-to-accent mapping), CC messages (parameter control), and manages MIDI/CV priority.

### `dsp/*`
The modified Open303 engine. See `dsp/README.md` for the complete file mapping and modification summary.

## Audio Callback Structure

```
AudioCallback(in, out, size=48)
│
├── controls.Process()          // Read knobs, CVs, gates, button
│   ├── ProcessButton()         // Mode toggle with debounce
│   ├── ProcessKnobs()          // Read 4 knobs, apply to current mode params
│   ├── ProcessCV()             // Read 4 CV inputs, add to base params
│   ├── ProcessGates()          // Gate 1 (note), Gate 2 (accent/slide)
│   └── ProcessToggle()         // Waveform switch
│
├── midi.Process()              // Parse pending USB MIDI messages
│
├── for each sample:
│   └── synth.Process()         // One sample of 303 engine
│       ├── UpdatePitch()       // Slide processing
│       ├── filter_env.Process()
│       ├── amp_env.Process()
│       ├── oscillator.Process()
│       ├── drive (if active)
│       ├── filter.Process()
│       └── VCA scaling
│
├── WriteCvOut()                // Envelope follower to CV output
└── Write gate outputs          // Accent/note echo
```

## Memory Budget

| Component | Estimated RAM |
|-----------|---------------|
| Open303 engine | ~2 KB |
| Filter state | ~200 B |
| Oscillator state | ~100 B |
| Envelopes (×3) | ~300 B |
| Control smoothing | ~200 B |
| MIDI handler | ~500 B |
| Audio buffers (DMA) | ~1 KB |
| **Total** | **~4–5 KB** |

The STM32H750 has 1 MB internal SRAM and 64 MB external SDRAM, so memory is not a constraint for a mono 303 voice.

## CPU Budget

Target: audio callback completes within 20% of available CPU time.

At 48 kHz with 48-sample blocks, one block period = 1 ms = 480,000 cycles at 480 MHz. The 303 engine with 2× oversampling processes 96 samples per block. With optimized float DSP, the estimated budget is 50,000–100,000 cycles per block (10–20%).

CPU profiling uses the Cortex-M7 cycle counter (`DWT->CYCCNT`).
