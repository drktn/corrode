#include "blep_oscillator.h"
#include "../src/util.h"

namespace corrode {
namespace dsp {

void BlepOscillator::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    phase_ = 0.0f;
    SetFrequency(440.0f);
    SetWaveform(Waveform::Sawtooth);
}

SampleType BlepOscillator::Process()
{
    SampleType output = 0.0f;
    float t = phase_;  // Normalized phase [0, 1)

    switch (waveform_)
    {
        case Waveform::Sawtooth:
            output = 2.0f * t - 1.0f;           // Naive sawtooth
            output -= PolyBlep(t);                // Apply BLEP at discontinuity
            break;

        case Waveform::Square:
        {
            // 303 square has slight duty cycle offset (~48%)
            constexpr float kDutyCycle = 0.48f;
            output = (t < kDutyCycle) ? 1.0f : -1.0f;
            output += PolyBlep(t);                           // Rising edge
            output -= PolyBlep(fmodf(t + (1.0f - kDutyCycle), 1.0f));  // Falling edge
            break;
        }
    }

    // Advance phase
    phase_ += phase_inc_;
    if (phase_ >= 1.0f)
        phase_ -= 1.0f;

    return output;
}

float BlepOscillator::PolyBlep(float t) const
{
    float dt = phase_inc_;
    if (t < dt)
    {
        t /= dt;
        return t + t - t * t - 1.0f;
    }
    else if (t > 1.0f - dt)
    {
        t = (t - 1.0f) / dt;
        return t * t + t + t + 1.0f;
    }
    return 0.0f;
}

void BlepOscillator::SetFrequency(float freq_hz)
{
    frequency_ = fast_math::clamp(freq_hz, kMinFrequency, kMaxFrequency);
    CalculatePhaseIncrement();
}

void BlepOscillator::SetWaveform(Waveform wf)
{
    waveform_ = wf;
}

void BlepOscillator::CalculatePhaseIncrement()
{
    phase_inc_ = frequency_ / sample_rate_;
}

} // namespace dsp
} // namespace corrode
