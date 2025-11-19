#include "HeatpadController.h"

#include "app/providers/ColorMaps.h"
#include "app/providers/PartsProvider.h"
#include "modules/parts/heatpad/Heatpad.h"
#include "modules/parts/led/rgb/RgbLed.h"

namespace Garbox {

HeatpadController::HeatpadController(ComponentId id): 
    // init members
    ControllerAbs(id),
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

void HeatpadController::onHeatpadCommand(const EventView<EventData::HeatpadCommand> event){
    // apply enabled 
    if(mHeatpad.isEnabled() != event.data->enabled){
        mHeatpad.setEnabled(event.data->enabled);
        mStateChanged = true;
    }
    // apply duty cycle
    if(mHeatpad.getNextDutyCycle() != event.data->dutyCycle){
        mHeatpad.setDutyCycle(event.data->dutyCycle);
        mStateChanged = true;
    }
    // apply period
    if(mHeatpad.getNextPeriodDurationMicros() != event.data->periodMicros){
        mHeatpad.setPeriodDurationMicros(event.data->periodMicros);
        mStateChanged = true;
    }
    // send changed event
    if(mStateChanged){
        mStateChanged = false;
    }
}

void HeatpadController::handleHeatpadStateChanged(HeatpadState oldState, HeatpadState newState){
    mStateChanged = true;
}

void HeatpadController::sendStatusEvent(){
    EventWrapper wrapper = makeEvent<EventData::HeatpadStatus>();
    wrapper.data->state = mHeatpad.getState();
    sendEvent(wrapper.event);
}

} // namespace