#include "open303.h"
#include "../src/util.h"

namespace corrode {
namespace dsp {

void Open303::Init(float sample_rate)
{
    sample_rate_     = sample_rate;
    oversample_rate_ = sample_rate * kOversampleFactor;

    oscillator_.Init(oversample_rate_);
    filter_.Init(oversample_rate_);
    filter_env_.Init(sample_rate);
    amp_env_.Init(sample_rate);
    accent_env_.Init(sample_rate);

    // Default 303-like settings
    SetCutoff(1000.0f);
    SetResonance(0.0f);
    SetEnvMod(50.0f);
    SetDecay(200.0f);
    SetAccent(50.0f);
    SetSlideTime(60.0f);
    SetDrive(0.0f);
    SetTuning(0.0f);
    SetWaveform(Waveform::Sawtooth);
}

SampleType Open303::Process()
{
    // Update slide pitch
    UpdatePitch();

    // Get envelope values
    float filter_env_val = filter_env_.Process();
    float amp_env_val    = amp_env_.Process();
    float accent_env_val = accent_active_ ? accent_env_.Process() : 0.0f;

    // Calculate effective cutoff with envelope modulation and accent
    float effective_cutoff = cutoff_base_
                           + env_mod_ * filter_env_val * 100.0f
                           + accent_level_ * accent_env_val * 50.0f;
    effective_cutoff = fast_math::clamp(effective_cutoff, kMinFrequency, kMaxFrequency);

    filter_.SetCutoff(effective_cutoff);

    // Process with 2× oversampling
    // TODO: Implement proper oversampling with anti-aliasing filter
    SampleType osc_out = oscillator_.Process();

    // Apply drive (pre-filter saturation)
    if (drive_amount_ > 0.001f)
    {
        float drive_gain = 1.0f + drive_amount_ * 9.0f;  // 1× to 10× gain
        osc_out = fast_math::tanh_approx(osc_out * drive_gain);
    }

    // Filter
    SampleType filtered = filter_.Process(osc_out);

    // VCA
    float vca_gain = amp_env_val;
    if (accent_active_)
        vca_gain += accent_level_ * accent_env_val * 0.01f;
    vca_gain = fast_math::clamp(vca_gain, 0.0f, 1.5f);

    return filtered * vca_gain;
}

void Open303::NoteOn(int midi_note, bool accent, bool slide)
{
    target_note_   = static_cast<float>(midi_note) + tuning_offset_;
    accent_active_ = accent;

    if (slide && gate_active_)
    {
        // Slide: don't retrigger envelopes, just glide pitch
        slide_active_ = true;
    }
    else
    {
        // Normal note: set pitch immediately, trigger envelopes
        if (!slide)
            current_note_ = target_note_;
        slide_active_ = false;

        oscillator_.SetFrequency(
            fast_math::clamp(
                440.0f * powf(2.0f, (current_note_ - 69.0f) / 12.0f),
                kMinFrequency, kMaxFrequency));

        filter_env_.Trigger();
        amp_env_.Trigger();

        if (accent)
        {
            accent_env_.Trigger();
            // Accent forces short filter decay (authentic 303 behavior)
            filter_env_.SetDecay(kAccentForcedDecayMs);
        }
    }

    gate_active_ = true;
}

void Open303::NoteOff()
{
    gate_active_   = false;
    slide_active_  = false;
    accent_active_ = false;

    filter_env_.Release();
    amp_env_.Release();
}

void Open303::UpdatePitch()
{
    if (slide_active_)
    {
        // Exponential slide toward target note
        current_note_ += slide_coeff_ * (target_note_ - current_note_);

        float freq = 440.0f * powf(2.0f, (current_note_ - 69.0f) / 12.0f);
        oscillator_.SetFrequency(fast_math::clamp(freq, kMinFrequency, kMaxFrequency));

        // Snap when close enough
        if (fabsf(target_note_ - current_note_) < 0.01f)
        {
            current_note_ = target_note_;
            slide_active_ = false;
        }
    }
}

// --- Parameter setters ---

void Open303::SetCutoff(float freq_hz)
{
    cutoff_base_ = fast_math::clamp(freq_hz, kMinFrequency, kMaxFrequency);
    filter_.SetCutoff(cutoff_base_);
}

void Open303::SetResonance(float percent)
{
    resonance_ = fast_math::clamp(percent, 0.0f, 100.0f);
    filter_.SetResonance(resonance_);
}

void Open303::SetEnvMod(float percent)
{
    env_mod_ = fast_math::clamp(percent, 0.0f, 100.0f);
}

void Open303::SetDecay(float ms)
{
    filter_env_.SetDecay(fast_math::clamp(ms, 30.0f, 3000.0f));
}

void Open303::SetAccent(float percent)
{
    accent_level_ = fast_math::clamp(percent, 0.0f, 100.0f);
}

void Open303::SetSlideTime(float ms)
{
    CalculateSlideCoefficient(fast_math::clamp(ms, kMinSlideTimeMs, kMaxSlideTimeMs));
}

void Open303::SetDrive(float percent)
{
    drive_amount_ = fast_math::clamp(percent, 0.0f, 100.0f) / 100.0f;
}

void Open303::SetTuning(float semitones)
{
    tuning_offset_ = fast_math::clamp(semitones, -1.0f, 1.0f);
}

void Open303::SetWaveform(Waveform wf)
{
    oscillator_.SetWaveform(wf);
}

float Open303::GetFilterEnvelopeValue() const
{
    return 0.0f;  // TODO: Return last filter envelope value
}

void Open303::CalculateSlideCoefficient(float time_ms)
{
    // RC time constant: coeff = 1 - exp(-1 / (time_ms * sampleRate / 1000))
    float time_samples = time_ms * sample_rate_ / 1000.0f;
    slide_coeff_ = 1.0f - fast_math::exp_approx(-1.0f / time_samples);
}

} // namespace dsp
} // namespace corrode
