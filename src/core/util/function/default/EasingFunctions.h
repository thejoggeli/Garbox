#pragma once

#include "core/util/function/math/MathFunctionIfc.h"

namespace Garbox {

class EasingFunctions {
public:

    // all analytic
    static const MathFunctionIfc& GetLinear();      
    static const MathFunctionIfc& GetInQuad();      
    static const MathFunctionIfc& GetOutQuad();     
    static const MathFunctionIfc& GetInOutQuad();   

    // all sampled
    static const MathFunctionIfc& GetInSine();      
    static const MathFunctionIfc& GetOutSine();     
    static const MathFunctionIfc& GetInOutSine();   

    // all sampled
    static const MathFunctionIfc& GetInElasticLight();
    static const MathFunctionIfc& GetInElasticMedium();
    static const MathFunctionIfc& GetInElasticStrong();

    // all sampled
    static const MathFunctionIfc& GetOutElasticLight();
    static const MathFunctionIfc& GetOutElasticMedium();
    static const MathFunctionIfc& GetOutElasticStrong();

    // all sampled
    static const MathFunctionIfc& GetInOutElasticLight();
    static const MathFunctionIfc& GetInOutElasticMedium();
    static const MathFunctionIfc& GetInOutElasticStrong();

};

} // namespace Garbox
