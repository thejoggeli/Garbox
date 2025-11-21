#pragma once

#include "app/controllers/generated/HeatpadControllerAbs.h"
#include "modules/parts/heatpad/HeatpadState.h"

namespace Garbox {

class Heatpad;
class RgbLed;

class HeatpadController : public HeatpadControllerAbs {
public:
    
    HeatpadController(ComponentId id, const RuntimeContext& context);

    void onInputTick() final;
    void onOutputTick() final;

    void onHeatpadCommand(const EventRead<EventPayload::HeatpadCommand>& event) final;

private:

    bool mStateChanged = false;
    Heatpad& mHeatpad;
    RgbLed& mLed;

    void onInit() final;
    void onStart() final;

    void handleHeatpadStateChanged(HeatpadState oldState, HeatpadState newState);
    void sendStatusEvent();

};

} // namespace