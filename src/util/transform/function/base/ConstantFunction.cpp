#include "ConstantFunction.h"

#include "assert/Assert.h"

namespace Garbox {

ConstantFunction::ConstantFunction(float value){
    // nothing to do
}

float ConstantFunction::evaluate(float x) const {
    return mValue;
}

} // namespace Garbox
