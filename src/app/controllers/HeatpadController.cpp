#include "HeatpadController.h"

#include "app/providers/ColorMaps.h"
#include "app/providers/PartsProvider.h"
#include "modules/parts/heatpad/Heatpad.h"
#include "modules/parts/led/rgb/RgbLed.h"

namespace Garbox {

HeatpadController::HeatpadController(const RuntimeContext& context): 
    // init members
    HeatpadControllerAbs(context),
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
    mHeatpad.setDutyCycle(0.5f);
    mHeatpad.setPeriodDurationMicros(5000_ms);
    mHeatpad.setEnabled(true);
}

void HeatpadController::onInputTick(){
    mHeatpad.tick();

    // send changed event
    if(mStateChanged){
        sendStatusEvent();
        mStateChanged = false;
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

void HeatpadController::onHeatpadCommand(const EventRead<EventPayload::HeatpadCommand>& event){
    // apply enabled 
    if(mHeatpad.isEnabled() != event.payload->enabled){
        mHeatpad.setEnabled(event.payload->enabled);
        mStateChanged = true;
    }
    // apply duty cycle
    if(mHeatpad.getNextDutyCycle() != event.payload->dutyCycle){
        mHeatpad.setDutyCycle(event.payload->dutyCycle);
        mStateChanged = true;
    }
    // apply period
    if(mHeatpad.getNextPeriodDurationMicros() != event.payload->periodMicros){
        mHeatpad.setPeriodDurationMicros(event.payload->periodMicros);
        mStateChanged = true;
    }
}

void HeatpadController::handleHeatpadStateChanged(HeatpadState oldState, HeatpadState newState){
    mStateChanged = true;
}

void HeatpadController::sendStatusEvent(){
    EventWrite event = makeHeatpadStatusEvent();
    event.payload->state = mHeatpad.getState();
    event.payload->dutyCycle = mHeatpad.getCurrentDutyCycle();
    event.payload->periodMicros = mHeatpad.getCurrentPeriodDurationMicros();
    sendEvent(event);
}

} // namespace