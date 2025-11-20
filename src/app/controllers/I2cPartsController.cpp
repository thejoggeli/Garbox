#include "I2cPartsController.h"

#include "app/hardware/gpio/GpioInstances.h"
#include "app/providers/PartsProvider.h"
#include "core/assert/Assert.h"
#include "core/hardware/gpio/Gpio.h"
#include "core/time/Time.h"
#include "modules/parts/temperature/Sht31.h"


namespace Garbox {

I2cPartsController::I2cPartsController(ComponentId id, const RuntimeContext& context): 
    // init members
    ControllerAbs(id, context),
    mEnablePowerGpio(GpioInstances::GetI2cEnablePower()),
    mTemperatureSensor(PartsProvider::GetTemperatureSensor()){
    // nothing to do
}

void I2cPartsController::onInit(){
    // enable i2c power and wait before starting communication with temperature sensor
    // the sht31 needs ~1ms internal setup time
    enterFsmState(FsmState::ResetPowerOnPhase);
}

void I2cPartsController::onStart(){
    // nothing to do
}

void I2cPartsController::onInputTick(){

    // handle current fsm state
    switch(mFsmState){
    case FsmState::ResetPowerOffPhase:
        if(mResetTimer.isExpired()){
            enterFsmState(FsmState::ResetPowerOnPhase);
        }
        break;
    case FsmState::ResetPowerOnPhase:
        if(mResetTimer.isExpired()){
            enterFsmState(FsmState::Running);
        }
        break;
    case FsmState::Running:
        if(mTemperatureSensor.tryFetch()){
            mChanged = true;
        }
        break;
    default:
        TriggerDebug("I2cPartsController", "unhandled state");
    }

    // send if status event changed flag is set
    if(mChanged){
        sendTemperatureStatusEvent();
        mChanged = false;
    }
}

void I2cPartsController::onOutputTick(){
    // nothing to do
}

void I2cPartsController::enterFsmState(FsmState state){
    if(state == mFsmState){
        return;
    }
    switch(state){
    case FsmState::ResetPowerOffPhase:
        if(mTemperatureSensor.isStarted()){
            mTemperatureSensor.reset();
        }
        mResetTimer.start(ResetPowerOffPhaseDurationMicros);
        mEnablePowerGpio.writeLevel(false);
        mChanged = true;
        break;
    case FsmState::ResetPowerOnPhase:
        if(mTemperatureSensor.isStarted()){
            mTemperatureSensor.reset();
        }
        mResetTimer.start(ResetPowerOnPhaseDurationMicros);
        mEnablePowerGpio.writeLevel(true);
        mChanged = true;
        break;
    case FsmState::Running:
        mTemperatureSensor.start();
        mResetTimer.reset();
        mChanged = true;
        break;
    default:
        TriggerDebug("I2cPartsController", "unhandled state");
    }
    mFsmState = state;
}

void I2cPartsController::sendTemperatureStatusEvent(){
    EventWrite event = makeEvent<EventData::TemperatureStatus>();
    event.payload->sensorEnabled = mTemperatureSensor.isStarted();
    event.payload->sensorError = false;
    event.payload->temperatureCelcius = mTemperatureSensor.getTemperatureCelcius();
    event.payload->humidityRelative = mTemperatureSensor.getHumidityRelative();
    sendEvent(event.header);
}

} // namespace