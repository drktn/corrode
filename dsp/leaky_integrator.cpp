#include "leaky_integrator.h"
#include "../src/util.h"

namespace corrode {
namespace dsp {

void LeakyIntegrator::SetTimeConstant(float ms)
{
    float samples = ms * sample_rate_ / 1000.0f;
    coeff_ = (samples > 1.0f) ? fast_math::exp_approx(-1.0f / samples) : 0.0f;
}

} // namespace dsp
} // namespace corrode
