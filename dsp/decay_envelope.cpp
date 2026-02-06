#include "decay_envelope.h"
#include "../src/util.h"

namespace corrode {
namespace dsp {

void DecayEnvelope::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    output_ = 0.0f;
    SetDecay(kAccentDecayMs);
}

SampleType DecayEnvelope::Process()
{
    output_ *= decay_;
    return output_;
}

void DecayEnvelope::Trigger()
{
    // Accumulating behavior: add to current level rather than reset
    // This models the 303's accent capacitor that builds up
    // across consecutive accented notes
    output_ = fast_math::clamp(output_ + 1.0f, 0.0f, 2.0f);
}

void DecayEnvelope::SetDecay(float ms)
{
    float samples = ms * sample_rate_ / 1000.0f;
    decay_ = fast_math::exp_approx(-1.0f / samples);
}

} // namespace dsp
} // namespace corrode
