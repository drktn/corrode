#include "aa_filter.h"
#include <cmath>

namespace corrode {
namespace dsp {

void AAFilter::Init(float sample_rate)
{
    DesignButterworth(sample_rate);
    Reset();
}

SampleType AAFilter::Process(SampleType input)
{
    SampleType output = input;
    for (auto& section : sections_)
        output = section.Process(output);
    return output;
}

void AAFilter::Reset()
{
    for (auto& s : sections_)
    {
        s.x1 = s.x2 = 0.0f;
        s.y1 = s.y2 = 0.0f;
    }
}

void AAFilter::DesignButterworth(float sample_rate)
{
    // Design 4th-order Butterworth LPF at Nyquist/2 (quarter-band)
    // for 2× oversampling decimation.
    // Cutoff = sample_rate / 4 (half of oversampled Nyquist)

    float fc = sample_rate / 4.0f;
    float wc = tanf(kPi * fc / sample_rate);
    float wc2 = wc * wc;

    // 4th-order Butterworth: two biquad sections
    // Section 1: Q = 1 / (2 * cos(pi/8)) ≈ 0.5412
    // Section 2: Q = 1 / (2 * cos(3*pi/8)) ≈ 1.3066
    constexpr float q_values[2] = { 0.54119610f, 1.30656296f };

    for (int i = 0; i < 2; i++)
    {
        float q = q_values[i];
        float norm = 1.0f / (1.0f + wc / q + wc2);

        sections_[i].b0 = wc2 * norm;
        sections_[i].b1 = 2.0f * sections_[i].b0;
        sections_[i].b2 = sections_[i].b0;
        sections_[i].a1 = 2.0f * (wc2 - 1.0f) * norm;
        sections_[i].a2 = (1.0f - wc / q + wc2) * norm;
    }
}

} // namespace dsp
} // namespace corrode
