#pragma once

#include <cstdint>

namespace corrode {

/// All controllable parameters
enum class ParamId : uint8_t
{
    // Mode A — Tone
    Cutoff    = 0,  // CTRL 1 (Mode A)
    Resonance = 1,  // CTRL 2 (Mode A)
    EnvMod    = 2,  // CTRL 3 (Mode A)
    Decay     = 3,  // CTRL 4 (Mode A)

    // Mode B — Character
    Accent    = 4,  // CTRL 1 (Mode B)
    SlideTime = 5,  // CTRL 2 (Mode B)
    Drive     = 6,  // CTRL 3 (Mode B)
    Tuning    = 7,  // CTRL 4 (Mode B)

    COUNT     = 8
};

/// Parameter definition: display range and MIDI CC
struct ParamDef
{
    const char* name;
    float       min;
    float       max;
    float       default_value;  // normalized 0..1
    uint8_t     midi_cc;
};

/// Parameter definitions table
constexpr ParamDef kParamDefs[static_cast<int>(ParamId::COUNT)] = {
    // Mode A — Tone
    { "Cutoff",     50.0f,   12000.0f, 0.5f,  74 },
    { "Resonance",  0.0f,    100.0f,   0.0f,  71 },
    { "Env Mod",    0.0f,    100.0f,   0.5f,  70 },
    { "Decay",      30.0f,   3000.0f,  0.3f,  75 },

    // Mode B — Character
    { "Accent",     0.0f,    100.0f,   0.5f,  76 },
    { "Slide Time", 10.0f,   200.0f,   0.3f,   5 },
    { "Drive",      0.0f,    100.0f,   0.0f,  77 },
    { "Tuning",    -1.0f,    1.0f,     0.5f,  94 },
};

/// Number of knobs per mode
constexpr int kKnobsPerMode = 4;

/// Knob-to-parameter mapping per mode
constexpr ParamId kModeAParams[kKnobsPerMode] = {
    ParamId::Cutoff, ParamId::Resonance, ParamId::EnvMod, ParamId::Decay
};

constexpr ParamId kModeBParams[kKnobsPerMode] = {
    ParamId::Accent, ParamId::SlideTime, ParamId::Drive, ParamId::Tuning
};

} // namespace corrode
