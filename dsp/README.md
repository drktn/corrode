# DSP Engine: Modified Open303

This directory contains the TB-303 DSP engine, adapted from Robin Schmidt's [Open303](https://github.com/RobinSchmidt/Open303) for the ARM Cortex-M7 target (STM32H750 on the Daisy Patch Init).

## Origin

The original Open303 codebase (files prefixed `rosic_`) was developed as a desktop VST plugin using double-precision arithmetic and C++03 patterns. This port preserves the DSP algorithms while optimizing for embedded, real-time audio processing.

## File Mapping

| Original (Open303)          | Corrode                  | Notes                           |
|-----------------------------|--------------------------|---------------------------------|
| rosic_Open303.h/cpp         | open303.h/cpp            | Main engine class               |
| rosic_TeeBeeFilter.h/cpp    | tee_bee_filter.h/cpp     | Diode ladder filter             |
| rosic_BlepOscillator.h/cpp  | blep_oscillator.h/cpp    | Band-limited oscillator         |
| rosic_AnalogEnvelope.h/cpp  | analog_envelope.h/cpp    | Exponential envelope            |
| rosic_DecayEnvelope.h/cpp   | decay_envelope.h/cpp     | Accent decay envelope           |
| rosic_OnePoleFilter.h/cpp   | one_pole_filter.h/cpp    | Utility one-pole filter         |
| rosic_LeakyIntegrator.h/cpp | leaky_integrator.h/cpp   | DC blocking / smoothing         |
| rosic_EllipticQuarterBandFilter | aa_filter.h/cpp      | **Replaced** with Butterworth   |
| rosic_FourierTransformer    | *removed*                | Not needed for real-time        |
| rosic_AcidSequencer         | *removed*                | External sequencing only        |
| rosic_AcidPattern           | *removed*                | External sequencing only        |

## Modification Summary

See `docs/open303_modifications.md` for the detailed changelog with rationale and measurements.

### Performance

- All `double` → `float` (SampleType typedef in dsp_types.h)
- `tanh()` → Padé rational approximation
- `exp()` → Schraudolph's bit-manipulation approximation
- Elliptic AA filter → 4th-order Butterworth (float-stable)
- Pre-computed pitch-to-frequency lookup table

### Code Quality

- C++17: constexpr, enum class, std::array, RAII
- Snake_case file and variable naming
- Removed virtual functions from audio path
- Stack-allocated engine (~2-6 KB RAM)

### Features Added

- External audio input routing through filter/VCA
- Soft-clipping drive stage (pre-filter)
- Accent capacitor accumulation (authentic 303 behavior)
