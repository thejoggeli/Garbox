#include "ScaleFunction.h"

namespace Garbox {

ScaleFunction::ScaleFunction(){
    // nothing to do
}

ScaleFunction::ScaleFunction(float scale) : mScale(scale){
    // constructor body
}

void ScaleFunction::setScale(float scale){
    mScale = scale;
}

float ScaleFunction::getScale() const{
    return mScale;
}

float ScaleFunction::evaluate(float x) const{
    return x * mScale;
}

} // namespace Garbox
