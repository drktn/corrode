# Optimization Notes

Performance measurements and optimization decisions for the STM32H750 ARM Cortex-M7 target.

## Profiling Method

CPU usage measured via DWT cycle counter:

```cpp
CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
DWT->CYCCNT = 0;
DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

uint32_t start = DWT->CYCCNT;
// ... audio callback ...
uint32_t cycles = DWT->CYCCNT - start;
```

Budget: 480,000 cycles per 1 ms block (48 samples at 48 kHz, 480 MHz clock).

## Measurements

*TODO: Fill in after Phase 3 implementation*

| Configuration | Cycles/Block | CPU % |
|---------------|-------------|-------|
| Original (double, elliptic AA) | TBD | TBD |
| Float conversion only | TBD | TBD |
| + Butterworth AA filter | TBD | TBD |
| + Fast tanh() | TBD | TBD |
| + All optimizations | TBD | TBD |

## Key Decisions

### float vs double

The STM32H750's FPv5-D16 FPU handles both precisions in hardware. Double-precision operations are NOT software-emulated (a common misconception), but they take approximately 2× the cycles of single-precision. For a mono synth voice, either precision is feasible. Float was chosen for the performance headroom it provides, leaving room for potential polyphony or additional processing.

### Oversampling Strategy

The original Open303 uses 2× oversampling with an elliptic decimation filter. Two alternatives were considered:

1. **Keep 2× with Butterworth decimation** (chosen): Preserves the original audio character while using a float-stable filter design.
2. **Remove oversampling entirely**: The polyBLEP oscillator provides good anti-aliasing on its own, but the filter's nonlinear feedback generates harmonics that benefit from oversampling. Removing it would be a worthwhile experiment if CPU budget becomes tight.

### tanh Approximation Selection

Three approximations were evaluated:

- **Padé rational** `x(27+x²)/(27+9x²)`: 8 cycles, max error 0.004 — **chosen**
- **Polynomial** `x - x³/3`: 4 cycles, max error 0.13 — too inaccurate for filter feedback
- **Lookup + interpolation**: 12 cycles, max error 0.001 — more accurate but slower and uses cache

### Compiler Optimization

Using `-O2` for release builds. `-O3` showed marginal improvement but increased code size significantly. `-Ofast` enables `-ffast-math` which breaks IEEE compliance; not used due to potential for subtle DSP artifacts.
