# Documentation

This directory contains six focused documents, each serving a distinct audience, plus the project roadmap and design rationale.

## For Users

**[user_manual.md](user_manual.md)** targets end users who have flashed the firmware and want to make music. It covers physical setup (mounting the Patch Init, power connections), a getting-started walkthrough (patch a gate and V/Oct source, hear your first note), detailed descriptions of each control mode with sonic examples, tips for patching with external sequencers (Keystep Pro, Metropolix, Pamela's NEW Workout), a MIDI setup guide for USB MIDI from a DAW, and a troubleshooting FAQ. Written in non-technical language.

**[control_reference.md](control_reference.md)** is a printable quick-reference card. Every knob, CV input, gate, switch, and MIDI CC is documented in dense table format, suitable for printing and keeping near the module. Includes voltage ranges, default values, and interaction notes (e.g., "CV 2 adds to CTRL 1 cutoff value; combined range is clamped").

**[calibration.md](calibration.md)** describes the V/Oct calibration procedure for accurate pitch tracking, including expected ADC values and the fine-tuning control.

## For Developers

**[build_guide.md](build_guide.md)** provides step-by-step instructions: installing the ARM GCC toolchain on macOS/Linux/Windows, cloning the repository with submodules, building libDaisy and DaisySP from source, compiling the firmware, entering DFU mode, and flashing. Includes common error messages and solutions, VS Code configuration walkthrough, and instructions for debug builds with ST-Link.

**[architecture.md](architecture.md)** is the technical deep-dive for contributors and DSP enthusiasts. It describes the complete signal flow with block diagrams, the audio callback structure, the control smoothing and parameter scaling implementation, the mode-switching state machine, MIDI handling architecture, memory budget breakdown, and CPU budget analysis.

**[open303_modifications.md](open303_modifications.md)** documents every change made to the original Open303 codebase, organized by component. Each modification includes the rationale (performance, stability, or feature), the specific code changes, and any measured impact on audio quality or CPU usage. This document ensures the modifications are transparent and reviewable by the Open303 community, and makes it straightforward to merge upstream improvements.

**[optimization_notes.md](optimization_notes.md)** records the ARM Cortex-M7 optimization journey: benchmarks of the original `double` code versus `float` conversion, the elliptic filter instability investigation and Butterworth replacement, `tanh` approximation accuracy measurements, lookup table sizing decisions, block processing cache behavior analysis, and compiler flag choices. Serves as a reference for anyone optimizing audio DSP on STM32H7 targets.

## Project-Level

**[ROADMAP.md](ROADMAP.md)** contains the full 6-phase development plan with week-by-week objectives, detailed descriptions, and deliverable checklists for each phase.

**[DESIGN_RATIONALE.md](DESIGN_RATIONALE.md)** explains the core design decisions: why the Open303 engine benefits from C++17 modernization, why the Patch Init is an ideal hardware platform for a patchable 303 voice, and what makes this project unique in the Eurorack ecosystem.
