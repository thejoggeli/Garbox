// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HeatpadSampleState.h"

namespace Garbox {

HeatpadSampleState::HeatpadSampleState(): StateAbs(StateType::HeatpadSample){
    // mCurrent and mNext default-initialized
}

float HeatpadSampleState::getMeasuredVoltage() const {
    return mCurrent.measuredVoltage;
}

float HeatpadSampleState::getMeasuredCurrent() const {
    return mCurrent.measuredCurrent;
}

float HeatpadSampleState::getPwmProgressMicros() const {
    return mCurrent.pwmProgressMicros;
}

void HeatpadSampleState::setMeasuredVoltage(float value){
    if (mNext.measuredVoltage == value) {
        return;
    }
    mNext.measuredVoltage = value;
    setDirty();
}

void HeatpadSampleState::setMeasuredCurrent(float value){
    if (mNext.measuredCurrent == value) {
        return;
    }
    mNext.measuredCurrent = value;
    setDirty();
}

void HeatpadSampleState::setPwmProgressMicros(float value){
    if (mNext.pwmProgressMicros == value) {
        return;
    }
    mNext.pwmProgressMicros = value;
    setDirty();
}

void HeatpadSampleState::publish(){
    if (!isDirty()) {
        return;
    }
    mCurrent = mNext;
}

} // namespace Garbox