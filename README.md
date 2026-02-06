# Corrode

**A TB-303 acid synthesizer voice for the Electro-Smith Daisy Patch Init**

Corrode brings the iconic acid sound of the Roland TB-303 to a 10HP Eurorack module. Built on Robin Schmidt's [Open303](https://github.com/RobinSchmidt/Open303) DSP engine — widely regarded as the finest open-source 303 emulation — and optimized for the ARM Cortex-M7 processor on the [Daisy Patch Init](https://electro-smith.com/products/patch-init). No built-in sequencer: drive it from your favorite external sequencer via CV/Gate or USB MIDI.

Inspired by [AcidusVersio](https://github.com/abluenautilus/AcidusVersio) by Blue Nautilus, which proved the Open303 engine runs beautifully on the Daisy platform. Corrode adapts the concept for the Patch Init's different control layout and adds modern C++ improvements, performance optimizations, and expanded CV integration for a deeply patchable Eurorack acid voice.

## Features

- **Authentic TB-303 sound** — Diode ladder filter with feedback highpass, exponential envelopes, accent sweep circuit with capacitor accumulation, exponential slide/glide
- **Sawtooth and square waveforms** — Hardware toggle switch for instant waveform selection
- **Dual-mode knob system** — 4 knobs × 2 pages = 8 parameters, switched via button press
- **Full CV/Gate integration** — V/Oct pitch, filter cutoff CV, dedicated accent gate input, envelope mod CV
- **USB MIDI** — Complete note, velocity, and CC control over all parameters
- **External audio input** — Run external signals through the 303 filter
- **Envelope follower output** — CV output tracks the filter envelope for patching to other modules
- **Optimized for ARM** — Modern C++17, single-precision DSP, fast math approximations, stable anti-aliasing

## Hardware Requirements

- **[Daisy Patch Init](https://electro-smith.com/products/patch-init)** (10HP Eurorack module with Daisy Patch Submodule)
- Eurorack case with ±12V power supply (90mA on +12V, 5mA on -12V)
- External sequencer: hardware (Arturia Keystep, Torso T-1, modular sequencer) or DAW via USB MIDI
- USB Micro-B cable (for flashing firmware and USB MIDI)

### Processor Specs (Daisy Patch Submodule)

| Spec | Value |
|------|-------|
| CPU | ARM Cortex-M7 @ 480 MHz |
| FPU | Hardware double-precision (FPv5-D16) |
| SDRAM | 64 MB |
| Flash | 8 MB QSPI |
| Audio Codec | PCM3060, 24-bit, 48 kHz |
| Latency | ~1 ms (48-sample blocks) |

## Control Mapping

### Physical Controls Overview

| Control | Type | Count |
|---------|------|-------|
| Knobs (CTRL 1–4) | Potentiometer (0–3.3 V) | 4 |
| CV Inputs (CV 1–4) | Bipolar (±5 V) | 4 |
| Gate Inputs | Digital, active high | 2 |
| Gate Outputs | Digital GPIO | 2 |
| CV Output | 12-bit DAC (0–5 V) | 1 |
| Toggle Switch | 2-position | 1 |
| Button | Momentary | 1 |
| LED | User-controllable | 1 |
| Audio In / Out | Stereo (L/R) | 1 each |

### Knob Assignments (Dual-Mode System)

Press the **button** to switch between Mode A and Mode B. The LED indicates the current mode: **steady = Mode A**, **blinking = Mode B**.

#### Mode A — Tone (Primary Sound Shaping)

| Knob | Parameter | Range | Notes |
|------|-----------|-------|-------|
| CTRL 1 | **Cutoff Frequency** | 50 Hz – 12 kHz | The 303's signature control |
| CTRL 2 | **Resonance** | 0 – 100% | Diode ladder squelch; does not self-oscillate (authentic behavior) |
| CTRL 3 | **Envelope Mod** | 0 – 100% | Depth of filter envelope sweep |
| CTRL 4 | **Decay** | 30 ms – 3 s | Filter envelope decay time |

#### Mode B — Character (Expression & Dynamics)

| Knob | Parameter | Range | Notes |
|------|-----------|-------|-------|
| CTRL 1 | **Accent Level** | 0 – 100% | Accent strength (filter + VCA boost) |
| CTRL 2 | **Slide Time** | 10 ms – 200 ms | Exponential glide between slid notes |
| CTRL 3 | **Drive / Overdrive** | Clean – saturated | Soft clipping / distortion amount |
| CTRL 4 | **Tuning** | ±1 semitone | Fine-tune oscillator pitch |

### Toggle Switch

| Position | Function |
|----------|----------|
| **Up** | Sawtooth waveform |
| **Down** | Square waveform |

### CV Inputs

| Jack | Function | Voltage | Notes |
|------|----------|---------|-------|
| **CV 1** | **V/Oct Pitch** | ±5 V (1V/Oct) | Primary pitch input from sequencer |
| **CV 2** | **Cutoff CV** | ±5 V | Additive modulation of filter cutoff |
| **CV 3** | **Envelope Mod CV** | ±5 V | Modulate envelope depth dynamically |
| **CV 4** | **Accent CV** | ±5 V | Continuous accent amount; alternative to Gate 2 accent trigger |

### Gate Inputs

| Jack | Function | Notes |
|------|----------|-------|
| **Gate In 1** | **Note Gate** | Rising edge triggers note-on; falling edge triggers note-off |
| **Gate In 2** | **Accent / Slide Gate** | High + Gate 1 rising edge = accented note. High while Gate 1 already high (legato) = slide to next note |

### Outputs

| Jack | Function | Notes |
|------|----------|-------|
| **Audio Out L** | Mono synth output | Main 303 voice |
| **Audio Out R** | Mono synth output (duplicate) | Identical to L for easy stereo patching |
| **CV Out** | Filter envelope follower | 0–5 V envelope output |
| **Gate Out 1** | Accent echo | Mirrors accent state |
| **Gate Out 2** | Note echo | Mirrors gate state |

### Audio Input (Filter Mode)

| Jack | Function |
|------|----------|
| **Audio In L** | External audio routed through the 303 filter, envelope, and VCA. When patched, replaces the internal oscillator. |

### USB MIDI

All parameters are controllable via MIDI CC messages over USB. MIDI note-on/off messages trigger the synth voice with velocity mapped to accent amount.

| MIDI CC | Parameter |
|---------|-----------|
| CC 74 | Cutoff Frequency |
| CC 71 | Resonance |
| CC 70 | Envelope Mod |
| CC 75 | Decay |
| CC 76 | Accent Level |
| CC 5 | Slide Time (Portamento) |
| CC 77 | Drive |
| CC 94 | Tuning |

MIDI notes trigger the oscillator pitch directly (bypassing CV 1 when MIDI is active). Velocity > 100 triggers accent behavior.

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                   CORRODE — Signal Flow                     │
│                                                             │
│  CV 1 (V/Oct) ──→ ┌──────────┐                             │
│  MIDI Note ──────→ │ Pitch    │──→ ┌─────────────────────┐ │
│  Gate In 1 ──────→ │ + Gate   │    │ Open303 DSP Engine  │ │
│  Gate In 2 ──────→ │ + Accent │    │                     │ │
│                    └──────────┘    │  Oscillator (BLEP)  │ │
│                                    │    ↓ Saw / Square   │ │
│  Audio In ──────────────────────→  │  [OR External Audio]│ │
│                                    │    ↓                │ │
│  CTRL 1/CV 2 ──→ Cutoff ────────→ │  Diode Ladder LPF  │ │
│  CTRL 2 ──────→ Resonance ──────→ │  (4-pole + FB HPF)  │ │
│  CTRL 3/CV 3 ──→ Env Mod ───────→ │    ↑ Filter Env     │ │
│  CTRL 4 ──────→ Decay ──────────→ │    ↓                │ │
│  Accent ──────→ Accent Circuit ──→ │  VCA Envelope       │ │
│  Slide ───────→ Glide Circuit ───→ │    ↓                │ │
│  Drive ───────→ Soft Clip ───────→ │  Output Stage       │ │
│                                    └─────────┬───────────┘ │
│                                              ↓             │
│                                    ┌─────────────────────┐ │
│                                    │ Audio Out L / R     │ │
│                                    │ CV Out (Env Follow) │ │
│                                    │ Gate Out 1/2        │ │
│                                    └─────────────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

### DSP Engine Components

The Open303 engine provides these core DSP blocks, each modeling a specific circuit in the original TB-303:

- **BlepOscillator** — Band-limited sawtooth and square waves via polyBLEP anti-aliasing. The 303's square wave has a distinctive non-50% duty cycle.
- **TeeBeeFilter** — 4 cascaded one-pole lowpass stages (24 dB/oct) with `tanh` saturation in the feedback path and a highpass filter (~120 Hz) in the resonance loop. The filter does not self-oscillate at maximum resonance, matching authentic 303 behavior.
- **AnalogEnvelope** — RC-circuit modeled exponential attack/decay for both filter (MEG) and amplitude (VEG). Accent forces the filter decay to minimum.
- **Accent Circuit** — Simultaneously boosts filter cutoff, increases VCA level, and shortens filter decay. The accent sweep circuit uses a capacitor that accumulates charge across consecutive accented notes.
- **Slide Circuit** — Exponential portamento via RC lowpass on the pitch CV. Slide also extends the gate (legato), preventing envelope retrigger.

### Open303 Engine Modifications

See [docs/open303_modifications.md](docs/open303_modifications.md) for the full changelog. Key changes:

1. **`float` conversion** — All DSP from `double` to `float` for ~2× throughput on Cortex-M7
2. **Anti-aliasing filter replacement** — Elliptic quarter-band filter (numerically unstable in SP) replaced with 4th-order Butterworth
3. **Fast `tanh()` approximation** — Padé rational approximation: `x * (27 + x²) / (27 + 9x²)`
4. **Fast `exp()` approximation** — Optimized exponential for envelope coefficient calculations
5. **Lookup tables** — Pre-computed pitch-to-frequency table (2048 entries, linear interpolation)
6. **Modern C++17** — `constexpr`, `std::array`, `enum class`, RAII, no raw pointers
7. **Removed unused components** — FourierTransformer, AcidSequencer, VST wrapper stripped
8. **Block processing** — Inner loops restructured for Cortex-M7 cache behavior
9. **External audio input** — Audio input routing through the filter/VCA chain
10. **Drive stage** — Soft-clipping distortion inspired by Devil Fish modifications

## Build Instructions

### Prerequisites

1. Install the [Daisy Toolchain](https://daisy.audio/tutorials/cpp-dev-env/) (ARM GCC, Make, OpenOCD)
2. Clone with submodules:
   ```bash
   git clone --recursive https://github.com/yourusername/corrode.git
   cd corrode
   ```
3. Build libDaisy and DaisySP:
   ```bash
   cd lib/libDaisy && make && cd ../..
   cd lib/DaisySP && make && cd ../..
   ```

### Build & Flash

```bash
make                    # Build firmware
make program-dfu        # Flash via USB (hold BOOT button, then RESET)
```

To enter DFU mode: hold the BOOT button on the Daisy Patch Submodule, press and release RESET, then release BOOT.

### Development

```bash
make clean              # Clean build artifacts
make DEBUG=1            # Build with debug symbols
```

## License

GPL v3 — See [LICENSE](LICENSE) for details.

## Acknowledgments

- **[Robin Schmidt](https://github.com/RobinSchmidt)** — Open303 DSP engine
- **[Blue Nautilus](https://github.com/abluenautilus)** — AcidusVersio, proving the Open303-on-Daisy concept
- **[Electro-Smith](https://electro-smith.com)** — Daisy platform, libDaisy, and DaisySP
- **[midilab](https://github.com/midilab/jc303)** — JC-303, modern Open303 maintenance
- **Tim Stinchcombe** — TB-303 diode ladder filter analysis
- **Robin Whittle** — Devil Fish modifications and accent circuit documentation
