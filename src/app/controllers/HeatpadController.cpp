#include "HeatpadController.h"

#include "app/providers/ColorMaps.h"
#include "app/providers/PartsProvider.h"
#include "parts/heatpad/Heatpad.h"
#include "parts/led/rgb/RgbLed.h"

namespace Garbox {

HeatpadController::HeatpadController(): 
    // init members
    ControllerAbs(ControllerId::Heatpad),
    mHeatpad(PartsProvider::GetHeatpad()),
    mLed(PartsProvider::GetRgbLed()){
    // nothing to do
}

void HeatpadController::onInit(){
    // heatpad already initialized in parts provider
}

void HeatpadController::onStart(){
    mHeatpad.setDutyCycle(0.5f);
    mHeatpad.setPeriodDurationMicros(5000_ms);
    mHeatpad.start();
}

void HeatpadController::onTick(){
    mHeatpad.tick();

    // rgb led tick
    static const ColorMap& colorMap = ColorMaps::GetRedBlue();
    constexpr float brightness = 0.14f;
    float const tColorMap = mHeatpad.getMeasuredVoltage() / 17.0f;
    HslColor hslColor = colorMap.interpolateHsl(tColorMap);
    hslColor.l = brightness;
    mLed.setColor(hslColor.toLinearRgb());
}



} // namespace