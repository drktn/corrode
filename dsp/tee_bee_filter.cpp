#include "tee_bee_filter.h"
#include "../src/util.h"

namespace corrode {
namespace dsp {

void TeeBeeFilter::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    y1_ = y2_ = y3_ = y4_ = 0.0f;
    feedback_hp_state_ = 0.0f;
    SetCutoff(1000.0f);
    SetResonance(0.0f);
}

SampleType TeeBeeFilter::Process(SampleType input)
{
    // TODO: Port the full TeeBeeFilter::getSample() from Open303.
    //
    // The authentic implementation includes:
    // 1. Feedback with tanh saturation: feedback = tanh(k_ * y4_)
    // 2. Highpass filter on feedback: removes DC, ~120 Hz corner
    // 3. Four cascaded one-pole stages: y[n] = y[n-1] + g * (in - y[n-1])
    // 4. First stage uses 2*g (mismatched capacitor)
    //
    // Placeholder: simple one-pole lowpass for testing
    float feedback = fast_math::tanh_approx(k_ * y4_);
    float hp = feedback - feedback_hp_state_;
    feedback_hp_state_ = feedback * 0.997f;  // HP coefficient (~120 Hz at 96kHz)

    float in = input - hp;

    // Stage 1 (2× cutoff — mismatched capacitor)
    y1_ += 2.0f * g_ * (in - y1_);
    // Stage 2
    y2_ += g_ * (y1_ - y2_);
    // Stage 3
    y3_ += g_ * (y2_ - y3_);
    // Stage 4
    y4_ += g_ * (y3_ - y4_);

    return y4_;
}

void TeeBeeFilter::SetCutoff(float freq_hz)
{
    cutoff_freq_ = fast_math::clamp(freq_hz, kMinFrequency, kMaxFrequency);
    CalculateCoefficients();
}

void TeeBeeFilter::SetResonance(float percent)
{
    resonance_ = fast_math::clamp(percent, 0.0f, 100.0f);
    CalculateCoefficients();
}

void TeeBeeFilter::CalculateCoefficients()
{
    // Cutoff coefficient (bilinear transform approximation)
    float wc = kTwoPi * cutoff_freq_ / sample_rate_;
    g_ = fast_math::clamp(wc / (1.0f + wc), 0.0f, 1.0f);

    // Resonance: scale 0..100% to feedback gain
    // 303 filter does not self-oscillate, so max k < 4.0
    k_ = resonance_ * 0.038f;  // 0..~3.8
}

} // namespace dsp
} // namespace corrode
