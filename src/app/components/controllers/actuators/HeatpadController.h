#pragma once

#include "generated/controllers/HeatpadControllerAbs.h"
#include "modules/parts/heatpad/HeatpadState.h"

namespace Garbox {

class Heatpad;
class RgbLed;

class HeatpadController : public HeatpadControllerAbs {
public:
    
    HeatpadController();

    void onInputTick() final;
    void onOutputTick() final;

    void onHeatpadCommandEvent(const HeatpadCommandEvent& event) final;

private:

    float mLastMeasuredVoltage = 0.0f;
    float mLastMeasuredCurrent = 0.0f;

    bool mStateChanged = true;
    bool mSensorValuesChanged = true;

    Heatpad& mHeatpad;
    RgbLed& mLed;

    void onInit() final;
    void onStart() final;

    void handleHeatpadStateChanged(HeatpadState oldState, HeatpadState newState);
    void sendStatusEvent();
    void updateSensorValues();

};

} // namespace