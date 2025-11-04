#include "SoftwarePwm.h"

#include "assert/Assert.h"

namespace Garbox {

SoftwarePwm::SoftwarePwm(uint32_t periodMicros) : 
    // init members
    mPwmTimer(),
    mPeriodDurationMicros(periodMicros),
    mHighDurationMicros(0){
    // nothing to do
}

void SoftwarePwm::setStateChangedHandler(Handler handler){
    mHandler = handler;
}

void SoftwarePwm::reset(){
    mPwmTimer.reset();
    applyDutyCycle(0.0f);
    enterState(State::Reset);
}

void SoftwarePwm::tick(){
    mCurrentTickRunsCount = 0;
    runStateMachine();
}


void SoftwarePwm::setDutyCycle(float duty, bool finishCurrent){
    // duty cycle must be between 0 and 1
    if(duty < 0.0f || duty > 1.0f){
        TriggerDebug("SoftwarePwm", "invalid duty value");
        return; 
    }

    // queue duty cycle for next pwm cycle 
    mNextDutyCycle = duty;

    // start next pwm cycle immediately
    if(!finishCurrent || mState == State::Reset){
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
        TriggerDebug("SoftwarePwm", "exceeded max runs");
        return;
    }

    switch(mState){
        case State::Reset:
            break;
        case State::High:
            if(mPwmTimer.getElapsedMicros() >= mHighDurationMicros){
                if(mCurrentDutyCycle == 1.0f){
                    startNextCycle();
                }
                else {
                    enterState(State::Low);
                }
            }
            break;
        case State::Low:
            if(mPwmTimer.isExpired()){
                startNextCycle();
            }
            break;
        default:
            TriggerDebug("SoftwarePwm:runStateMachine()", "unhandled state");
            break;
    }
}

void SoftwarePwm::enterState(State nextState){
    if(mState == nextState){
        return;
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

    // start timer for next cycle
    if(mState == State::Reset){
        mPwmTimer.start(mPeriodDurationMicros);
    } 
    else {
        mPwmTimer.restart();
    }

    // enter state for next cycle
    if(mCurrentDutyCycle == 0.0f){
        enterState(State::Low);
    } 
    else {
        enterState(State::High);
    }
}

void SoftwarePwm::applyDutyCycle(float duty){
    mCurrentDutyCycle = duty;
    mNextDutyCycle = duty;
    mHighDurationMicros = static_cast<uint32_t>(static_cast<float>(mPeriodDurationMicros) * duty + 0.5f);
}

} // namespace
