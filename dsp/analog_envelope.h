#pragma once

#include "dsp_types.h"

namespace corrode {
namespace dsp {

/// RC-circuit modeled exponential envelope (attack/decay/release).
/// Models both the MEG (filter) and VEG (amplitude) envelopes of the 303.
/// Original source: rosic_AnalogEnvelope.h
class AnalogEnvelope
{
public:
    void Init(float sample_rate);
    SampleType Process();

    void Trigger();
    void Release();

    void SetAttack(float ms);
    void SetDecay(float ms);
    void SetRelease(float ms);
    void SetSustain(float level);

private:
    float sample_rate_ = kDefaultSampleRate;
    EnvelopeState state_ = EnvelopeState::Idle;

    float output_   = 0.0f;
    float attack_   = 0.0f;  // Coefficient
    float decay_    = 0.0f;
    float sustain_  = 0.0f;  // Level
    float release_  = 0.0f;  // Coefficient

    float MsToCoeff(float ms) const;
};

} // namespace dsp
} // namespace corrode
