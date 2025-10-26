#include "SoftwarePwm.h"

#include "assert/Assert.h"

namespace Garbox {

SoftwarePwm::SoftwarePwm(uint32_t periodMillis) : 
    // init members
    mPwmTimer(),
    mPeriodDuration(periodMillis),
    mHighDuration(0),
    mLowDuration(periodMillis){
    // nothing to do
}

void SoftwarePwm::setStateChangedHandler(Handler handler){
    mHandler = handler;
}

void SoftwarePwm::reset(){
    enterState(State::Off);
}

void SoftwarePwm::tick(){
    mCurrentTickRunsCount = 0;
    runStateMachine();
}

void SoftwarePwm::runStateMachine(){

    // prevent recursive lockup
    if(++mCurrentTickRunsCount > MaxRunsPerTick){
        AssertDebug(false, "SoftwarePwm::runStateMachine() exceeded max runs");
        return;
    }

    switch(mState){
        case State::Off:
            break;
        case State::High:
            if(mPwmTimer.getElapsedMillis() >= mHighDuration){
                enterState(State::Low);
            }
            break;
        case State::Low:
            if(mPwmTimer.isExpired()){
                startNextCycle();
            }
            break;
        default:
            AssertDebug(false, "SoftwarePwm:updateStateMachine() unhandled state");
            break;
    }
}

void SoftwarePwm::enterState(State nextState){
    if(mState == nextState){
        return;
    }
    switch(nextState){
        case State::Off:
            mPwmTimer.reset();
            updateDutyCycle(0.0f);
            break;
        case State::Low:
            // nothing to do
            break;
        case State::High:
            // nothing to do
            break;
    }
    mState = nextState;
    if(mHandler != nullptr){
        mHandler(mState);
    }
    runStateMachine();
}

void SoftwarePwm::startNextCycle(){
    // apply new duty cycle 
    if(mNextDutyCycle != mCurrentDutyCycle){
        updateDutyCycle(mNextDutyCycle);
    }

    // enter next state
    if(mNextDutyCycle == 0.0f){
        // entter off state
        mPwmTimer.reset();
        enterState(State::Off);
    }
    else {
        // enter next pwm cycle
        mPwmTimer.restart();
        enterState(State::High);
    }
}

void SoftwarePwm::setNextDutyCycle(float duty){

    if(duty < 0.0f || duty > 1.0f){
        AssertDebug(false, "SoftwarePwm::setNextDutyCycle() invalid duty value");
        return; 
    }

    if(mState == State::Off && duty != 0.0f){
        // turn on
        updateDutyCycle(duty);
        mPwmTimer.start(mPeriodDuration);
        enterState(State::High);
    }
    else {
        // change duty cycle after finishing current cycle
        mNextDutyCycle = duty;
    }

}

float SoftwarePwm::getCurrentDutyCycle(){
    return mCurrentDutyCycle;
}

float SoftwarePwm::getNextDutyCycle(){
    return mNextDutyCycle;
}

void SoftwarePwm::updateDutyCycle(float duty){
    mCurrentDutyCycle = duty;
    mNextDutyCycle = duty;
    mHighDuration = static_cast<uint32_t>(static_cast<float>(mPeriodDuration) * duty + 0.5f);
    mLowDuration = mPeriodDuration - mHighDuration;
}

}