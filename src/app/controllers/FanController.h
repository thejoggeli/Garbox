#pragma once

#include "shared/types/EventData.h"
#include "core/controller/ControllerAbs.h"
#include "core/event/EventView.h"
#include "modules/parts/fan/Fan.h"

namespace Garbox {

class AnimatedLed;
class Fan;

class FanController : public ControllerAbs {
public:
    
    FanController(ControllerId id);

    void onInputTick();
    void onOutputTick();

    void onFanCommand(const EventView<EventData::FanCommand> event);
    void onHeartbeat(const EventView<EventData::Heartbeat>& event);

private:

    Fan& mFan;
    AnimatedLed& mStatusLed;

    bool mFanStateChanged = false;

    bool mSwitchStateChanged = false;
    uint32_t mSwitchState = 0;
    
    void onInit() final;
    void onStart() final;

    void handleFanStateChanged(FanState oldState, FanState newState);
    void handleFanStalledAlert(uint32_t counter);

    void applySwitchState();
    void sendStatusEvent();

};

} // namespace