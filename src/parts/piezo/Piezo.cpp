#include "Piezo.h"

#include <Arduino.h>
#include "assert/Assert.h"
#include "global/mcpwm/McpwmInstances.h"

namespace Garbox {

Piezo::Piezo() :
    // init members
    mPwm(McpwmInstances::GetPiezoPair()),
    mTestTimer() {
    // constructor body
    // nothing to do
}

void Piezo::init(){
    mTestTimer.start(1000);
}

void Piezo::tick(){

    static constexpr uint32_t numStates = 9;
    static uint32_t state = 0;

    if(mTestTimer.isExpired()){
        state = (++state) % numStates;
        switch(state){
            case 0: 
                setEnabled(false);
                mTestTimer.restart(5000);
                break;
            case 1: 
                setFrequency(750);
                setDutyPercent(50.0f);
                setVolume(Volume::Low);
                setEnabled(true);
                mTestTimer.restart(200);
                break;
            case 2: 
                setEnabled(false);
                setFrequency(1000);
                setEnabled(true);
                mTestTimer.restart(200);
                break;
            case 3: 
                setEnabled(false);
                setFrequency(1500);
                setEnabled(true);
                mTestTimer.restart(200);
                break;
            case 4: 
                setEnabled(false);
                mTestTimer.restart(1000);
                break;
            case 5: 
                setFrequency(1000);
                setDutyPercent(50.0f);
                setEnabled(true);
                setVolume(Volume::Low);
                mTestTimer.restart(1000);
                break;
            case 6: 
                setVolume(Volume::High);
                mTestTimer.restart(1000);
                break;
            case 7: 
                setVolume(Volume::Low);
                mTestTimer.restart(1000);
                break;
            case 8: 
                setVolume(Volume::High);
                mTestTimer.restart(1000);
                break;
        }

    }

}

void Piezo::setFrequency(uint32_t frequency){
    mPwm.setFrequency(frequency);
}

void Piezo::setEnabled(bool enabled){
    if(mEnabled == enabled){
        return;
    }
    mEnabled = enabled;
    updatePwmEnable();
}

void Piezo::setDutyPercent(float duty){
    mPwm.setDutyPercent(duty);
}

void Piezo::setVolume(Volume volume){
    if(mVolume == volume){
        return;
    }
    mVolume = volume;
    updatePwmEnable();
}

void Piezo::updatePwmEnable(){
    if(mEnabled){
        switch(mVolume){
            case Volume::Low:
                mPwm.setEnabledA(true);
                mPwm.setEnabledB(false);
                break;
            case Volume::High:
                mPwm.setEnabledA(true);
                mPwm.setEnabledB(true);
                break;
            default:
                AssertDebug(false, "Unhandled Piezo volume");
                break;
        }
    } 
    else {
        mPwm.setEnabled(false);
    }
}

} // namespace
