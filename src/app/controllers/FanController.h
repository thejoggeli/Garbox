#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "modules/parts/fan/Fan.h"

namespace Garbox {

class AnimatedLed;
class Fan;

class FanController : public ControllerAbs {
public:
    
    FanController(ComponentId id, const RuntimeContext& context);

    void onInputTick();
    void onOutputTick();

    void onFanCommand(const EventRead<EventPayload::FanCommand> event);

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