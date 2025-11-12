#include "EasingFunctions.h"

#include <math.h>
#include "util/function/math/MathFunctionHelpers.h"
#include "util/math/EasingEquations.h"

namespace Garbox {

using namespace MathFunctionHelpers;

static constexpr uint32_t SineSamples = 32;
static constexpr uint32_t ElasticSamples = 32;

struct ElasticParams { float amplitude; float period; };
static constexpr ElasticParams ElasticLight = {0.8f, 0.45f};
static constexpr ElasticParams ElasticMedium = {1.0f, 0.4f};
static constexpr ElasticParams ElasticStrong = {1.2f, 0.3f};

const MathFunctionIfc& EasingFunctions::GetLinear(){
    return GetAnalytic(EasingEquations::Linear);
}

const MathFunctionIfc& EasingFunctions::GetInQuad(){
    return GetAnalytic(EasingEquations::InQuad);
}

const MathFunctionIfc& EasingFunctions::GetOutQuad(){
    return GetAnalytic(EasingEquations::OutQuad);
}

const MathFunctionIfc& EasingFunctions::GetInOutQuad(){
    return GetAnalytic(EasingEquations::InOutQuad);
}

const MathFunctionIfc& EasingFunctions::GetInSine(){
    return GetSampled(EasingEquations::InSine, SineSamples);
}

const MathFunctionIfc& EasingFunctions::GetOutSine(){
    return GetSampled(EasingEquations::OutSine, SineSamples);
}

const MathFunctionIfc& EasingFunctions::GetInOutSine(){
    return GetSampled(EasingEquations::InOutSine, SineSamples);
}

const MathFunctionIfc& EasingFunctions::GetInElasticLight(){
    return GetSampled([](float t){ return EasingEquations::InElastic(t, ElasticLight.amplitude, ElasticLight.period); }, ElasticSamples);
}

const MathFunctionIfc& EasingFunctions::GetInElasticMedium(){
    return GetSampled([](float t){ return EasingEquations::InElastic(t, ElasticMedium.amplitude, ElasticMedium.period); }, ElasticSamples);
}

const MathFunctionIfc& EasingFunctions::GetInElasticStrong(){
    return GetSampled([](float t){ return EasingEquations::InElastic(t, ElasticStrong.amplitude, ElasticStrong.period); }, ElasticSamples);
}

const MathFunctionIfc& EasingFunctions::GetOutElasticLight(){
    return GetSampled([](float t){ return EasingEquations::OutElastic(t, ElasticLight.amplitude, ElasticLight.period); }, ElasticSamples);
}

const MathFunctionIfc& EasingFunctions::GetOutElasticMedium(){
    return GetSampled([](float t){ return EasingEquations::OutElastic(t, ElasticMedium.amplitude, ElasticMedium.period); }, ElasticSamples);
}

const MathFunctionIfc& EasingFunctions::GetOutElasticStrong(){
    return GetSampled([](float t){ return EasingEquations::OutElastic(t, ElasticStrong.amplitude, ElasticStrong.period); }, ElasticSamples);
}

const MathFunctionIfc& EasingFunctions::GetInOutElasticLight(){
    return GetSampled([](float t){ return EasingEquations::InOutElastic(t, ElasticLight.amplitude, ElasticLight.period); }, ElasticSamples);
}

const MathFunctionIfc& EasingFunctions::GetInOutElasticMedium(){
    return GetSampled([](float t){ return EasingEquations::InOutElastic(t, ElasticMedium.amplitude, ElasticMedium.period); }, ElasticSamples);
}

const MathFunctionIfc& EasingFunctions::GetInOutElasticStrong(){
    return GetSampled([](float t){ return EasingEquations::InOutElastic(t, ElasticStrong.amplitude, ElasticStrong.period); }, ElasticSamples);
}

} // namespace Garbox
