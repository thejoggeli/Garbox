#pragma once

#include "app/controllers/abstract/FanControllerAbs.h"
#include "core/util/control/PidControl.h"
#include "modules/parts/fan/Fan.h"

namespace Garbox {

class AnimatedLed;
class Fan;

class FanController : public FanControllerAbs {
public:
    
    FanController();

    void onInputTick() final;
    void onOutputTick() final;

    void onFanCommandEvent(const FanCommandEvent& event) final;

private:

    Fan& mFan;
    AnimatedLed& mStatusLed;
    PidControl mPid;

    bool mUsePid = false;
    float mTargetSpeed = 0.0f;

    float mLastMeasuredRpm = 0.0f;
    bool mStateChanged = false;
    
    void onInit() final;
    void onStart() final;

    void handleFanStateChanged(FanState oldState, FanState newState);
    void handleFanStalledAlert(uint32_t counter);

    void updateFanStatus();
    void updateFanSample();

};

} // namespace