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
static const DirectFunction& GetDirect(Func func){
    static DirectFunction instance;
    return instance;
}

template<typename Func>
static const SampledFunction& GetSampled(Func func, uint32_t sampleCount){
    static SampledFunction instance;
    static bool initialized = false;
    if(!initialized){
        instance.init(func, sampleCount, 0.0f, 1.0f);
        initialized = true;
    }
    return instance;
}

const SampledFunction& FunctionInstances::GetGamma22Sampled(){
    return GetSampled([](float x){ return powf(x, 2.2f); }, GammaSamples);
}

const SampledFunction& FunctionInstances::GetSinSampled(){
    return GetSampled([](float x){ return sinf(x * MathConstants::TwoPi); }, SineSamples);
}

const SampledFunction& FunctionInstances::GetSinSampledNorm(){
    return GetSampled([](float x){ return 0.5f * (sinf(x * MathConstants::TwoPi) + 1.0f); }, SineSamples);
}

const SampledFunction& FunctionInstances::GetCosSampled(){
    return GetSampled([](float x){ return cosf(x * MathConstants::TwoPi); }, SineSamples);
}

const SampledFunction& FunctionInstances::GetCosSampledNorm(){
    return GetSampled([](float x){ return 0.5f * cosf(x * MathConstants::TwoPi) + 0.5f; }, SineSamples);
}

const SampledFunction& FunctionInstances::GetCosSampledNormNeg(){
    return GetSampled([](float x){ return -0.5f * cosf(x * MathConstants::TwoPi) + 0.5f; }, SineSamples);
}

const DirectFunction& FunctionInstances::GetEaseLinearDirect(){
    return GetDirect(EasingFunctions::EaseLinear);
}

const DirectFunction& FunctionInstances::GetEaseInQuadDirect(){
    return GetDirect(EasingFunctions::EaseInQuad);
}

const DirectFunction& FunctionInstances::GetEaseOutQuadDirect(){
    return GetDirect(EasingFunctions::EaseOutQuad);
}

const DirectFunction& FunctionInstances::GetEaseInOutQuadDirect(){
    return GetDirect(EasingFunctions::EaseInOutQuad);
}

const SampledFunction& FunctionInstances::GetEaseInSineSampled(){
    return GetSampled(EasingFunctions::EaseInSine, EaseSineSamples);
}

const SampledFunction& FunctionInstances::GetEaseOutSineSampled(){
    return GetSampled(EasingFunctions::EaseOutSine, EaseSineSamples);
}

const SampledFunction& FunctionInstances::GetEaseInOutSineSampled(){
    return GetSampled(EasingFunctions::EaseInOutSine, EaseSineSamples);
}

const SampledFunction& FunctionInstances::GetEaseInElasticLightSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseInElastic(t, ElasticLight.amplitude, ElasticLight.period); }, EaseElasticSamples);
}

const SampledFunction& FunctionInstances::GetEaseInElasticMediumSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseInElastic(t, ElasticMedium.amplitude, ElasticMedium.period); }, EaseElasticSamples);
}

const SampledFunction& FunctionInstances::GetEaseInElasticStrongSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseInElastic(t, ElasticStrong.amplitude, ElasticStrong.period); }, EaseElasticSamples);
}

const SampledFunction& FunctionInstances::GetEaseOutElasticLightSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseOutElastic(t, ElasticLight.amplitude, ElasticLight.period); }, EaseElasticSamples);
}

const SampledFunction& FunctionInstances::GetEaseOutElasticMediumSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseOutElastic(t, ElasticMedium.amplitude, ElasticMedium.period); }, EaseElasticSamples);
}

const SampledFunction& FunctionInstances::GetEaseOutElasticStrongSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseOutElastic(t, ElasticStrong.amplitude, ElasticStrong.period); }, EaseElasticSamples);
}

const SampledFunction& FunctionInstances::GetEaseInOutElasticLightSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseInOutElastic(t, ElasticLight.amplitude, ElasticLight.period); }, EaseElasticSamples);
}

const SampledFunction& FunctionInstances::GetEaseInOutElasticMediumSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseInOutElastic(t, ElasticMedium.amplitude, ElasticMedium.period); }, EaseElasticSamples);
}

const SampledFunction& FunctionInstances::GetEaseInOutElasticStrongSampled(){
    return GetSampled([](float t){ return EasingFunctions::EaseInOutElastic(t, ElasticStrong.amplitude, ElasticStrong.period); }, EaseElasticSamples);
}

} // namespace Garbox
