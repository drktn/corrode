#pragma once

#include "daisy_patch_sm.h"
#include "corrode.h"
#include "parameter_map.h"

namespace corrode {

/// Handles USB MIDI input: note on/off, CC mapping, velocity-to-accent.
class MidiHandler
{
public:
    void Init(daisy::patch_sm::DaisyPatchSM& hw);

    /// Process pending MIDI messages. Called once per audio block.
    void Process(daisy::patch_sm::DaisyPatchSM& hw, Corrode& synth);

    /// Returns true if MIDI is actively controlling pitch (overrides CV 1)
    bool IsMidiActive() const { return midi_active_; }

private:
    daisy::MidiUsbHandler midi_;
    bool midi_active_ = false;
    uint32_t midi_timeout_ = 0;

    // Velocity threshold for accent
    static constexpr uint8_t kAccentVelocity = 100;

    // MIDI channel filter (0 = all channels)
    uint8_t channel_filter_ = 0;

    void HandleNoteOn(Corrode& synth, uint8_t note, uint8_t velocity);
    void HandleNoteOff(Corrode& synth, uint8_t note);
    void HandleCC(Corrode& synth, uint8_t cc, uint8_t value);

    /// Find parameter by MIDI CC number, returns ParamId::COUNT if not found
    ParamId FindParamByCC(uint8_t cc) const;
};

} // namespace corrode
