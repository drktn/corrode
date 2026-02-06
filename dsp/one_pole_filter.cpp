#include "one_pole_filter.h"
#include "../src/util.h"

namespace corrode {
namespace dsp {

void OnePoleFilter::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    state_ = 0.0f;
    SetFrequency(1000.0f);
}

SampleType OnePoleFilter::ProcessLowpass(SampleType input)
{
    state_ += coeff_ * (input - state_);
    return state_;
}

SampleType OnePoleFilter::ProcessHighpass(SampleType input)
{
    state_ += coeff_ * (input - state_);
    return input - state_;
}

void OnePoleFilter::SetFrequency(float freq_hz)
{
    float wc = kTwoPi * fast_math::clamp(freq_hz, 1.0f, sample_rate_ * 0.49f) / sample_rate_;
    coeff_ = wc / (1.0f + wc);
}

} // namespace dsp
} // namespace corrode
