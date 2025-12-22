#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/controllers/FanControllerAbs.h"
// ==== GENERATED END: include ====================================================

#include "core/util/control/PidControl.h"
#include "modules/parts/fan/Fan.h"

namespace Garbox {

class AnimatedLed;
class Fan;

class FanController : public FanControllerAbs {
private:

    Fan& mFan;
    AnimatedLed& mStatusLed;
    PidControl mPid;

    bool mUsePid = false;
    float mTargetSpeed = 0.0f;

    float mLastMeasuredRpm = 0.0f;
    bool mStateChanged = false;

    void handleFanStateChanged(FanState oldState, FanState newState);
    void handleFanStalledAlert(uint32_t counter);

    void updateFanStatus();
    void updateFanSample();

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    FanController();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;

    // generated tick handlers
    void onInputTick() final;
    void onOutputTick() final;

    // generated event handlers
    void onFanCommandEvent(const FanCommandEvent& event) final;

// ==== GENERATED END: interface ==================================================

};

} // namespace