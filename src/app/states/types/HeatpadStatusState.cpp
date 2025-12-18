// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HeatpadStatusState.h"

namespace Garbox {

HeatpadStatusState::HeatpadStatusState(): StateAbs(StateType::HeatpadStatus){
    // mCurrent and mNext default-initialized
}

HeatpadState HeatpadStatusState::getState() const {
    return mCurrent.state;
}

float HeatpadStatusState::getCurrentDutyCycle() const {
    return mCurrent.currentDutyCycle;
}

uint32_t HeatpadStatusState::getCurrentPeriodMicros() const {
    return mCurrent.currentPeriodMicros;
}

float HeatpadStatusState::getNextDutyCycle() const {
    return mCurrent.nextDutyCycle;
}

uint32_t HeatpadStatusState::getNextPeriodMicros() const {
    return mCurrent.nextPeriodMicros;
}

void HeatpadStatusState::setState(HeatpadState value){
    if (mNext.state == value) {
        return;
    }
    mNext.state = value;
    setDirty();
}

void HeatpadStatusState::setCurrentDutyCycle(float value){
    if (mNext.currentDutyCycle == value) {
        return;
    }
    mNext.currentDutyCycle = value;
    setDirty();
}

void HeatpadStatusState::setCurrentPeriodMicros(uint32_t value){
    if (mNext.currentPeriodMicros == value) {
        return;
    }
    mNext.currentPeriodMicros = value;
    setDirty();
}

void HeatpadStatusState::setNextDutyCycle(float value){
    if (mNext.nextDutyCycle == value) {
        return;
    }
    mNext.nextDutyCycle = value;
    setDirty();
}

void HeatpadStatusState::setNextPeriodMicros(uint32_t value){
    if (mNext.nextPeriodMicros == value) {
        return;
    }
    mNext.nextPeriodMicros = value;
    setDirty();
}

void HeatpadStatusState::publish(){
    if (!isDirty()) {
        return;
    }
    mCurrent = mNext;
}

} // namespace Garbox