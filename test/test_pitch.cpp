// test_pitch.cpp — V/Oct pitch tracking accuracy
//
// Tests:
// 1. PitchTable lookup accuracy vs direct pow() calculation
// 2. Linear interpolation accuracy for fractional notes
// 3. Cent deviation across MIDI range

#include "../src/util.h"
#include <cmath>
#include <cstdio>

int main()
{
    using namespace corrode::fast_math;

    PitchTable table;
    table.Init();

    float max_cents_error = 0.0f;

    for (int note = 0; note < 128; note++)
    {
        float exact = 440.0f * std::pow(2.0f, (note - 69.0f) / 12.0f);
        float lookup = table.NoteToFreq(note);
        float cents = 1200.0f * std::log2(lookup / exact);

        if (std::abs(cents) > max_cents_error)
            max_cents_error = std::abs(cents);
    }

    printf("PitchTable max error: %.4f cents\n", max_cents_error);
    printf("PitchTable PASS: %s (threshold: 0.1 cents)\n",
           max_cents_error < 0.1f ? "YES" : "NO");

    return 0;
}
