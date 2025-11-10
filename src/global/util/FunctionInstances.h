#pragma once

#include "util/transform/function/FunctionIfc.h"

namespace Garbox {

class FunctionInstances {
public:

    static const FunctionIfc& GetConstantZero();
    static const FunctionIfc& GetConstantOne();
    static const FunctionIfc& GetLinear();
    static const FunctionIfc& GetQuadratic();

    static const FunctionIfc& GetRectangleStep();
    static const FunctionIfc& GetRectanglePulse();

    static const FunctionIfc& GetTrianglePulse();

    static const FunctionIfc& GetGamma22Sampled();

    static const FunctionIfc& GetSinSampled();
    static const FunctionIfc& GetSinSampledNorm();

    static const FunctionIfc& GetCosSampled();
    static const FunctionIfc& GetCosSampledNorm();
    static const FunctionIfc& GetCosSampledNormNeg();

    static const FunctionIfc& GetEaseLinearAnalytic();
    static const FunctionIfc& GetEaseInQuadAnalytic();
    static const FunctionIfc& GetEaseOutQuadAnalytic();
    static const FunctionIfc& GetEaseInOutQuadAnalytic();

    static const FunctionIfc& GetEaseInSineSampled();
    static const FunctionIfc& GetEaseOutSineSampled();
    static const FunctionIfc& GetEaseInOutSineSampled();

    static const FunctionIfc& GetEaseInElasticLightSampled();
    static const FunctionIfc& GetEaseInElasticMediumSampled();
    static const FunctionIfc& GetEaseInElasticStrongSampled();

    static const FunctionIfc& GetEaseOutElasticLightSampled();
    static const FunctionIfc& GetEaseOutElasticMediumSampled();
    static const FunctionIfc& GetEaseOutElasticStrongSampled();

    static const FunctionIfc& GetEaseInOutElasticLightSampled();
    static const FunctionIfc& GetEaseInOutElasticMediumSampled();
    static const FunctionIfc& GetEaseInOutElasticStrongSampled();

};

} // namespace Garbox
