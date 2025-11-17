#pragma once

#include "core/controller/ControllerAbs.h"
#include "core/event/EventView.h"
#include "parts/heatpad/HeatpadState.h"

namespace Garbox {

class Heatpad;
class RgbLed;

class HeatpadController : public ControllerAbs {
public:
    
    HeatpadController();

    void onHeatpadCommand(const EventView<EventData::HeatpadCommand> event);

private:

    bool mApplyingCommand = false;
    Heatpad& mHeatpad;
    RgbLed& mLed;

    void onInit() final;
    void onStart() final;
    void onTick() final;

    void handleHeatpadStateChanged(HeatpadState oldState, HeatpadState newState);
    void sendStatusEvent();

};

} // namespace