#include "I2cPartsController.h"

#include "generated/hardware/GpioInstances.h"
#include "app/providers/PartsProvider.h"
#include "core/assert/Assert.h"
#include "core/hardware/gpio/Gpio.h"
#include "core/time/Time.h"
#include "modules/parts/temperature/Sht31.h"

namespace Garbox {

I2cPartsController::I2cPartsController(): 
    // init members
    I2cPartsControllerAbs(),
    mEnablePowerGpio(GpioInstances::GetI2cEnable()),
    mTemperatureSensor(PartsProvider::GetTemperatureSensor()){
    // nothing to do
}

void I2cPartsController::onInit(){
    // enable i2c power and wait before starting communication with temperature sensor
    // the sht31 needs ~1ms internal setup time
    mFsm.setTransitionDelayMicros(FsmState::ResetPowerOffPhase, FsmState::ResetPowerOnPhase, ResetPowerOffPhaseDurationMicros);
    mFsm.setTransitionDelayMicros(FsmState::ResetPowerOnPhase, FsmState::Running, ResetPowerOnPhaseDurationMicros);
    mFsm.setTransitionDelayMicros(FsmState::Running, FsmState::ResetPowerOffPhase, MaxFetchDurationBeforeRestartMicros);
    mFsm.init(FsmState::ResetPowerOnPhase);
    mFsm.setStateChangedCallback([this](FsmState oldState, FsmState newState){
        handleStateChanged(oldState, newState);
    });
}

void I2cPartsController::onStart(){
    // nothing to do
}

void I2cPartsController::onInputTick(){

    // handle current fsm state
    mFsm.tick();
    switch(mFsm.getState()){
    case FsmState::ResetPowerOffPhase:
        mFsm.transition(FsmState::ResetPowerOnPhase);
        break;
    case FsmState::ResetPowerOnPhase:
        mFsm.transition(FsmState::Running);
        break;
    case FsmState::Running:
        handleRunningState();
        break;
    default:
        TriggerDebug("I2cPartsController", "unhandled state");
    }

    // set new sample
    if(mNewSample){
        updateTemperatureSample();
        mNewSample = false;
    }

    // send if status event changed flag is set
    if(mStateChanged){
        updateTemperatureStatus();
        mStateChanged = false;
    }
}

void I2cPartsController::onButtonStateChangedEvent(const ButtonStateChangedEvent& event){
    if(event->newState == ButtonState::Pressed){
        mFsm.forceTransition(FsmState::ResetPowerOffPhase);
    }
}

void I2cPartsController::handleRunningState(){
    mNewSample = mTemperatureSensor.tryFetch();
    if(!mNewSample){
        mFsm.transition(FsmState::ResetPowerOffPhase);
    }
    else {
        mFsm.cancelPendingTransition();
        if(!mHasFirstSample){
            mHasFirstSample = true;
            mStateChanged = true;
        }
    }
}

void I2cPartsController::handleStateChanged(FsmState oldState, FsmState newState){
    mStateChanged = true;
    switch(newState){
    case FsmState::ResetPowerOffPhase:
        if(mTemperatureSensor.isStarted()){
            mTemperatureSensor.reset();
        }
        mHasFirstSample = false;
        mResetting = true;
        mEnablePowerGpio.writeLevel(false);
        break;
    case FsmState::ResetPowerOnPhase:
        if(mTemperatureSensor.isStarted()){
            mTemperatureSensor.reset();
        }
        mHasFirstSample = false;
        mResetting = true;
        mEnablePowerGpio.writeLevel(true);
        break;
    case FsmState::Running:
        mTemperatureSensor.start();
        mResetting = false;
        break;
    default:
        TriggerDebug("I2cPartsController", "unhandled state");
    }
}

void I2cPartsController::updateTemperatureStatus(){
    states().temperatureStatus.setPowerEnabled(mEnablePowerGpio.readLevel());
    states().temperatureStatus.setDriverEnabled(mTemperatureSensor.isStarted());
    states().temperatureStatus.setResetting(mResetting);
    states().temperatureStatus.setHasFirstSample(mHasFirstSample);
}

void I2cPartsController::updateTemperatureSample(){
    states().temperatureSample.setTemperatureCelcius(mTemperatureSensor.getTemperatureCelcius());
    states().temperatureSample.setHumidityRelative(mTemperatureSensor.getHumidityRelative());
}

} // namespace