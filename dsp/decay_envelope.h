#pragma once

#include "dsp_types.h"

namespace corrode {
namespace dsp {

/// Simple decay-only envelope for the accent circuit.
/// Models the accent sweep capacitor that accumulates charge
/// across consecutive accented notes.
class DecayEnvelope
{
public:
    void Init(float sample_rate);
    SampleType Process();
    void Trigger();

    void SetDecay(float ms);

private:
    float sample_rate_ = kDefaultSampleRate;
    float output_ = 0.0f;
    float decay_  = 0.0f;
};

} // namespace dsp
} // namespace corrode
