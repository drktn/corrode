#pragma once

#include "dsp_types.h"

namespace corrode {
namespace dsp {

/// Simple one-pole lowpass/highpass filter for utility use.
/// Original: rosic_OnePoleFilter
class OnePoleFilter
{
public:
    void Init(float sample_rate);
    SampleType ProcessLowpass(SampleType input);
    SampleType ProcessHighpass(SampleType input);
    void SetFrequency(float freq_hz);

private:
    float sample_rate_ = kDefaultSampleRate;
    float coeff_ = 0.0f;
    float state_ = 0.0f;
};

} // namespace dsp
} // namespace corrode
