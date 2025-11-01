#include "Piezo.h"

#include "global/ledc/LedcInstances.h"

namespace Garbox {

Piezo::Piezo() :
    // init members
    mPwmTimer(LedcInstances::GetPiezoTimer()),
    mPwmChannel(LedcInstances::GetPiezoChannel()),
    mTestTimer() {
    // constructor body
    // nothing to do
}

void Piezo::init(){
    mTestTimer.start(0);
}

void Piezo::setFrequency(uint32_t frequency){
    mFrequency = frequency;
    mPwmTimer.setFrequency(frequency);
}

void Piezo::setEnabled(bool enabled){
    if(mEnabled == enabled){
        return;
    }
    mEnabled = enabled;
    if(mEnabled){
        mPwmChannel.setDutyRelative(mDuty);
    } 
    else {
        mPwmChannel.setDutyRaw(0);
    }
}

void Piezo::setDuty(float duty){
    mDuty = duty;
    if(mEnabled){
        mPwmChannel.setDutyRelative(mDuty);
    }
}

uint32_t Piezo::getFrequency(){
    return mFrequency;
}

bool Piezo::isEnabled(){
    return mEnabled;
}

float Piezo::getDuty(){
    return mDuty;
}

} // namespace
