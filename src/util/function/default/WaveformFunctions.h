#pragma once

#include "util/function/math/MathFunctionIfc.h"

namespace Garbox {

class WaveformFunctions {
public:

    static const MathFunctionIfc& GetRectangleStep();
    static const MathFunctionIfc& GetRectanglePulse();
    static const MathFunctionIfc& GetTrianglePulse();

};

} // namespace Garbox
