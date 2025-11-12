#include "GammaFunctions.h"

#include <math.h>
#include "util/function/math/MathFunctionHelpers.h"
#include "util/math/MathConstants.h"

namespace Garbox {

using namespace MathFunctionHelpers;

namespace Samples {
    static constexpr uint32_t Gamma = 64;
    static constexpr uint32_t GammaInverse = 64;
};

const MathFunctionIfc& GammaFunctions::GetGamma22(){
    return GetSampled([](float x){ return powf(x, 2.2f); }, Samples::Gamma);
}

const MathFunctionIfc& GammaFunctions::GetGammaInverse22(){
    return GetSampled([](float x){ return powf(x, (1.0f/2.2f)); }, Samples::GammaInverse);
}

} // namespace Garbox
