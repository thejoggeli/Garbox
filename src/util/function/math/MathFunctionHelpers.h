#pragma once

#include <cstring>
#include "util/function/math/custom/AnalyticFunction.h"
#include "util/function/math/custom/ConstantFunction.h"
#include "util/function/math/custom/LookupFunction.h"
#include "util/function/math/custom/SampledFunction.h"

namespace Garbox::MathFunctionHelpers {

template<typename Func>
inline static const MathFunctionIfc& GetAnalytic(Func func){
    static AnalyticFunction instance;
    static bool initialized = false;
    if(!initialized){
        instance.init(func);
        initialized = true;
    }
    return instance;
}

template<typename Func>
inline static const MathFunctionIfc& GetSampled(Func func, uint32_t sampleCount, float xMin = 0.0f, float xMax = 1.0f){
    static SampledFunction instance;
    static bool initialized = false;
    if(!initialized){
        instance.init(func, sampleCount, xMin, xMax);
        initialized = true;
    }
    return instance;
}

inline static const MathFunctionIfc& GetLookup(LookupFunction::Point* points, size_t numPoints, LookupFunction::Mode mode){
    static LookupFunction instance;
    static bool initialized = false;
    if(!initialized){
        instance.init(points, false, numPoints, mode);
        initialized = true;
    }
    return instance;
}

} // namespace