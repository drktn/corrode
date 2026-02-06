#pragma once
#include "dsp_types.h"

namespace corrode {
namespace dsp {

/// Leaky integrator: DC-blocking filter and signal smoothing.
/// Original: rosic_LeakyIntegrator
class LeakyIntegrator
{
public:
    void Init(float sample_rate) { sample_rate_ = sample_rate; state_ = 0.0f; }
    SampleType Process(SampleType input) { state_ = coeff_ * state_ + (1.0f - coeff_) * input; return state_; }
    void SetTimeConstant(float ms);
    void Reset() { state_ = 0.0f; }

private:
    float sample_rate_ = kDefaultSampleRate;
    float coeff_ = 0.999f;
    float state_ = 0.0f;
};

} // namespace dsp
} // namespace corrode
