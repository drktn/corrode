#pragma once

#include "daisy_patch_sm.h"
#include "corrode.h"
#include "parameter_map.h"

namespace corrode {

/// Control mode: button toggles between A and B
enum class ControlMode : uint8_t
{
    ToneA      = 0,  // LED steady
    CharacterB = 1,  // LED blinking
};

/// Handles all hardware control reading, mode switching, knob pickup,
/// parameter smoothing, CV input processing, and gate detection.
class Controls
{
public:
    void Init();

    /// Read all controls and update synth parameters.
    /// Called once per audio block from AudioCallback.
    void Process(daisy::patch_sm::DaisyPatchSM& hw, Corrode& synth);

    /// Update LED indicator (called from main loop)
    void UpdateLed(daisy::patch_sm::DaisyPatchSM& hw);

private:
    ControlMode mode_ = ControlMode::ToneA;

    // Knob pickup: stores last known position per parameter
    float param_values_[static_cast<int>(ParamId::COUNT)] = {};
    bool  knob_picked_up_[kKnobsPerMode] = {};

    // Parameter smoothing (one-pole lowpass per parameter)
    float smoothed_[static_cast<int>(ParamId::COUNT)] = {};
    static constexpr float kSmoothCoeff = 0.01f;  // ~3ms at 48kHz/48 block

    // Button debounce
    bool  button_prev_ = false;
    uint32_t button_debounce_timer_ = 0;
    static constexpr uint32_t kDebounceMs = 50;

    // Gate state tracking for accent/slide logic
    bool gate1_prev_ = false;
    bool gate2_prev_ = false;

    // LED blink state
    uint32_t led_timer_ = 0;
    bool     led_state_ = false;

    // Helpers
    void ProcessButton(daisy::patch_sm::DaisyPatchSM& hw);
    void ProcessKnobs(daisy::patch_sm::DaisyPatchSM& hw, Corrode& synth);
    void ProcessCV(daisy::patch_sm::DaisyPatchSM& hw, Corrode& synth);
    void ProcessGates(daisy::patch_sm::DaisyPatchSM& hw, Corrode& synth);
    void ProcessToggle(daisy::patch_sm::DaisyPatchSM& hw, Corrode& synth);

    float SmoothParam(int idx, float raw);
    bool  CheckPickup(int knob_idx, float raw, float stored);
};

} // namespace corrode
