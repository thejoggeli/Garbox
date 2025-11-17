#pragma once

#include "core/controller/ControllerAbs.h"
#include "core/event/EventView.h"

namespace Garbox {

class Heatpad;
class RgbLed;

class HeatpadController : public ControllerAbs {
public:
    
    HeatpadController();

private:

    Heatpad& mHeatpad;
    RgbLed& mLed;

    void onInit() final;
    void onStart() final;
    void onTick() final;

};

} // namespace