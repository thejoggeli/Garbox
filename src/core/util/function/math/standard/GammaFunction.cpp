#include "GammaFunction.h"

#include "core/assert/Assert.h"
#include <cmath>

namespace Garbox {

GammaFunction::GammaFunction(float gamma) : mGamma(gamma){
    // nothing to do
}

void GammaFunction::setGamma(float gamma){
    if(gamma <= 0.0f){
        TriggerDebug("GammaFunction", "invalid gamma value");
    }
    mGamma = gamma;
}

float GammaFunction::evaluate(float x) const {
    if(x <= 0.0f){
        return 0.0f;
    }
    return powf(x, mGamma);
}

float GammaFunction::getGamma() const {
    return mGamma;
}

} // namespace Garbox
