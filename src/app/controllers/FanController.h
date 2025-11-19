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
    
    FanController(ComponentId id);

    void onInputTick();
    void onOutputTick();

    void onFanCommand(const EventView<EventData::FanCommand> event);
    void onHeartbeat(const EventView<EventData::Heartbeat>& event);

private:

    Fan& mFan;
    AnimatedLed& mStatusLed;

    bool mStateChanged = false;
    
    void onInit() final;
    void onStart() final;

    void handleFanStateChanged(FanState oldState, FanState newState);
    void handleFanStalledAlert(uint32_t counter);

    void sendStatusEvent();

};

} // namespace