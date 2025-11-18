#include "Piezo.h"

#include "core/hardware/ledc/LedcChannel.h"
#include "core/hardware/ledc/LedcTimer.h"

namespace Garbox {

Piezo::Piezo(LedcTimer& pwmTimer, LedcChannel& pwmChannel) :
    // init members
    mPwmTimer(pwmTimer),
    mPwmChannel(pwmChannel),
    mTestTimer(){
    // constructor body
}

void Piezo::init(){
    mTestTimer.start(0);
}

void Piezo::setFrequency(uint32_t frequency){
    if(mFrequency == frequency){
        return;
    }
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
    if(mDuty == duty){
        return;
    }
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
