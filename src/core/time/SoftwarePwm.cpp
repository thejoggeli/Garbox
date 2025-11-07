#include "SoftwarePwm.h"

#include "assert/Assert.h"

namespace Garbox {

SoftwarePwm::SoftwarePwm(float duty, uint32_t periodMicros): 
    // init members
    mTimer(),
    mCurrentPeriodDurationMicros(periodMicros),
    mNextPeriodDurationMicros(periodMicros),
    mCurrentDutyCycle(duty),
    mNextDutyCycle(duty){
    // constructor body
    updateThreshold();
}

void SoftwarePwm::start(){
    if(isRunning()){
        return;
    }
    startNextCycle();
}

void SoftwarePwm::stop(){
    if(isReset()){
        return;
    }
    mTimer.reset();
    updateState();
}

void SoftwarePwm::tick(){
    
    // abort if not running
    if(!isRunning()){
        return;
    }

    // cycle timer expired, start nexct cycle
    if(mTimer.isExpired()){
        startNextCycle();
    }
    // cycle timer still active, update state
    else {
        updateState();
    }
}

void SoftwarePwm::startNextCycle(){

    bool thresholdNeedsUpdate = false;

    // apply new period duration
    if(mCurrentPeriodDurationMicros != mNextPeriodDurationMicros){
        thresholdNeedsUpdate = true;
        mCurrentPeriodDurationMicros = mNextPeriodDurationMicros;
    }

    // apply new duty cycle
    if(mNextDutyCycle != mCurrentDutyCycle){
        thresholdNeedsUpdate = true;
        mCurrentDutyCycle = mNextDutyCycle;
    }

    // update threshold
    if(thresholdNeedsUpdate){
        updateThreshold();
    }

    // restart timer
    if(mTimer.isReset()){
        mTimer.start(mCurrentPeriodDurationMicros);
    }
    else {
        mTimer.restart(mCurrentPeriodDurationMicros);
    }

    // update state
    updateState();
}

void SoftwarePwm::updateState(){
    
    // next state to be entered
    State newState = mState;

    // in reset state
    if(mTimer.isReset()){
        newState = State::Reset;
    }
    // first half of cycle
    else if(mTimer.getElapsedMicros() < mThresholdMicros){
        switch(mMode){
        case Mode::HighLow:
            newState = State::High;
            break;
        case Mode::LowHigh:
            newState = State::Low;
            break;
        default:
            TriggerDebug("SoftwarePwm", "unhandled mode");
            break;
        }
    }
    // second half of cycle
    else {
        switch(mMode){
        case Mode::HighLow:
            newState = State::Low;
            break;
        case Mode::LowHigh:
            newState = State::High;
            break;
        default:
            TriggerDebug("SoftwarePwm", "unhandled mode");
            break;
        }
    }

    // enter new state
    enterState(newState);
}

void SoftwarePwm::enterState(State state){
    if(mState == state){
        return;
    }
    mState = state;
    if(mHandler){
        mHandler(state);
    }
}

void SoftwarePwm::updateThreshold(){
    mThresholdMicros = static_cast<uint32_t>(static_cast<float>(mCurrentPeriodDurationMicros) * mCurrentDutyCycle);
}

void SoftwarePwm::setPeriodDurationMicros(uint32_t durationMicros, bool finishCurrent){
    // period duration must be > 0
    if(durationMicros == 0){
        TriggerDebug("SoftwarePwm", "invalid period duration");
        return; 
    }

    // queue period duration for next pwm cycle 
    mNextPeriodDurationMicros = durationMicros;

    // start next pwm cycle immediately
    if(!finishCurrent && isRunning()){
        startNextCycle();
    }
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
    if(!finishCurrent && isRunning()){
        startNextCycle();
    }
}

void SoftwarePwm::setMode(Mode mode){
    mMode = mode;
    updateState();
}

void SoftwarePwm::setStateChangedHandler(Handler handler){
    mHandler = handler;
}

float SoftwarePwm::getCurrentDutyCycle() const {
    return mCurrentDutyCycle;
}

float SoftwarePwm::getNextDutyCycle() const {
    return mNextDutyCycle;
}

uint32_t SoftwarePwm::getCurrentPeriodDurationMicros() const {
    return mCurrentPeriodDurationMicros;
}

uint32_t SoftwarePwm::getNextPeriodDurationMicros() const {
    return mNextPeriodDurationMicros;
}

SoftwarePwm::State SoftwarePwm::getState() const {
    return mState;
}

bool SoftwarePwm::isReset() const {
    return (mState == State::Reset);
}

bool SoftwarePwm::isRunning() const {
    return (mState != State::Reset);
}


} // namespace
