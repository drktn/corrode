# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Phase 1 — Foundation
- [ ] Repository structure and build system
- [ ] Open303 engine compilation for ARM
- [ ] Basic audio output (hardcoded note)
- [ ] Gate In 1 + CV 1 (pitch) control
- [ ] Mode A knobs: Cutoff, Resonance, Env Mod, Decay
- [ ] Toggle switch: saw/square selection

### Phase 2 — Full Controls
- [ ] Dual-mode button system with LED indicator
- [ ] Knob pickup behavior
- [ ] Mode B knobs: Accent, Slide, Drive, Tuning
- [ ] Gate In 2: accent/slide trigger logic
- [ ] CV inputs: cutoff mod, env mod, accent
- [ ] Parameter smoothing

### Phase 3 — DSP Optimization
- [ ] double → float conversion
- [ ] Anti-aliasing filter replacement (Butterworth)
- [ ] Fast tanh() approximation
- [ ] Fast exp() approximation
- [ ] Pitch lookup table
- [ ] CPU profiling and benchmarks

### Phase 4 — MIDI & Extended Features
- [ ] USB MIDI note/CC control
- [ ] External audio input routing
- [ ] CV output envelope follower
- [ ] Gate outputs (accent/note echo)
- [ ] Preset storage (QSPI flash)

### Phase 5 — Polish & Release
- [ ] Drive/overdrive stage
- [ ] Documentation
- [ ] Custom faceplate design
- [ ] Unit tests
- [ ] v1.0.0 release with compiled binary
