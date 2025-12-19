// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HeatpadProgressState.h"

namespace Garbox {

HeatpadProgressState::HeatpadProgressState(): StateAbs(StateType::HeatpadProgress){
    // mCurrent and mNext default-initialized
}

float HeatpadProgressState::getPwmProgressMicros() const {
    return mCurrent.pwmProgressMicros;
}

float HeatpadProgressState::nextPwmProgressMicros() const {
    return mCurrent.pwmProgressMicros;
}

float HeatpadProgressState::lastPwmProgressMicros() const {
    return mLast.pwmProgressMicros;
}

void HeatpadProgressState::setPwmProgressMicros(float value){
    if (mNext.pwmProgressMicros == value) {
        return;
    }
    mNext.pwmProgressMicros = value;
    markDirty();
}

void HeatpadProgressState::publish(){
    if (!isDirty()) {
        return;
    }
    mLast = mCurrent;
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox