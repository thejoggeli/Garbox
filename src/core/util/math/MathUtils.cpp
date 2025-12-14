#include "MathUtils.h"

namespace Garbox {

float MathUtils::Normalize(float val, float min, float max){
    if (max == min) return 0.0f; // avoid div by zero
    return (val - min) / (max - min);
}

}  // namespace Garbox