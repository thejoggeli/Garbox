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
    AssertExit(periodMicros > 0, "SoftwarePwm", "invalid period value");
    AssertExit(duty >= 0.0f, "SoftwarePwm", "invalid duty value < 0");
    AssertExit(duty <= 1.0f, "SoftwarePwm", "invalid duty value > 1");
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
    enterState(State::Reset);
}

void SoftwarePwm::tick(){
    
    // abort if not running
    if(!isRunning()){
        return;
    }

    // cycle timer expired, start next cycle
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

    State newState = mState;

    // timer not running
    if (mTimer.isReset()){
        newState = State::Reset;
    }
    // 100% duty => always high
    else if (mCurrentDutyCycle == 1.0f){
        newState = State::High;
    }
    // 0% duty => always low
    else if (mCurrentDutyCycle == 0.0f){
        newState = State::Low;
    }
    else {
        // first or second phase of the pwm cycle
        bool inFirstPhase = (mTimer.getElapsedMicros() < mThresholdMicros);

        // start with high phase
        if (mMode == Mode::StartHigh){
            newState = inFirstPhase ? State::High : State::Low;
        }
        // start with low phase
        else {
            newState = inFirstPhase ? State::Low : State::High;
        }
    }

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
    float phaseFraction = (mMode == Mode::StartHigh) ? (mCurrentDutyCycle) : (1.0f - mCurrentDutyCycle);
    mThresholdMicros = static_cast<uint32_t>(static_cast<float>(mCurrentPeriodDurationMicros) * phaseFraction + 0.5f);
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
    updateThreshold();
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
