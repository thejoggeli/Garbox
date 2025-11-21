#pragma once

#include "app/controllers/generated/FanControllerAbs.h"
#include "modules/parts/fan/Fan.h"

namespace Garbox {

class AnimatedLed;
class Fan;

class FanController : public FanControllerAbs {
public:
    
    FanController(ComponentId id, const RuntimeContext& context);

    void onInputTick() final;
    void onOutputTick() final;

    void onFanCommand(const EventRead<EventPayload::FanCommand>& event) final;

private:

    Fan& mFan;
    AnimatedLed& mStatusLed;

    float mLastMeasuredRpm = 0.0f;
    bool mStateChanged = false;
    
    void onInit() final;
    void onStart() final;

    void handleFanStateChanged(FanState oldState, FanState newState);
    void handleFanStalledAlert(uint32_t counter);

    void sendStatusEvent();
    void sendSampleEvent(float measuredRpm);

};

} // namespace