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

void Piezo::tick(){

    static constexpr uint32_t numStates = 6;
    static uint32_t state = 0;

    if(mTestTimer.isExpired()){
        state = (state+1) % numStates;
        switch(state){
            case 0: 
                setEnabled(false);
                mTestTimer.restart(5000_ms);
                break;
            case 1: 
                setFrequency(750);
                setDuty(0.5f);
                setEnabled(true);
                mTestTimer.restart(200_ms);
                break;
            case 2: 
                setFrequency(1000);
                mTestTimer.restart(200_ms);
                break;
            case 3: 
                setFrequency(1500);
                mTestTimer.restart(200_ms);
                break;
            case 4: 
                setFrequency(1000);
                mTestTimer.restart(400_ms);
                break;
            case 5: 
                setEnabled(false);
                mTestTimer.reset();
                break;
        }

    }

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

} // namespace
