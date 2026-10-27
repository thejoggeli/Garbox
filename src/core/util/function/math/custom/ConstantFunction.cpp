#include "ConstantFunction.h"

#include "core/assert/Assert.h"

namespace Garbox {

ConstantFunction::ConstantFunction(float value) : mValue(value){}

float ConstantFunction::evaluate(float x) const {
    return mValue;
}

} // namespace Garbox
