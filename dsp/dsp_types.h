#pragma once

#include <cstdint>
#include <cmath>

namespace corrode {
namespace dsp {

/// Sample type: float for ARM Cortex-M7 performance
using SampleType = float;

/// Waveform selection
enum class Waveform : uint8_t
{
    Sawtooth = 0,
    Square   = 1,
};

/// Envelope state
enum class EnvelopeState : uint8_t
{
    Idle    = 0,
    Attack  = 1,
    Decay   = 2,
    Sustain = 3,
    Release = 4,
};

/// Audio processing constants
constexpr float kDefaultSampleRate  = 48000.0f;
constexpr float kOversampleFactor   = 2.0f;
constexpr int   kOversampleInt      = 2;
constexpr float kMinFrequency       = 20.0f;
constexpr float kMaxFrequency       = 20000.0f;
constexpr float kTuningA4           = 440.0f;
constexpr float kPi                 = 3.14159265358979323846f;
constexpr float kTwoPi              = 2.0f * kPi;

/// TB-303 specific constants
constexpr float kAccentDecayMs      = 200.0f;   // Accent envelope decay
constexpr float kMinSlideTimeMs     = 10.0f;
constexpr float kMaxSlideTimeMs     = 200.0f;
constexpr float kNormalDecayMs      = 200.0f;    // Normal filter envelope decay
constexpr float kAccentForcedDecayMs = 30.0f;    // Shortened decay during accent
constexpr float kFeedbackHPFreq     = 120.0f;    // Resonance feedback highpass

} // namespace dsp
} // namespace corrode
