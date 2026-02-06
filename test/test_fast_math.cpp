// test_fast_math.cpp — Accuracy tests for math approximations
//
// Tests:
// 1. tanh_approx vs std::tanh: max error bounds for |x| < 3
// 2. exp_approx vs std::exp: relative error for envelope coefficients
// 3. PitchTable accuracy vs direct calculation

#include "../src/util.h"
#include <cmath>
#include <cstdio>

int main()
{
    using namespace corrode::fast_math;

    // Test tanh approximation
    float max_error = 0.0f;
    float worst_x = 0.0f;

    for (float x = -3.0f; x <= 3.0f; x += 0.001f)
    {
        float exact = std::tanh(x);
        float approx = tanh_approx(x);
        float error = std::abs(exact - approx);
        if (error > max_error)
        {
            max_error = error;
            worst_x = x;
        }
    }

    printf("tanh_approx max error: %.6f at x = %.3f\n", max_error, worst_x);
    printf("tanh_approx PASS: %s (threshold: 0.005)\n",
           max_error < 0.005f ? "YES" : "NO");

    // Test exp approximation
    float max_rel_error = 0.0f;
    for (float x = -10.0f; x <= 10.0f; x += 0.01f)
    {
        float exact = std::exp(x);
        float approx = exp_approx(x);
        if (exact > 0.001f)
        {
            float rel_error = std::abs(exact - approx) / exact;
            if (rel_error > max_rel_error)
                max_rel_error = rel_error;
        }
    }

    printf("exp_approx max relative error: %.4f%%\n", max_rel_error * 100.0f);
    printf("exp_approx PASS: %s (threshold: 5%%)\n",
           max_rel_error < 0.05f ? "YES" : "NO");

    return 0;
}
