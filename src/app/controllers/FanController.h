#pragma once

#include "app/types/EventData.h"
#include "core/controller/ControllerAbs.h"
#include "core/event/EventView.h"
#include "core/time/SoftwareTimer.h"
#include "parts/fan/Fan.h"

namespace Garbox {

class AnimatedLed;
class Fan;

class FanController : public ControllerAbs {
public:
    
    FanController();

    void onFanEvent(const EventView<FanEventData>& event);
    void onHeartbeatEvent(const EventView<HeartbeatEventData>& event);

private:

    Fan& mFan;
    AnimatedLed& mStatusLed;

    SoftwareTimer mRpmTimer;

    uint32_t mSwitchState = 0;

    void onInit() final;
    void onStart() final;
    void onTick() final;

    void handleFanStateChanged(Fan::State oldState, Fan::State newState);
    void handleFanStalledAlert(uint32_t counter);

    void applySwitchState();

};

} // namespace