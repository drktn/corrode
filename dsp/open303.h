#pragma once

#include "dsp_types.h"
#include "blep_oscillator.h"
#include "tee_bee_filter.h"
#include "analog_envelope.h"
#include "decay_envelope.h"

namespace corrode {
namespace dsp {

/// Open303: Main TB-303 synthesizer engine.
///
/// Ported from Robin Schmidt's Open303 (https://github.com/RobinSchmidt/Open303).
/// Modified for single-precision float processing and ARM Cortex-M7 optimization.
///
/// Signal flow:
///   Oscillator (BLEP saw/square)
///     → Diode ladder filter (TeeBeeFilter)
///       ← Filter envelope (AnalogEnvelope)
///       ← Accent circuit (DecayEnvelope + accumulation)
///     → VCA
///       ← Amplitude envelope (AnalogEnvelope)
///     → Output
///
/// Original source: rosic_Open303.h / rosic_Open303.cpp
class Open303
{
public:
    void Init(float sample_rate);

    /// Process one output sample
    SampleType Process();

    // --- Note control ---
    void NoteOn(int midi_note, bool accent = false, bool slide = false);
    void NoteOff();

    // --- Parameter setters ---
    void SetCutoff(float freq_hz);
    void SetResonance(float percent);
    void SetEnvMod(float percent);
    void SetDecay(float ms);
    void SetAccent(float percent);
    void SetSlideTime(float ms);
    void SetDrive(float percent);
    void SetTuning(float semitones);
    void SetWaveform(Waveform wf);

    // --- State queries ---
    float GetFilterEnvelopeValue() const;

private:
    float sample_rate_    = kDefaultSampleRate;
    float oversample_rate_ = kDefaultSampleRate * kOversampleFactor;

    // DSP components
    BlepOscillator oscillator_;
    TeeBeeFilter   filter_;
    AnalogEnvelope filter_env_;    // MEG (Modulation Envelope Generator)
    AnalogEnvelope amp_env_;       // VEG (Volume Envelope Generator)
    DecayEnvelope  accent_env_;

    // State
    float current_note_  = 48.0f;
    float target_note_   = 48.0f;
    float slide_coeff_   = 0.0f;
    bool  slide_active_  = false;
    bool  accent_active_ = false;
    bool  gate_active_   = false;

    // Parameters (stored as engine values, not normalized)
    float cutoff_base_   = 1000.0f;
    float resonance_     = 0.0f;
    float env_mod_       = 50.0f;
    float accent_level_  = 50.0f;
    float drive_amount_  = 0.0f;
    float tuning_offset_ = 0.0f;

    // Anti-aliasing filter for oversampled output
    // TODO: Replace with aa_filter.h Butterworth implementation

    // Internal processing
    SampleType ProcessOversampled();
    void UpdatePitch();
    void CalculateSlideCoefficient(float time_ms);
};

} // namespace dsp
} // namespace corrode
