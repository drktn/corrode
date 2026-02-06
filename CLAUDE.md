# Corrode

TB-303 acid synth for the Electro-Smith Daisy Patch Init (ARM Cortex-M7).
C++17 embedded firmware — no OS, bare-metal. DSP engine based on
Robin Schmidt's Open303.

## Build

```bash
make                 # release build
make DEBUG=1         # debug build (-Og -g3)
make clean           # remove artifacts
make program-dfu     # flash via USB DFU
```

Toolchain: ARM GCC cross-compiler (`arm-none-eabi-gcc`), Make, OpenOCD.
Install via [Daisy Toolchain](https://daisy.audio/tutorials/cpp-dev-env/).

### First-time setup (submodules)

```bash
git submodule update --init --recursive
cd lib/libDaisy && make && cd ../..
cd lib/DaisySP && make && cd ../..
make
```

## Test

Test files in `test/` — standalone C++ files compiled and run on host:

```bash
g++ -std=c++17 -Isrc -Idsp -o test_fast_math test/test_fast_math.cpp && ./test_fast_math
```

No test framework; each file has its own `main()` with assertions.

## Project structure

```text
src/           App layer (main, controls, MIDI, parameter mapping)
dsp/           Open303 DSP engine (oscillator, filter, envelopes)
lib/libDaisy/  Daisy HAL submodule (do not edit)
lib/DaisySP/   Daisy DSP library submodule (do not edit)
test/          Host-side test files
docs/          Architecture, calibration, optimization notes
hardware/      BOM, faceplate design
tools/         Python scripts (envelope plots, freq response, LUT gen)
presets/       Default parameter values
```

## Key conventions

- **Single-precision float** for all DSP — no `double` in signal path
- **No raw pointers** — use `std::array`, RAII, references
- **`constexpr`** for compile-time constants and LUTs
- **Block processing** — process full audio blocks (48 samples), not
  single samples
- **Fast math** — `tanh`/`exp` approximations in `dsp/fast_math.h`
- Compiler flags: `-std=gnu++17`, `-O2` (release), `-Og -g3` (debug)
- Makefile inherits from `lib/libDaisy/core/Makefile` for ARM
  cross-compilation targets
