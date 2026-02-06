#include "corrode.h"

namespace corrode {

void Corrode::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    engine_.Init(sample_rate);

    // Set default parameter values
    SetParameter(ParamId::Cutoff,     0.5f);
    SetParameter(ParamId::Resonance,  0.0f);
    SetParameter(ParamId::EnvMod,     0.5f);
    SetParameter(ParamId::Decay,      0.3f);
    SetParameter(ParamId::Accent,     0.5f);
    SetParameter(ParamId::SlideTime,  0.3f);
    SetParameter(ParamId::Drive,      0.0f);
    SetParameter(ParamId::Tuning,     0.5f);
}

float Corrode::Process(float external_in)
{
    // Detect external audio presence (simple envelope follower)
    external_audio_level_ = external_audio_level_ * 0.999f
                          + fabsf(external_in) * 0.001f;
    external_audio_active_ = (external_audio_level_ > kExternalThreshold);

    // Process through engine
    // TODO: Route external_in through filter when active
    float sample = engine_.Process();

    // Track envelope for CV output
    env_value_ = engine_.GetFilterEnvelopeValue();

    return sample;
}

void Corrode::SetParameter(ParamId param, float value)
{
    // Value is normalized 0..1, scale to engine range
    const auto& def = kParamDefs[static_cast<int>(param)];
    float scaled = def.min + value * (def.max - def.min);

    switch (param)
    {
        case ParamId::Cutoff:    engine_.SetCutoff(scaled);        break;
        case ParamId::Resonance: engine_.SetResonance(scaled);     break;
        case ParamId::EnvMod:    engine_.SetEnvMod(scaled);        break;
        case ParamId::Decay:     engine_.SetDecay(scaled);         break;
        case ParamId::Accent:    engine_.SetAccent(scaled);        break;
        case ParamId::SlideTime: engine_.SetSlideTime(scaled);     break;
        case ParamId::Drive:     engine_.SetDrive(scaled);         break;
        case ParamId::Tuning:    engine_.SetTuning(scaled);        break;
    }
}

float Corrode::GetParameter(ParamId param) const
{
    // TODO: Return normalized parameter value
    return 0.0f;
}

void Corrode::SetWaveform(bool sawtooth)
{
    engine_.SetWaveform(sawtooth ? dsp::Waveform::Sawtooth
                                : dsp::Waveform::Square);
}

void Corrode::NoteOn(int note, bool accent, bool slide)
{
    gate_state_   = true;
    accent_state_ = accent;
    engine_.NoteOn(note, accent, slide);
}

void Corrode::NoteOff()
{
    gate_state_   = false;
    accent_state_ = false;
    engine_.NoteOff();
}

float Corrode::GetEnvelopeValue() const { return env_value_; }
bool  Corrode::GetAccentState() const   { return accent_state_; }
bool  Corrode::GetGateState() const     { return gate_state_; }

} // namespace corrode
