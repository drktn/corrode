#pragma once

#include "../src/parameter_map.h"

namespace corrode {

/// Default parameter values (normalized 0..1) for factory reset.
/// These represent a "classic acid" starting point.
constexpr float kDefaultParams[static_cast<int>(ParamId::COUNT)] = {
    0.50f,  // Cutoff:     ~mid range
    0.00f,  // Resonance:  off
    0.50f,  // Env Mod:    moderate sweep
    0.30f,  // Decay:      short-medium
    0.50f,  // Accent:     moderate
    0.30f,  // Slide Time: ~60ms
    0.00f,  // Drive:      clean
    0.50f,  // Tuning:     centered (0 offset)
};

} // namespace corrode
