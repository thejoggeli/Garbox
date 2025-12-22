#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/components/controllers/heatpad/HeatpadControllerAbs.h"
// ==== GENERATED END: include ====================================================

#include "modules/parts/heatpad/HeatpadState.h"

namespace Garbox {

class Heatpad;
class RgbLed;

class HeatpadController : public HeatpadControllerAbs {
private:

    float mLastMeasuredVoltage = 0.0f;
    float mLastMeasuredCurrent = 0.0f;

    bool mStateChanged = true;
    bool mSensorValuesChanged = true;

    Heatpad& mHeatpad;
    RgbLed& mLed;

    void handleHeatpadStateChanged(HeatpadState oldState, HeatpadState newState);
    void updateSensorValues();

    void updateStatus();
    void updateSample();

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    HeatpadController();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;

    // generated tick handlers
    void onInputTick() final;
    void onOutputTick() final;

    // generated event handlers
    void onHeatpadCommandEvent(const HeatpadCommandEvent& event) final;

// ==== GENERATED END: interface ==================================================

};

} // namespace