#pragma once

#include "dsp_types.h"

namespace corrode {
namespace dsp {

/// TeeBeeFilter: 4-pole diode ladder lowpass filter modeling the TB-303.
///
/// Features:
/// - 4 cascaded one-pole stages (24 dB/oct nominal)
/// - tanh saturation in feedback path (authentic nonlinear behavior)
/// - Highpass filter (~120 Hz) in resonance feedback loop
/// - Does NOT self-oscillate at max resonance (authentic 303 behavior)
/// - First stage tuned an octave higher (mismatched capacitor modeling)
///
/// Original source: rosic_TeeBeeFilter.h / rosic_TeeBeeFilter.cpp
class TeeBeeFilter
{
public:
    void Init(float sample_rate);
    SampleType Process(SampleType input);

    void SetCutoff(float freq_hz);
    void SetResonance(float percent);

private:
    float sample_rate_ = kDefaultSampleRate;
    float cutoff_freq_ = 1000.0f;
    float resonance_   = 0.0f;

    // Filter state (4 cascaded one-pole stages)
    float y1_ = 0.0f;
    float y2_ = 0.0f;
    float y3_ = 0.0f;
    float y4_ = 0.0f;

    // Feedback highpass state
    float feedback_hp_state_ = 0.0f;

    // Coefficients
    float g_  = 0.0f;   // Cutoff coefficient
    float k_  = 0.0f;   // Resonance feedback gain

    void CalculateCoefficients();
};

} // namespace dsp
} // namespace corrode
