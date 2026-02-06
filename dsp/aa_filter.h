#pragma once

#include "dsp_types.h"
#include <array>

namespace corrode {
namespace dsp {

/// 4th-order Butterworth lowpass anti-aliasing filter.
///
/// Replaces the original Open303 elliptic quarter-band filter which is
/// numerically unstable when converted from double to float precision.
/// Used for 2× oversampling decimation.
class AAFilter
{
public:
    void Init(float sample_rate);

    /// Process one sample (call at oversampled rate)
    SampleType Process(SampleType input);

    /// Reset filter state
    void Reset();

private:
    // Two cascaded second-order sections (biquads) = 4th order
    struct Biquad
    {
        float b0 = 0.0f, b1 = 0.0f, b2 = 0.0f;
        float a1 = 0.0f, a2 = 0.0f;
        float x1 = 0.0f, x2 = 0.0f;
        float y1 = 0.0f, y2 = 0.0f;

        SampleType Process(SampleType input)
        {
            float output = b0 * input + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
            x2 = x1; x1 = input;
            y2 = y1; y1 = output;
            return output;
        }
    };

    std::array<Biquad, 2> sections_;

    void DesignButterworth(float sample_rate);
};

} // namespace dsp
} // namespace corrode
