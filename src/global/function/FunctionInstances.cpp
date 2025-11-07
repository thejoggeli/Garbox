#include "FunctionInstances.h"

#include <math.h>
#include "assert/Assert.h"
#include "util/math/EasingFunctions.h"
#include "util/math/MathConstants.h"

namespace Garbox {

static constexpr uint32_t GammaSamples = 32;
static constexpr uint32_t SineSamples = 32;
static constexpr uint32_t EaseSineSamples = 32;
static constexpr uint32_t EaseElasticSamples = 32;

struct ElasticParams { float amplitude; float period; };
static constexpr ElasticParams ElasticLight = {0.8f, 0.45f};
static constexpr ElasticParams ElasticMedium = {1.0f, 0.4f};
static constexpr ElasticParams ElasticStrong = {1.2f, 0.3f};

template<typename Func>
static const FunctionIfc& GetDirect(Func func){
    static DirectFunction instance;
    static bool initialized = false;
    if(!initialized){
        instance.init(func);
        initialized = true;
    }
    return instance;
}

template<typename Func>
static const FunctionIfc& GetSampled(Func func, uint32_t sampleCount){
    static SampledFunction instance;
    static bool initialized = false;
    if(!initialized){
        instance.init(func, sampleCount, 0.0f, 1.0f);
        initialized = true;
    }
    return instance;
}

const FunctionIfc& FunctionInstances::GetConstantZero(){
    return GetDirect([](float x){ return 0.0f; });
}

const FunctionIfc& FunctionInstances::GetConstantOne(){
    return GetDirect([](float x){ return 1.0f; });
}

const FunctionIfc& FunctionInstances::GetGamma22Sampled(){
    return GetSampled([](float x){ return powf(x, 2.2f); }, GammaSamples);
}

const FunctionIfc& FunctionInstances::GetSinSampled(){
    return GetSampled([](float x){ return sinf(x * MathConstants::TwoPi); }, SineSamples);
}

const FunctionIfc& FunctionInstances::GetSinSampledNorm(){
    return GetSampled([](float x){ return 0.5f * (sinf(x * MathConstants::TwoPi) + 1.0f); }, SineSamples);
}

const FunctionIfc& FunctionInstances::GetCosSampled(){
    return GetSampled([](float x){ return cosf(x * MathConstants::TwoPi); }, SineSamples);
}

const FunctionIfc& FunctionInstances::GetCosSampledNorm(){
    return GetSampled([](float x){ return 0.5f * cosf(x * MathConstants::TwoPi) + 0.5f; }, SineSamples);
}

const FunctionIfc& FunctionInstances::GetCosSampledNormNeg(){
    return GetSampled([](float x){ return -0.5f * cosf(x * MathConstants::TwoPi) + 0.5f; }, SineSamples);
}

const FunctionIfc& FunctionInstances::GetEaseLinearDirect(){
    return GetDirect(EasingFunctions::EaseLinear);
}

const FunctionIfc& FunctionInstances::GetEaseInQuadDirect(){
    return GetDirect(EasingFunctions::EaseInQuad);
}

const FunctionIfc& FunctionInstances::GetEaseOutQuadDirect(){
    return GetDirect(EasingFunctions::EaseOutQuad);
}

const FunctionIfc& FunctionInstances::GetEaseInOutQuadDirect(){
    return GetDirect(EasingFunctions::EaseInOutQuad);
}

const FunctionIfc& FunctionInstances::GetEaseInSineSampled(){
    return GetSampled(EasingFunctions::EaseInSine, EaseSineSamples);
}

const FunctionIfc& FunctionInstances::GetEaseOutSineSampled(){
    return GetSampled(EasingFunctions::EaseOutSine, EaseSineSamples);
}

const FunctionIfc& FunctionInstances::GetEaseInOutSineSampled(){
    return GetSampled(EasingFunctions::EaseInOutSine, EaseSineSamples);
}

const FunctionIfc& FunctionInstances::GetEaseInElasticLightSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseInElastic(t, ElasticLight.amplitude, ElasticLight.period); }, EaseElasticSamples);
}

const FunctionIfc& FunctionInstances::GetEaseInElasticMediumSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseInElastic(t, ElasticMedium.amplitude, ElasticMedium.period); }, EaseElasticSamples);
}

const FunctionIfc& FunctionInstances::GetEaseInElasticStrongSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseInElastic(t, ElasticStrong.amplitude, ElasticStrong.period); }, EaseElasticSamples);
}

const FunctionIfc& FunctionInstances::GetEaseOutElasticLightSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseOutElastic(t, ElasticLight.amplitude, ElasticLight.period); }, EaseElasticSamples);
}

const FunctionIfc& FunctionInstances::GetEaseOutElasticMediumSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseOutElastic(t, ElasticMedium.amplitude, ElasticMedium.period); }, EaseElasticSamples);
}

const FunctionIfc& FunctionInstances::GetEaseOutElasticStrongSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseOutElastic(t, ElasticStrong.amplitude, ElasticStrong.period); }, EaseElasticSamples);
}

const FunctionIfc& FunctionInstances::GetEaseInOutElasticLightSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseInOutElastic(t, ElasticLight.amplitude, ElasticLight.period); }, EaseElasticSamples);
}

const FunctionIfc& FunctionInstances::GetEaseInOutElasticMediumSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseInOutElastic(t, ElasticMedium.amplitude, ElasticMedium.period); }, EaseElasticSamples);
}

const FunctionIfc& FunctionInstances::GetEaseInOutElasticStrongSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseInOutElastic(t, ElasticStrong.amplitude, ElasticStrong.period); }, EaseElasticSamples);
}

} // namespace Garbox
