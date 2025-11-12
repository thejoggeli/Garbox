#pragma once

#include "util/function/math/MathFunctionIfc.h"

namespace Garbox {

class ColorFunctions {
public:
    
    // sampled in range [-0.1, 2.0] for safe use in Lab conversion
    static const MathFunctionIfc& GetLabCbrt();

};

} // namespace Garbox
