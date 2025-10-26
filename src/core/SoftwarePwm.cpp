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


void SoftwarePwm::setDutyCycle(float duty, bool finishCurrent){
    // duty cycle must be between 0 and 1
    if(duty < 0.0f || duty > 1.0f){
        AssertDebug(false, "SoftwarePwm::setNextDutyCycle() invalid duty value");
        return; 
    }

    // queue duty cycle for next pwm cycle 
    mNextDutyCycle = duty;

    // start next pwm cycle immediately
    if(!finishCurrent || mState == State::Off){
        startNextCycle();
    }
}

float SoftwarePwm::getCurrentDutyCycle(){
    return mCurrentDutyCycle;
}

float SoftwarePwm::getNextDutyCycle(){
    return mNextDutyCycle;
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
            applyDutyCycle(0.0f);
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
        applyDutyCycle(mNextDutyCycle);
    }

    // enter next state
    if(mNextDutyCycle == 0.0f){
        // enter off state
        enterState(State::Off);
    }
    else {
        // enter next pwm cycle
        if(mState == State::Off){
            mPwmTimer.start(mPeriodDuration);
        } 
        else {
            mPwmTimer.restart();
        }
        enterState(State::High);
    }
}

void SoftwarePwm::applyDutyCycle(float duty){
    mCurrentDutyCycle = duty;
    mNextDutyCycle = duty;
    mHighDuration = static_cast<uint32_t>(static_cast<float>(mPeriodDuration) * duty + 0.5f);
    mLowDuration = mPeriodDuration - mHighDuration;
}

}