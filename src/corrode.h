#pragma once

#include "dsp/open303.h"
#include "parameter_map.h"

namespace corrode {

/// Top-level synth voice: wraps the Open303 engine with parameter management,
/// external audio routing, and output state tracking.
class Corrode
{
public:
    void Init(float sample_rate);

    /// Process one sample. If external_in is non-zero and external audio
    /// routing is active, it replaces the internal oscillator.
    float Process(float external_in = 0.0f);

    /// Set a parameter by ID (used by Controls and MidiHandler)
    void SetParameter(ParamId param, float value);

    /// Get current parameter value (for knob pickup comparison)
    float GetParameter(ParamId param) const;

    /// Set waveform: true = sawtooth, false = square
    void SetWaveform(bool sawtooth);

    /// Trigger note on with MIDI note number
    void NoteOn(int note, bool accent = false, bool slide = false);
    void NoteOff();

    /// State accessors for gate/CV outputs
    float GetEnvelopeValue() const;
    bool  GetAccentState() const;
    bool  GetGateState() const;

private:
    dsp::Open303 engine_;
    float sample_rate_ = 48000.0f;

    // State tracking
    bool  gate_state_   = false;
    bool  accent_state_ = false;
    float env_value_    = 0.0f;

    // External audio detection
    bool  external_audio_active_ = false;
    float external_audio_level_  = 0.0f;
    static constexpr float kExternalThreshold = 0.001f;
};

} // namespace corrode
