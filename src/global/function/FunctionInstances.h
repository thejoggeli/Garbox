#pragma once

#include "util/function/DirectFunction.h"
#include "util/function/SampledFunction.h"

namespace Garbox {

class FunctionInstances {
public:

    static const SampledFunction& GetGamma22Sampled();

    static const SampledFunction& GetSinSampled();
    static const SampledFunction& GetSinSampledNorm();

    static const SampledFunction& GetCosSampled();
    static const SampledFunction& GetCosSampledNorm();

    static const DirectFunction& GetEaseLinearDirect();
    static const DirectFunction& GetEaseInQuadDirect();
    static const DirectFunction& GetEaseOutQuadDirect();
    static const DirectFunction& GetEaseInOutQuadDirect();

    static const SampledFunction& GetEaseInSineSampled();
    static const SampledFunction& GetEaseOutSineSampled();
    static const SampledFunction& GetEaseInOutSineSampled();

    static const SampledFunction& GetEaseInElasticLightSampled();
    static const SampledFunction& GetEaseInElasticMediumSampled();
    static const SampledFunction& GetEaseInElasticStrongSampled();

    static const SampledFunction& GetEaseOutElasticLightSampled();
    static const SampledFunction& GetEaseOutElasticMediumSampled();
    static const SampledFunction& GetEaseOutElasticStrongSampled();

    static const SampledFunction& GetEaseInOutElasticLightSampled();
    static const SampledFunction& GetEaseInOutElasticMediumSampled();
    static const SampledFunction& GetEaseInOutElasticStrongSampled();

};

} // namespace Garbox
