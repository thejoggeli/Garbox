#include "FunctionInstances.h"

#include <math.h>
#include "assert/Assert.h"
#include "util/transform/function/base/AnalyticFunction.h"
#include "util/transform/function/base/ConstantFunction.h"
#include "util/transform/function/base/LookupFunction.h"
#include "util/transform/function/base/SampledFunction.h"
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
static const FunctionIfc& GetAnalytic(Func func){
    static AnalyticFunction instance;
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

static const FunctionIfc& GetLookup(LookupFunction::Point* points, size_t numPoints, LookupFunction::Mode mode){
    static LookupFunction instance;
    static bool initialized = false;
    if(!initialized){
        instance.init(points, false, numPoints, mode);
        initialized = true;
    }
    return instance;
}

const FunctionIfc& FunctionInstances::GetConstantZero(){
    static ConstantFunction instance(0.0f);
    return instance;
}

const FunctionIfc& FunctionInstances::GetConstantOne(){
    static ConstantFunction instance(1.0f);
    return instance;
}

const FunctionIfc& FunctionInstances::GetLinear(){
    return GetAnalytic([](float x){ return x; });
}

const FunctionIfc& FunctionInstances::GetQuadratic(){
    return GetAnalytic([](float x){ return x*x; });
}

const FunctionIfc& FunctionInstances::GetRectangleStep(){
    static LookupFunction::Point points[] = {
        {0.0f, 0.0f},
        {0.5f, 1.0f},
    };
    return GetLookup(points, sizeof(points)/sizeof(LookupFunction::Point), LookupFunction::Mode::Hold);
}

const FunctionIfc& FunctionInstances::GetRectanglePulse(){
    static LookupFunction::Point points[] = {
        {0.0f, 0.0f},
        {0.25f, 1.0f},
        {0.75f, 0.0f},
    };
    return GetLookup(points, sizeof(points)/sizeof(LookupFunction::Point), LookupFunction::Mode::Hold);
}

const FunctionIfc& FunctionInstances::GetTrianglePulse(){
    static LookupFunction::Point points[] = {
        {0.0f, 0.0f},
        {0.5f, 1.0f},
        {1.0f, 0.0f},
    };
    return GetLookup(points, sizeof(points)/sizeof(LookupFunction::Point), LookupFunction::Mode::Linear);
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

const FunctionIfc& FunctionInstances::GetEaseLinearAnalytic(){
    return GetAnalytic(EasingFunctions::EaseLinear);
}

const FunctionIfc& FunctionInstances::GetEaseInQuadAnalytic(){
    return GetAnalytic(EasingFunctions::EaseInQuad);
}

const FunctionIfc& FunctionInstances::GetEaseOutQuadAnalytic(){
    return GetAnalytic(EasingFunctions::EaseOutQuad);
}

const FunctionIfc& FunctionInstances::GetEaseInOutQuadAnalytic(){
    return GetAnalytic(EasingFunctions::EaseInOutQuad);
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
