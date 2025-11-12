#include "ColorFunctions.h"

#include <math.h>
#include "util/function/math/MathFunctionHelpers.h"

namespace Garbox {

using namespace MathFunctionHelpers;

static constexpr uint32_t LabCbrtSamples = 64;
static constexpr float LabCbrtMinX = -0.1f;
static constexpr float LabCbrtMaxX = 2.0f;

const MathFunctionIfc& ColorFunctions::GetLabCbrt(){
    return MathFunctionHelpers::GetSampled([](float x){ return cbrtf(x); }, LabCbrtSamples, LabCbrtMinX, LabCbrtMaxX);
}


} // namespace Garbox
