#include "controls.h"

using namespace daisy;
using namespace patch_sm;

namespace corrode {

void Controls::Init()
{
    mode_ = ControlMode::ToneA;
    for (int i = 0; i < static_cast<int>(ParamId::COUNT); i++)
    {
        param_values_[i] = kParamDefs[i].default_value;
        smoothed_[i] = kParamDefs[i].default_value;
    }
    for (int i = 0; i < kKnobsPerMode; i++)
        knob_picked_up_[i] = true;  // Allow immediate control on startup
}

void Controls::Process(DaisyPatchSM& hw, Corrode& synth)
{
    hw.ProcessAllControls();

    ProcessButton(hw);
    ProcessToggle(hw, synth);
    ProcessKnobs(hw, synth);
    ProcessCV(hw, synth);
    ProcessGates(hw, synth);
}

void Controls::ProcessButton(DaisyPatchSM& hw)
{
    // TODO: Read button state with debounce
    // On press: toggle mode, reset knob pickup flags
    bool button = false; // hw.button.RisingEdge() — adapt to Patch Init API

    if (button && !button_prev_)
    {
        mode_ = (mode_ == ControlMode::ToneA)
              ? ControlMode::CharacterB
              : ControlMode::ToneA;

        // Reset pickup: knobs must cross stored value before updating
        for (int i = 0; i < kKnobsPerMode; i++)
            knob_picked_up_[i] = false;
    }
    button_prev_ = button;
}

void Controls::ProcessKnobs(DaisyPatchSM& hw, Corrode& synth)
{
    // Select parameter mapping based on current mode
    const ParamId* param_map = (mode_ == ControlMode::ToneA)
                             ? kModeAParams
                             : kModeBParams;

    for (int k = 0; k < kKnobsPerMode; k++)
    {
        // Read knob (0..1)
        // Patch Init knobs are on CV_1..CV_4 analog inputs
        float raw = hw.GetAdcValue(k);

        int param_idx = static_cast<int>(param_map[k]);

        // Knob pickup: only update if knob has crossed the stored value
        if (!knob_picked_up_[k])
        {
            if (CheckPickup(k, raw, param_values_[param_idx]))
                knob_picked_up_[k] = true;
            else
                continue;  // Skip until picked up
        }

        // Smooth and apply
        float smoothed = SmoothParam(param_idx, raw);
        param_values_[param_idx] = smoothed;
        synth.SetParameter(param_map[k], smoothed);
    }
}

void Controls::ProcessCV(DaisyPatchSM& hw, Corrode& synth)
{
    // CV 1: V/Oct pitch — handled in ProcessGates with note calculation
    // CV 2: Cutoff modulation (additive to knob value)
    // CV 3: Envelope Mod modulation
    // CV 4: Accent CV (continuous)

    // TODO: Read bipolar CV inputs, scale ±5V to ±1.0, add to base params
    // float cv2 = hw.GetAdcValue(CV_5);  // Adapt to Patch Init CV input pins
}

void Controls::ProcessGates(DaisyPatchSM& hw, Corrode& synth)
{
    bool gate1 = dsy_gpio_read(&hw.gate_in_1);
    bool gate2 = dsy_gpio_read(&hw.gate_in_2);

    bool gate1_rising  = gate1 && !gate1_prev_;
    bool gate1_falling = !gate1 && gate1_prev_;

    if (gate1_rising)
    {
        // Read V/Oct from CV 1 and convert to MIDI note
        // TODO: Implement CV-to-note conversion with calibration
        float cv_pitch = 0.0f; // hw.GetAdcValue(CV_1)
        int note = 48 + static_cast<int>(cv_pitch * 12.0f);  // Placeholder

        bool accent = gate2;  // Gate 2 high at note-on = accent

        // Slide detection: Gate 2 high while Gate 1 was already high
        // (requires tied/legato notes from sequencer)
        bool slide = false;  // TODO: Implement slide detection

        synth.NoteOn(note, accent, slide);
    }
    else if (gate1_falling)
    {
        synth.NoteOff();
    }

    gate1_prev_ = gate1;
    gate2_prev_ = gate2;
}

void Controls::ProcessToggle(DaisyPatchSM& hw, Corrode& synth)
{
    // TODO: Read toggle switch state from Patch Init GPIO
    // bool sawtooth = dsy_gpio_read(&hw.toggle);
    // synth.SetWaveform(sawtooth);
}

void Controls::UpdateLed(DaisyPatchSM& hw)
{
    if (mode_ == ControlMode::ToneA)
    {
        // Steady on
        dsy_gpio_write(&hw.gate_out_1, true);  // Adapt to LED pin
    }
    else
    {
        // Blink at ~3 Hz
        led_timer_++;
        if (led_timer_ > 150)
        {
            led_timer_ = 0;
            led_state_ = !led_state_;
        }
        dsy_gpio_write(&hw.gate_out_1, led_state_);
    }
}

float Controls::SmoothParam(int idx, float raw)
{
    smoothed_[idx] += kSmoothCoeff * (raw - smoothed_[idx]);
    return smoothed_[idx];
}

bool Controls::CheckPickup(int knob_idx, float raw, float stored)
{
    // Pickup threshold: knob must be within 2% of stored value
    constexpr float kPickupThreshold = 0.02f;
    return fabsf(raw - stored) < kPickupThreshold;
}

} // namespace corrode
