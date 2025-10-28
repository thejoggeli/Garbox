#include "Piezo.h"

#include <Arduino.h>
#include "config/PinConfig.h"
#include "config/LedcPwmConfig.h"

namespace Garbox {

Piezo::Piezo() :
    // init members
    mPwm(PinConfig::Piezo, LedcPwmConfig::Piezo, 5000, mResolutionBits),
    mTestTimer() {
    // constructor body
    // nothing to do
}

void Piezo::init(){
    mPwm.init();
    mPwm.setDutyNormalized(0.5f);
    mTestTimer.start(2000);
}

void Piezo::tick(){

    return;

    static constexpr uint32_t numStates = 7;
    static uint32_t state = 0;

    if(mTestTimer.isExpired()){
        mTestTimer.restart();
        state = (++state) % numStates;
        switch(state){
            case 0: 
                setFrequency(1000);
                setDuty(0.5f);
                setEnabled(false);
                break;
            case 1: 
                setFrequency(750);
                setDuty(0.5f);
                setEnabled(true);
                break;
            case 2: 
                setFrequency(1000);
                setDuty(0.5f);
                setEnabled(true);
                break;
            case 3: 
                setFrequency(1500);
                setDuty(0.5f);
                setEnabled(true);
                break;
            case 4: 
                setFrequency(1000);
                setDuty(0.25f);
                setEnabled(true);
                break;
            case 5: 
                setFrequency(1000);
                setDuty(0.5f);
                setEnabled(true);
                break;
            case 6: 
                setFrequency(1000);
                setDuty(0.75f);
                setEnabled(true);
                break;
        }

    }

}

void Piezo::setFrequency(uint32_t frequency){
    mFrequency = frequency;
    mPwm.setFrequency(mFrequency, mResolutionBits);
}

void Piezo::setEnabled(bool enabled){
    if(mEnabled == enabled){
        return;
    }
    mEnabled = enabled;
    if(mEnabled){
        mPwm.setDutyNormalized(mDuty);
    } 
    else {
        mPwm.setDutyRaw(0);
    }
}

void Piezo::setDuty(float duty){
    mDuty = duty;
    if(mEnabled){
        mPwm.setDutyNormalized(mDuty);
    }
}

} // namespace
