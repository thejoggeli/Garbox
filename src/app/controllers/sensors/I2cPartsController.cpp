#include "I2cPartsController.h"

#include "app/hardware/GpioInstances.h"
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
        sendTemperatureSampleEvent();
        mNewSample = false;
    }

    // send if status event changed flag is set
    if(mStateChanged){
        sendTemperatureStatusEvent();
        mStateChanged = false;
    }
}

void I2cPartsController::onButtonStateChanged(const ButtonStateChangedEvent& event){
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

void I2cPartsController::sendTemperatureStatusEvent(){
    TemperatureStatusEvent event = makeTemperatureStatusEvent();
    event->powerEnabled = mEnablePowerGpio.readLevel();
    event->driverEnabled = mTemperatureSensor.isStarted();
    event->resetting = mResetting;
    event->hasFirstSample = mHasFirstSample;
    sendEvent(event);
}

void I2cPartsController::sendTemperatureSampleEvent(){
    TemperatureSampleEvent event = makeTemperatureSampleEvent();
    event->temperatureCelcius = mTemperatureSensor.getTemperatureCelcius();
    event->humidityRelative = mTemperatureSensor.getHumidityRelative();
    sendEvent(event);
}

} // namespace