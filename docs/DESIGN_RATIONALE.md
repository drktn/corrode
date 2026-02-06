# Design Rationale

This document explains the key design decisions behind Corrode: why the Open303 engine benefits from modernization, why the Patch Init is an ideal platform for a patchable 303 voice, and what makes this project unique in the existing ecosystem.

---

## The Open303 Engine Deserves Modern C++

The original Open303 codebase dates to 2009 and uses C++03-era patterns. Corrode modernizes it for C++17 while preserving the DSP algorithms exactly. The following sections detail the key changes and their rationale.

**Type safety improvements** replace `#define` constants with `constexpr float` variables, C-style enums with `enum class` (waveform types, filter modes, envelope states), and implicit conversions with explicit casts. C-style arrays become `std::array` throughout, providing bounds checking in debug builds with zero overhead in release.

**Memory and ownership** patterns shift from any raw `new`/`delete` to stack allocation. The engine's memory footprint is only 2–6 KB RAM for a single voice, trivially stack-allocable on the STM32H750's 1 MB internal SRAM. Smart pointers wrap any remaining dynamic allocations. All virtual functions are eliminated: the engine uses composition and CRTP instead, removing vtable overhead in the audio hot path.

**Performance-critical transformations** focus on the three hottest functions. The `TeeBeeFilter::getSample()` inner loop calls `tanh()` every sample at 2× oversampling (96,000 calls/second). The Padé approximation reduces this from ~80 cycles to ~8 cycles per call with less than 0.1% THD difference. The `AnalogEnvelope` coefficient recalculation uses `exp()` which is replaced with a fast bit-manipulation approximation. The elliptic anti-aliasing filter's high-order IIR stages require `double` precision for numerical stability; replacing it with a lower-order Butterworth designed for `float` eliminates the single biggest obstacle to single-precision conversion.

**A note on the STM32H750's DP-FPU hardware**: `double` operations are not software-emulated (as sometimes incorrectly claimed). They run in hardware at roughly 2× the latency of `float` operations. This makes the conversion to `float` a performance improvement rather than a necessity, and means the original `double` code works as a fallback during development.

---

## Why the Patch Init Is Ideal for a Patchable 303

The Daisy Patch Init's control layout initially appears limiting: only 4 knobs compared to the Versio platform's 7. But the Patch Init's 8 independent analog inputs (4 knobs + 4 CV jacks, not hardware-summed like the original Daisy Patch) and 2 gate inputs actually create a more expressive instrument for Eurorack integration.

The **dedicated accent gate input** (Gate In 2) is a significant upgrade over the AcidusVersio approach, where accent is a static knob value applied to all notes equally. With a separate gate, every step in a sequence can independently have accent on or off, exactly how the real TB-303 works. A sequencer like Pamela's NEW Workout or a dedicated trigger sequencer can fire accent patterns that interact with the accent sweep circuit's accumulating capacitor, producing the progressively intensifying filter sweeps that define classic acid lines.

**Four bipolar CV inputs** at ±5V with 16-bit resolution mean that filter cutoff, envelope modulation, and accent level can all be voltage-controlled simultaneously from external modulation sources: LFOs, random voltage generators, other envelopes, or expression controllers. This level of patchability is impossible on the original TB-303 and goes beyond what most 303 clones offer.

The **toggle switch** for waveform selection provides the instant, hands-on switching that a button press cannot match, which is critical during live performance. Combined with the momentary button for mode cycling, the interface is minimal but complete.

The **price point** matters too. The Patch Init retails for approximately $99, making it one of the most affordable programmable Eurorack platforms available. A complete 303 Eurorack voice for under $100 is remarkable compared to analog 303 modules that typically start at $300–500.

---

## What Makes This Project Unique in the Ecosystem

No existing Daisy-based TB-303 emulation was found during research. The closest project is AcidusVersio (targeting the Noise Engineering Versio platform), which proved that Open303 runs well on the Daisy Seed's STM32H750 but cannot be used directly on the Patch Init due to the completely different hardware abstraction layer and control layout.

Several analog Eurorack 303 voices exist (x0x Heart, DinSync RE-303 modules, Ilse), but these are expensive, limited-run, or discontinued. A fully open-source, firmware-flashable 303 voice on the readily available $99 Patch Init fills a genuine gap in the Eurorack ecosystem, bringing the acid sound to any modular setup for a fraction of the cost of dedicated 303 modules.

The Open303 engine itself has been validated across multiple platforms: JC-303 (JUCE/desktop), AcidusVersio (Versio/Eurorack), AcidBox (ESP32). This confirms its portability and audio quality. Corrode builds on this proven foundation while contributing back ARM-optimized improvements that benefit the entire Open303 community.

Beyond the port itself, Corrode's approach to the Patch Init's limited controls (dual-mode knob system, accent/slide gate multiplexing, CV-addressable parameters) serves as a reference implementation for anyone designing complex synth voices on resource-constrained Daisy hardware. The control mapping patterns and knob pickup logic are reusable across other Patch Init projects.
