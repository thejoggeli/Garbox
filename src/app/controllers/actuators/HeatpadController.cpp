#include "HeatpadController.h"

#include "app/providers/ColorMaps.h"
#include "app/providers/PartsProvider.h"
#include "modules/parts/heatpad/Heatpad.h"
#include "modules/parts/led/rgb/RgbLed.h"

namespace Garbox {

HeatpadController::HeatpadController(): 
    // init members
    HeatpadControllerAbs(),
    mHeatpad(PartsProvider::GetHeatpad()),
    mLed(PartsProvider::GetRgbLed()){
    // nothing to do
}

void HeatpadController::onInit(){
    // heatpad already initialized in parts provider
    mHeatpad.setStateChangedHandler([this](HeatpadState oldState, HeatpadState newState){
        handleHeatpadStateChanged(oldState, newState);
    });
}

void HeatpadController::onStart(){
    // disable heatpad by default
    mHeatpad.setEnabled(false);
    mHeatpad.setDutyCycle(0.0f);
    mHeatpad.setPeriodDurationMicros(5000_ms);
}

void HeatpadController::onInputTick(){
    mHeatpad.tick();

    // send sample if measured voltage or current changed
    updateSensorValues();

    // send changed event
    if(mStateChanged){
        sendStatusEvent();
        mStateChanged = false;
    }

    // send sample event
    if(mHeatpad.isEnabled() || mSensorValuesChanged){
        HeatpadSampleEvent event = makeHeatpadSampleEvent();
        event->measuredVoltage = mLastMeasuredVoltage;
        event->measuredCurrent = mLastMeasuredCurrent;
        event->pwmProgressMicros = mHeatpad.getPwmProgressMicros();
        sendEvent(event);
        mSensorValuesChanged = false;
    }
}

void HeatpadController::onOutputTick(){

    // get measured voltage
    const float measuredVoltageNorm = mHeatpad.getMeasuredVoltage() / 17.0f;

    // get interpolated color
    static const ColorMap& colorMap = ColorMaps::GetRedBlue();
    HslColor hslColor = colorMap.interpolateHsl(measuredVoltageNorm);
    hslColor.l = 0.14f;

    // update rgb led 
    mLed.setColor(hslColor.toLinearRgb());
}

void HeatpadController::onHeatpadCommand(const HeatpadCommandEvent& event){
    // apply enabled 
    if(mHeatpad.isEnabled() != event->enabled){
        mHeatpad.setEnabled(event->enabled);
        mStateChanged = true;
    }
    // apply duty cycle
    if(mHeatpad.getNextDutyCycle() != event->dutyCycle){
        mHeatpad.setDutyCycle(event->dutyCycle);
        mStateChanged = true;
    }
    // apply period
    if(mHeatpad.getNextPeriodDurationMicros() != event->periodMicros){
        mHeatpad.setPeriodDurationMicros(event->periodMicros);
        mStateChanged = true;
    }
}

void HeatpadController::handleHeatpadStateChanged(HeatpadState oldState, HeatpadState newState){
    mStateChanged = true;
    mSensorValuesChanged = true;
}

void HeatpadController::sendStatusEvent(){
    HeatpadStatusEvent event = makeHeatpadStatusEvent();
    event->state = mHeatpad.getState();
    event->currentDutyCycle = mHeatpad.getCurrentDutyCycle();
    event->currentPeriodMicros = mHeatpad.getCurrentPeriodDurationMicros();
    event->nextDutyCycle = mHeatpad.getNextDutyCycle();
    event->nextPeriodMicros = mHeatpad.getNextPeriodDurationMicros();
    sendEvent(event);
}

void HeatpadController::updateSensorValues(){
    float voltage = mHeatpad.getMeasuredVoltage();
    float current = mHeatpad.getMeasuredCurrent();
    if(voltage != mLastMeasuredVoltage){
        mLastMeasuredVoltage = voltage;
        mSensorValuesChanged = true;
    }
    if(current != mLastMeasuredCurrent){
        mLastMeasuredCurrent = current;
        mSensorValuesChanged = true;
    }
}

} // namespace