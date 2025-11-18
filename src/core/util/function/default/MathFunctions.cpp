#include "MathFunctions.h"

#include <math.h>
#include "core/util/function/math/MathFunctionHelpers.h"
#include "core/util/math/MathConstants.h"

namespace Garbox {

using namespace MathFunctionHelpers;

namespace Samples {
    static constexpr uint32_t Gamma = 64;
    static constexpr uint32_t GammaInverse = 64;
    static constexpr uint32_t Sqrt = 64;
    static constexpr uint32_t Cbrt = 64;
    static constexpr uint32_t Sine = 64;
};

const MathFunctionIfc& MathFunctions::GetConstantZero(){
    static ConstantFunction instance(0.0f);
    return instance;
}

const MathFunctionIfc& MathFunctions::GetConstantOne(){
    static ConstantFunction instance(1.0f);
    return instance;
}

const MathFunctionIfc& MathFunctions::GetLinear(){
    return GetAnalytic([](float x){ return x; });
}

const MathFunctionIfc& MathFunctions::GetQuadratic(){
    return GetAnalytic([](float x){ return x*x; });
}

const MathFunctionIfc& MathFunctions::GetSin(){
    return GetSampled([](float x){ return sinf(x); }, Samples::Sine);
}

const MathFunctionIfc& MathFunctions::GetSin90(){
    return GetSampled([](float x){ return sinf(x + 0.5f * MathConstants::TwoPi); }, Samples::Sine);
}

const MathFunctionIfc& MathFunctions::GetSin180(){
    return GetSampled([](float x){ return sinf(x + 1.0f * MathConstants::TwoPi); }, Samples::Sine);
}

const MathFunctionIfc& MathFunctions::GetSin270(){
    return GetSampled([](float x){ return sinf(x + 1.5f * MathConstants::TwoPi); }, Samples::Sine);
}

const MathFunctionIfc& MathFunctions::GetSinNorm(){
    return GetSampled([](float x){ return 0.5f * (sinf(x * MathConstants::TwoPi) + 1.0f); }, Samples::Sine);
}

const MathFunctionIfc& MathFunctions::GetSinNorm90(){
    return GetSampled([](float x){ return 0.5f * (sinf(x * MathConstants::TwoPi + 0.5f * MathConstants::TwoPi) + 1.0f); }, Samples::Sine);
}

const MathFunctionIfc& MathFunctions::GetSinNorm180(){
    return GetSampled([](float x){ return 0.5f * (sinf(x * MathConstants::TwoPi + 1.0f * MathConstants::TwoPi) + 1.0f); }, Samples::Sine);
}

const MathFunctionIfc& MathFunctions::GetSinNorm270(){
    return GetSampled([](float x){ return 0.5f * (sinf(x * MathConstants::TwoPi + 1.5f * MathConstants::TwoPi) + 1.0f); }, Samples::Sine);
}

const MathFunctionIfc& MathFunctions::GetSinAnim(){
    return GetSinNorm270();
}


const MathFunctionIfc& MathFunctions::GetGamma22(){
    return GetSampled([](float x){ return powf(x, 2.2f); }, Samples::Gamma);
}

const MathFunctionIfc& MathFunctions::GetGamma22Inverse(){
    return GetSampled([](float x){ return powf(x, (1.0f/2.2f)); }, Samples::GammaInverse);
}

const MathFunctionIfc& MathFunctions::GetSqrt(){
    return GetSampled([](float x){ return cbrtf(x); }, Samples::Sqrt);
}

const MathFunctionIfc& MathFunctions::GetCbrt(){
    return GetSampled([](float x){ return cbrtf(x); }, Samples::Cbrt);
}

} // namespace Garbox
