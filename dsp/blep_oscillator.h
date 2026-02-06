#pragma once

#include "dsp_types.h"

namespace corrode {
namespace dsp {

/// Band-limited oscillator using polyBLEP anti-aliasing.
/// Generates sawtooth and square waveforms with minimal aliasing.
/// Original source: rosic_BlepOscillator.h
class BlepOscillator
{
public:
    void Init(float sample_rate);
    SampleType Process();

    void SetFrequency(float freq_hz);
    void SetWaveform(Waveform wf);

private:
    float sample_rate_ = kDefaultSampleRate;
    float frequency_   = 440.0f;
    float phase_       = 0.0f;
    float phase_inc_   = 0.0f;
    Waveform waveform_ = Waveform::Sawtooth;

    /// PolyBLEP correction term
    float PolyBlep(float t) const;
    void CalculatePhaseIncrement();
};

} // namespace dsp
} // namespace corrode
