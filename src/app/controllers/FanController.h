#pragma once

#include "app/types/EventData.h"
#include "core/controller/ControllerAbs.h"
#include "core/event/EventView.h"
#include "parts/fan/Fan.h"

namespace Garbox {

class AnimatedLed;
class Fan;

class FanController : public ControllerAbs {
public:
    
    FanController();

    void onFanCommand(const EventView<EventData::FanCommand> event);
    void onHeartbeat(const EventView<EventData::Heartbeat>& event);

private:

    Fan& mFan;
    AnimatedLed& mStatusLed;

    uint32_t mSwitchState = 0;
    bool mApplyingCommand = false;

    void onInit() final;
    void onStart() final;
    void onTick() final;

    void handleFanStateChanged(FanState oldState, FanState newState);
    void handleFanStalledAlert(uint32_t counter);

    void applySwitchState();
    void sendStatusEvent();

};

} // namespace