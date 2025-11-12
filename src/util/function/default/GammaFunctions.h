#pragma once

#include "util/function/math/MathFunctionIfc.h"

namespace Garbox {

class GammaFunctions {
public:
    
    // sampled
    static const MathFunctionIfc& GetGamma22();
    static const MathFunctionIfc& GetGammaInverse22();

};

} // namespace Garbox
