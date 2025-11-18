#pragma once

#include "core/controller/ControllerAbs.h"
#include "core/event/EventView.h"
#include "modules/parts/heatpad/HeatpadState.h"

namespace Garbox {

class Heatpad;
class RgbLed;

class HeatpadController : public ControllerAbs {
public:
    
    HeatpadController(ControllerId id);

    void onInputTick();
    void onOutputTick();

    void onHeatpadCommand(const EventView<EventData::HeatpadCommand> event);

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