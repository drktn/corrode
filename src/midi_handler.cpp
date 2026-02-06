#include "midi_handler.h"

using namespace daisy;

namespace corrode {

void MidiHandler::Init(patch_sm::DaisyPatchSM& hw)
{
    MidiUsbHandler::Config midi_cfg;
    midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
    midi_.Init(midi_cfg);
    midi_.StartReceive();
}

void MidiHandler::Process(patch_sm::DaisyPatchSM& hw, Corrode& synth)
{
    midi_.Listen();

    while (midi_.HasEvents())
    {
        MidiEvent event = midi_.PopEvent();

        // Channel filter (0 = omni)
        if (channel_filter_ > 0 && event.channel != (channel_filter_ - 1))
            continue;

        switch (event.type)
        {
            case NoteOn:
                HandleNoteOn(synth, event.data[0], event.data[1]);
                midi_active_ = true;
                midi_timeout_ = 0;
                break;

            case NoteOff:
                HandleNoteOff(synth, event.data[0]);
                break;

            case ControlChange:
                HandleCC(synth, event.data[0], event.data[1]);
                break;

            default:
                break;
        }
    }

    // MIDI activity timeout: revert to CV control after ~2 seconds of silence
    if (midi_active_)
    {
        midi_timeout_++;
        if (midi_timeout_ > 2000)  // Approximate: depends on block rate
            midi_active_ = false;
    }
}

void MidiHandler::HandleNoteOn(Corrode& synth, uint8_t note, uint8_t velocity)
{
    if (velocity == 0)
    {
        HandleNoteOff(synth, note);
        return;
    }

    bool accent = (velocity >= kAccentVelocity);

    // TODO: Slide detection (legato playing / overlapping notes)
    bool slide = false;

    synth.NoteOn(static_cast<int>(note), accent, slide);
}

void MidiHandler::HandleNoteOff(Corrode& synth, uint8_t note)
{
    synth.NoteOff();
}

void MidiHandler::HandleCC(Corrode& synth, uint8_t cc, uint8_t value)
{
    ParamId param = FindParamByCC(cc);
    if (param == ParamId::COUNT)
        return;  // Unknown CC

    // Normalize CC value (0..127) to 0..1
    float normalized = static_cast<float>(value) / 127.0f;
    synth.SetParameter(param, normalized);
}

ParamId MidiHandler::FindParamByCC(uint8_t cc) const
{
    for (int i = 0; i < static_cast<int>(ParamId::COUNT); i++)
    {
        if (kParamDefs[i].midi_cc == cc)
            return static_cast<ParamId>(i);
    }
    return ParamId::COUNT;
}

} // namespace corrode
