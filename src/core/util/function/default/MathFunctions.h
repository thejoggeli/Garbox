#pragma once

#include "core/util/function/math/MathFunctionIfc.h"

namespace Garbox {

class MathFunctions {
public:

    // constant
    static const MathFunctionIfc& GetConstantZero();
    static const MathFunctionIfc& GetConstantOne();

    // analytic
    static const MathFunctionIfc& GetLinear();
    static const MathFunctionIfc& GetQuadratic();

    // sampled
    static const MathFunctionIfc& GetSin();     // x [0, 2pi] and y [-1, 1] => phase shifted by   0 degrees =>  sin(x) => starts at  0.0
    static const MathFunctionIfc& GetSin90();   // x [0, 2pi] and y [-1, 1] => phase shifted by  90 degrees =>  cos(x) => starts at +1.0
    static const MathFunctionIfc& GetSin180();  // x [0, 2pi] and y [-1, 1] => phase shifted by 180 degrees => -sin(x) => starts at  0.0
    static const MathFunctionIfc& GetSin270();  // x [0, 2pi] and y [-1, 1] => phase shifted by 180 degrees => -cos(x) => starts at -1.0

    // sampled
    static const MathFunctionIfc& GetSinNorm();     // x [0, 1] and y [0, 1] => phase shifted by   0 degrees =>  sin(x) => starts at +0.5
    static const MathFunctionIfc& GetSinNorm90();   // x [0, 1] and y [0, 1] => phase shifted by  90 degrees =>  cos(x) => starts at +1.0
    static const MathFunctionIfc& GetSinNorm180();  // x [0, 1] and y [0, 1] => phase shifted by 180 degrees => -sin(x) => starts at +0.5
    static const MathFunctionIfc& GetSinNorm270();  // x [0, 1] and y [0, 1] => phase shifted by 180 degrees => -cos(x) => starts at +0.0

    // sampled
    static const MathFunctionIfc& GetSinAnim(); // alias for GetSinNorm270() as it's very useful for simple animations

    // sampled
    static const MathFunctionIfc& GetSqrt(); // square root
    static const MathFunctionIfc& GetCbrt(); // cube root
    
    // sampled
    static const MathFunctionIfc& GetGamma22();
    static const MathFunctionIfc& GetGamma22Inverse();

};

} // namespace Garbox
