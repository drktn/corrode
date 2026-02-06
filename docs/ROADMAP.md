# Development Roadmap

## Phase 1 — Foundation (weeks 1–3)

The goal is a working 303 voice with basic controls, no optimizations yet. Start by setting up the repository structure, build system, and Git submodules (libDaisy, DaisySP). Copy the Open303 DSP source files into `dsp/`, stripping the VST wrapper, sequencer, and FourierTransformer. Rename files from `rosic_` convention to clean snake_case. Get the unmodified Open303 engine compiling for ARM with the Daisy toolchain: this means resolving any compiler compatibility issues (the codebase was originally VS2008-era) and ensuring it builds under `arm-none-eabi-g++` with `-std=gnu++14`. Write a minimal `main.cpp` that initializes the Daisy Patch SM, creates an Open303 instance, hardcodes a note-on in the audio callback, and outputs audio.

**Milestone: hear a 303 sound from the Patch Init.**

Wire up the basic control mapping: CTRL 1 → cutoff, CTRL 2 → resonance, Gate In 1 → note trigger, CV 1 → V/Oct pitch. Implement pitch CV-to-MIDI-note conversion with a quantized lookup table. Test with an external sequencer sending gate and pitch. By the end of Phase 1, all four Mode A knobs (cutoff, resonance, env mod, decay) should be functional, the toggle switch should select saw/square, and notes should trigger from Gate In 1 with pitch from CV 1.

### Deliverables
- [ ] Repository structure and build system
- [ ] Open303 engine compilation for ARM (unmodified, double precision)
- [ ] Basic audio output with hardcoded note
- [ ] Gate In 1 + CV 1 (pitch) control working
- [ ] Mode A knobs: Cutoff, Resonance, Env Mod, Decay
- [ ] Toggle switch: saw/square selection

---

## Phase 2 — Full Control Mapping and Mode System (weeks 4–5)

Implement the dual-mode button system: pressing the button toggles between Mode A and Mode B, with the LED indicating state. Implement knob "pickup" behavior: when switching modes, a parameter only updates once the knob position crosses the stored value, preventing jumps. Wire up all Mode B parameters: accent level, slide time, drive, tuning.

Implement Gate In 2 as the accent/slide trigger. The logic: if Gate In 2 is high when Gate In 1 has a rising edge, the note is accented. If Gate In 2 is high while Gate In 1 is already high (legato), slide is activated. This dual interpretation gives sequencers with two gate outputs full control over both accent and slide patterns.

Add CV input processing for CV 2 (cutoff modulation), CV 3 (envelope mod), and CV 4 (accent). CVs are additive with their corresponding knob values, clamped to valid ranges. Implement parameter smoothing on all controls using one-pole lowpass filters to prevent zipper noise.

### Deliverables
- [ ] Dual-mode button system with LED indicator
- [ ] Knob pickup behavior across mode switches
- [ ] Mode B knobs: Accent, Slide Time, Drive, Tuning
- [ ] Gate In 2: accent/slide trigger logic
- [ ] CV inputs: cutoff mod (CV 2), env mod (CV 3), accent (CV 4)
- [ ] Parameter smoothing on all controls

---

## Phase 3 — DSP Optimization for ARM (weeks 6–8)

This is the critical engineering phase. Convert the entire `dsp/` codebase from `double` to `float` using a `SampleType` typedef. Replace the elliptic quarter-band anti-aliasing filter, which becomes numerically unstable within milliseconds at single precision, with a 4th-order Butterworth IIR designed for the same cutoff. Alternatively, evaluate removing 2× oversampling entirely and relying solely on the polyBLEP oscillator's anti-aliasing, which may be sufficient for the 48 kHz output rate.

Replace `tanh()` calls in the filter feedback with the Padé rational approximation `x * (27.0f + x*x) / (27.0f + 9.0f*x*x)`, clamped for |x| > 3. Measure THD and frequency response against the original `tanh()` to quantify the quality tradeoff. Replace `exp()` in envelope coefficient calculations with a fast approximation. Generate compile-time lookup tables for pitch-to-frequency conversion (2048 entries, linear interpolation).

Profile the audio callback using cycle counting (`DWT->CYCCNT` on Cortex-M7). Target: complete audio callback (48 samples) in under 20% of available CPU cycles at 48 kHz. Document all optimization decisions and measurements in `optimization_notes.md`.

### Deliverables
- [ ] Full double → float conversion with SampleType typedef
- [ ] Anti-aliasing filter replacement (Butterworth or removal evaluation)
- [ ] Fast tanh() approximation with accuracy measurements
- [ ] Fast exp() approximation for envelope coefficients
- [ ] Pitch-to-frequency lookup table
- [ ] CPU profiling with DWT cycle counter
- [ ] All benchmarks documented in `docs/optimization_notes.md`

---

## Phase 4 — USB MIDI and Extended Features (weeks 9–10)

Implement USB MIDI using libDaisy's `MidiUsbHandler`. Parse Note On/Off messages for pitch and gate control (MIDI takes priority over CV when active). Map velocity to accent amount (velocity > 100 triggers accent behavior). Implement CC mapping per the table in the README. Add running status and MIDI channel filtering.

Implement the external audio input feature: when Audio In L is patched, the oscillator output is replaced with the external signal, which then passes through the filter, VCA, and output stage. Detect input presence by monitoring signal level on Audio In L.

Add the CV output envelope follower: sample the filter envelope value each audio block and write it to the DAC via `WriteCvOut()`. Add gate outputs mirroring the accent and note states for patching to other modules.

Implement preset storage using libDaisy's `PersistentStorage` class on the QSPI flash. Long-press the button (>1 second) to save current knob positions as default startup values.

### Deliverables
- [ ] USB MIDI note/velocity/CC control
- [ ] MIDI channel filtering
- [ ] External audio input routing through filter/VCA
- [ ] CV output: filter envelope follower
- [ ] Gate outputs: accent echo, note echo
- [ ] Preset storage on QSPI flash (long-press to save)

---

## Phase 5 — Drive Stage, Polish, and Release (weeks 11–13)

Implement the overdrive/drive stage. Start with a simple waveshaper (`tanh`-based soft clipping) applied before the filter input, with the drive amount controlled by CTRL 3 in Mode B. Evaluate adding a more sophisticated model inspired by Devil Fish modifications: driving the filter input at up to 10× nominal level for internal filter saturation.

Add the "range mode" system from AcidusVersio as an optional feature: a long-press + toggle combination cycles between three parameter range presets (Conservative, Standard, Extended) that constrain knob ranges to musically useful zones.

Write all documentation. Create the custom faceplate SVG with control labels, module name, and jack designations. Build the Python test tools for frequency response plotting and envelope visualization. Write unit tests for the fast math approximations (max error bounds) and pitch tracking accuracy. Final testing with multiple sequencers (Keystep, MIDI from Ableton, Pam's gates). Tag v1.0.0 release with compiled firmware binary.

### Deliverables
- [ ] Drive/overdrive stage (pre-filter soft clipping)
- [ ] Optional range mode system
- [ ] Complete documentation (all 6 docs in `docs/`)
- [ ] Custom faceplate SVG
- [ ] Unit tests for fast math and pitch tracking
- [ ] Testing with multiple sequencer configurations
- [ ] v1.0.0 release with compiled `.bin` firmware

---

## Phase 6 — Community and Future (ongoing)

Post-release improvements based on community feedback. Potential additions:

- **Polyphonic mode**: The STM32H750 likely has headroom for 2–4 voices. Would require significant architectural changes to the engine but the CPU budget should allow it.
- **Alternative filter models**: The TPT (topology-preserving transform) diode ladder variant has been discussed in the Open303 community as a potential improvement over the original discretized model.
- **Wavetable oscillator mode**: Use the 64 MB SDRAM for extended wavetable storage, allowing the 303 filter/envelope/accent engine to process arbitrary waveforms.
- **Built-in pattern randomizer**: Button press generates random accent/slide patterns, driven from an internal clock or synced to an external clock input.
- **Improved V/Oct calibration**: Auto-calibration routine that measures known voltages and compensates for ADC nonlinearity.
- **Web-based parameter editor**: Use the USB connection for a browser-based control panel via WebUSB, allowing precise parameter editing and preset management from a computer.
