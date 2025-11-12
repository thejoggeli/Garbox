#include "WaveformFunctions.h"

#include "util/function/math/MathFunctionHelpers.h"

namespace Garbox {

using namespace MathFunctionHelpers;

const MathFunctionIfc& WaveformFunctions::GetRectangleStep(){
    static LookupFunction::Point points[] = {
        {0.0f, 0.0f},
        {0.5f, 1.0f},
    };
    return GetLookup(points, sizeof(points)/sizeof(LookupFunction::Point), LookupFunction::Mode::Hold);
}

const MathFunctionIfc& WaveformFunctions::GetRectanglePulse(){
    static LookupFunction::Point points[] = {
        {0.0f, 0.0f},
        {0.25f, 1.0f},
        {0.75f, 0.0f},
    };
    return GetLookup(points, sizeof(points)/sizeof(LookupFunction::Point), LookupFunction::Mode::Hold);
}

const MathFunctionIfc& WaveformFunctions::GetTrianglePulse(){
    static LookupFunction::Point points[] = {
        {0.0f, 0.0f},
        {0.5f, 1.0f},
        {1.0f, 0.0f},
    };
    return GetLookup(points, sizeof(points)/sizeof(LookupFunction::Point), LookupFunction::Mode::Linear);
}

} // namespace Garbox
