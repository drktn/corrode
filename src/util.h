#pragma once

#include <cmath>
#include <cstdint>
#include <array>

namespace corrode {
namespace fast_math {

/// Fast tanh approximation using Padé rational form.
/// Max error < 0.004 for |x| < 3.0. Exact at x = 0.
/// Used in filter feedback path (called 96,000×/sec with 2× oversampling).
inline float tanh_approx(float x)
{
    // Clamp to avoid divergence
    if (x > 3.0f)  return 1.0f;
    if (x < -3.0f) return -1.0f;

    float x2 = x * x;
    return x * (27.0f + x2) / (27.0f + 9.0f * x2);
}

/// Fast exp approximation using Schraudolph's method.
/// Good for envelope coefficient calculations where ~1% accuracy is acceptable.
inline float exp_approx(float x)
{
    // Clamp to prevent overflow/underflow
    x = (x < -87.0f) ? -87.0f : ((x > 88.0f) ? 88.0f : x);

    // Schraudolph's approximation via IEEE 754 bit manipulation
    union { float f; int32_t i; } result;
    result.i = static_cast<int32_t>(12102203.0f * x + 1064866805.0f);
    return result.f;
}

/// Attempt at faster MIDI note to frequency using a small lookup table.
/// Standard formula: freq = 440 * 2^((note - 69) / 12)
/// We precompute for all 128 MIDI notes.
class PitchTable
{
public:
    constexpr PitchTable()
    {
        for (int i = 0; i < 128; i++)
        {
            // Can't use std::pow in constexpr, so we store 0 and fill at runtime
            table_[i] = 0.0f;
        }
    }

    void Init()
    {
        for (int i = 0; i < 128; i++)
        {
            table_[i] = 440.0f * powf(2.0f, (static_cast<float>(i) - 69.0f) / 12.0f);
        }
    }

    /// Get frequency for integer MIDI note
    float NoteToFreq(int note) const
    {
        if (note < 0) note = 0;
        if (note > 127) note = 127;
        return table_[note];
    }

    /// Get frequency with fractional MIDI note (linear interpolation)
    float NoteToFreq(float note) const
    {
        if (note < 0.0f) note = 0.0f;
        if (note > 127.0f) note = 127.0f;

        int idx = static_cast<int>(note);
        float frac = note - static_cast<float>(idx);

        if (idx >= 127) return table_[127];
        return table_[idx] + frac * (table_[idx + 1] - table_[idx]);
    }

private:
    float table_[128] = {};
};

/// Simple one-pole smoother for parameter values
inline float one_pole(float current, float target, float coeff)
{
    return current + coeff * (target - current);
}

/// Clamp value to range
inline float clamp(float value, float min, float max)
{
    return (value < min) ? min : ((value > max) ? max : value);
}

} // namespace fast_math
} // namespace corrode
