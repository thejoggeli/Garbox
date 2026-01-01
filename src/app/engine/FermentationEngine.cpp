#include "FermentationEngine.h"

#include "core/assert/Assert.h"
#include "core/log/Log.h"
#include "core/time/Time.h"

static constexpr float PidKp = 0.05f; 
static constexpr float PidKi = 0.02f;
static constexpr float PidKd = 0.0f;
static constexpr float PidOutputMin = 0.0f;
static constexpr float PidOutputMax = 1.0f;

namespace Garbox {

FermentationEngine::FermentationEngine():
    // init members
    mPid(PidKp, PidKi, PidKd, PidOutputMin, PidOutputMax){

    // setup finite state machine
    mFsm.init(State::Reset);
    mFsm.setTransitionDelayMicros(State::OverTemperature, State::Regulating, OverTemperatureExitTimeMicros);
    mFsm.setStateChangedCallback([this](State oldState, State newState){
        handleFsmStateChanged(oldState, newState);
    });
}

void FermentationEngine::reset(){
    mRegulationEnabled = false;
    mTargetTemperatureChanged = false;
    mFsm.reset();
    mPid.reset();
}

void FermentationEngine::setRegulationEnabled(bool enabled){
    if(enabled == mRegulationEnabled){
        return;
    }
    mRegulationEnabled = enabled;
    if(enabled){
        mFsm.forceTransition(State::Ready);
    }
    else {
        if(mInput.measuredTemperature > CooldownStateThresholdTemperature){
            mFsm.forceTransition(State::Cooldown);
        }
        else {
            mFsm.forceTransition(State::Reset);
        }
    }
}

void FermentationEngine::step(){

    setRegulationEnabled(mInput.regulationEnabled);

    // input tracking
    if(mInput.targetTemperature != mLastTargetTemperature){
        mTargetTemperatureChanged = true;
    }

    // update finite state machine
    switch(mFsm.getState()){
    case State::Reset:
        handleResetState();
        break;
    case State::Ready:
        handleReadyState();
        break;
    case State::Regulating:
        handleRegulatingState();
        break;
    case State::InvalidInput:
        handleInvalidInputState();
        break;
    case State::OverTemperature:
        handleOverTemperatureState();
        break;
    case State::Cooldown:
        handleCooldownState();
        break;
    default:
        TriggerExit("FermentationEngine", "unhandled state");
    }
    mFsm.tick();

    // write output
    switch(mFsm.getState()){
    // in reset and reset states, everything is disabled
    case State::Reset:
    case State::Ready:
        mOutput.heaterEnabled = false;
        mOutput.heaterPwmDuty = 0.0f;
        mOutput.fanEnabled = false;
        mOutput.fanTargetSpeed = 0.0f;
        break;
    // in regulatiog mode, heater is enabled and fan runs at fixed 40%  
    case State::Regulating:
        if(mPid.isReset()){
            mOutput.heaterEnabled = false; // disable heater until pid did at least one step
            mOutput.heaterPwmDuty = 0.0f;
        }
        else {
            mOutput.heaterEnabled = true;
            mOutput.heaterPwmDuty = mPid.getOutput();
        }
        mOutput.fanEnabled = true;
        mOutput.fanTargetSpeed = 0.4f;
        break;
    // in error and cooldown states, the heater is forced off and fan runs at fixed 100%
    case State::InvalidInput:
    case State::OverTemperature:
    case State::Cooldown:
        mOutput.heaterEnabled = false;
        mOutput.heaterPwmDuty = 0.0f;
        mOutput.fanEnabled = true;
        mOutput.fanTargetSpeed = 1.0f;
        break;
    default:
        TriggerExit("FermentationEngine", "unhandled state");
    }

    // these outputs are the same for all states
    mOutput.heaterPwmPeriodMicros = 5000_ms;

    // update input tracking
    mLastTargetTemperature = mInput.targetTemperature;
    mTargetTemperatureChanged = false;
}

void FermentationEngine::handleResetState(){
    if(mRegulationEnabled){
        mFsm.forceTransition(State::Ready);
    }
}

void FermentationEngine::handleReadyState(){
    if(!isInputInvalid()){
        if(isInOverTemperatureZone()){
            mFsm.forceTransition(State::OverTemperature);
        }
        else {
            mFsm.forceTransition(State::Regulating); // enter 'Regulating' state as soon as all inputs are available
        }
    }
}

void FermentationEngine::handleRegulatingState(){
    if(isInOverTemperatureZone()){
        mFsm.forceTransition(State::OverTemperature); // turn heatpad off if measured temperature over safety threshold
    }
    else if(isInputInvalid()){
        mFsm.forceTransition(State::InvalidInput); // turn heatpad off if sensor reports fault or is disabled
        return;
    }
    else {
        float deltaTime = Time::GetTickDeltaSeconds(); // tick delta time is accurate enough 
        mPid.step(mInput.measuredTemperature, mInput.targetTemperature, deltaTime); // do PID control step
    }
}

void FermentationEngine::handleInvalidInputState(){
    if(isInOverTemperatureZone()){
        mFsm.forceTransition(State::OverTemperature); // turn heatpad off if measured temperature over safety threshold
    }
    else if(!isInputInvalid()){
        mFsm.transition(State::Regulating);
    }
    else {
        mFsm.cancelPendingTransition();
    }
}

void FermentationEngine::handleOverTemperatureState(){
    if(!isInOverTemperatureZone()){
        if(mTargetTemperatureChanged){
            mFsm.forceTransition(State::Regulating); // transition immediately if user changed target temperature (and no longer in over-temperature zone) 
        } 
        else {
            mFsm.transition(State::Regulating);
        }
    }
    else {
        mFsm.cancelPendingTransition();
    }
}

void FermentationEngine::handleCooldownState(){
    if(!isInputInvalid() && (mInput.measuredTemperature < CooldownStateThresholdTemperature)){
        mFsm.forceTransition(State::Reset);
    }
}

void FermentationEngine::handleFsmStateChanged(State oldState, State newState){
    switch(newState){
    case State::Reset:
    case State::Ready:
    case State::Regulating:
    case State::InvalidInput:
    case State::OverTemperature:
    case State::Cooldown:
        mPid.reset();
        break;
    default:
        TriggerExit("FermentationEngine", "unhandled state");
    }
}

bool FermentationEngine::isInOverTemperatureZone() const {
    const float overTemperatureThreshold = mInput.targetTemperature + OverTemperatureDelta;
    return (mInput.measuredTemperature > overTemperatureThreshold) && mInput.measuredTemperatureValid;
}

bool FermentationEngine::isInputInvalid() const {
    return !mInput.measuredTemperatureValid || !mInput.measuredHumidityValid || mInput.fanStalled; 
}

} // namespace