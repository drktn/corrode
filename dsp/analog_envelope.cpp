#include "analog_envelope.h"
#include "../src/util.h"

namespace corrode {
namespace dsp {

void AnalogEnvelope::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    state_ = EnvelopeState::Idle;
    output_ = 0.0f;
    SetAttack(3.0f);     // 303: very fast attack (~3ms)
    SetDecay(200.0f);
    SetSustain(0.0f);    // 303: no sustain on filter env
    SetRelease(10.0f);
}

SampleType AnalogEnvelope::Process()
{
    switch (state_)
    {
        case EnvelopeState::Attack:
            output_ += attack_ * (1.3f - output_);  // Overshoot target for exponential shape
            if (output_ >= 1.0f)
            {
                output_ = 1.0f;
                state_ = EnvelopeState::Decay;
            }
            break;

        case EnvelopeState::Decay:
            output_ += decay_ * (sustain_ - output_);
            if (output_ <= sustain_ + 0.001f)
            {
                output_ = sustain_;
                state_ = EnvelopeState::Sustain;
            }
            break;

        case EnvelopeState::Sustain:
            output_ = sustain_;
            break;

        case EnvelopeState::Release:
            output_ += release_ * (0.0f - output_);
            if (output_ < 0.001f)
            {
                output_ = 0.0f;
                state_ = EnvelopeState::Idle;
            }
            break;

        case EnvelopeState::Idle:
        default:
            output_ = 0.0f;
            break;
    }

    return output_;
}

void AnalogEnvelope::Trigger()
{
    state_ = EnvelopeState::Attack;
}

void AnalogEnvelope::Release()
{
    state_ = EnvelopeState::Release;
}

void AnalogEnvelope::SetAttack(float ms)  { attack_  = MsToCoeff(ms); }
void AnalogEnvelope::SetDecay(float ms)   { decay_   = MsToCoeff(ms); }
void AnalogEnvelope::SetRelease(float ms) { release_ = MsToCoeff(ms); }
void AnalogEnvelope::SetSustain(float level) { sustain_ = fast_math::clamp(level, 0.0f, 1.0f); }

float AnalogEnvelope::MsToCoeff(float ms) const
{
    if (ms < 0.1f) return 1.0f;
    float samples = ms * sample_rate_ / 1000.0f;
    return 1.0f - fast_math::exp_approx(-1.0f / samples);
}

} // namespace dsp
} // namespace corrode
