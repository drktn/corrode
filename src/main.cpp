#include "daisy_patch_sm.h"
#include "corrode.h"
#include "controls.h"
#include "midi_handler.h"

using namespace daisy;
using namespace patch_sm;

// Hardware
DaisyPatchSM hw;

// Engine
corrode::Corrode synth;
corrode::Controls controls;
corrode::MidiHandler midi;

// Audio callback: called once per block (48 samples at 48 kHz)
void AudioCallback(AudioHandle::InputBuffer in,
                   AudioHandle::OutputBuffer out,
                   size_t size)
{
    // Read hardware controls and update parameters
    controls.Process(hw, synth);

    // Process MIDI messages
    midi.Process(hw, synth);

    for (size_t i = 0; i < size; i++)
    {
        // Check for external audio input (replaces oscillator if present)
        float external_in = in[0][i];

        // Process one sample through the 303 engine
        float sample = synth.Process(external_in);

        // Output mono to both channels
        out[0][i] = sample;
        out[1][i] = sample;
    }

    // Write envelope follower to CV output
    hw.WriteCvOut(CV_OUT_1, synth.GetEnvelopeValue() * 5.0f);

    // Write gate outputs
    dsy_gpio_write(&hw.gate_out_1, synth.GetAccentState());
    dsy_gpio_write(&hw.gate_out_2, synth.GetGateState());
}

int main(void)
{
    // Initialize hardware
    hw.Init();
    hw.SetAudioBlockSize(48);
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

    // Initialize engine
    float sample_rate = hw.AudioSampleRate();
    synth.Init(sample_rate);
    controls.Init();
    midi.Init(hw);

    // Start audio
    hw.StartAudio(AudioCallback);

    // Main loop: handle non-audio tasks
    while (true)
    {
        // LED mode indicator
        controls.UpdateLed(hw);

        // Low-priority background tasks
        System::Delay(1);
    }
}
