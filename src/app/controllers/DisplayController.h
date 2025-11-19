#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "modules/parts/fan/FanState.h"
#include "modules/parts/heatpad/HeatpadState.h"

namespace Garbox {

class Display;

class DisplayController : public ControllerAbs {
public:

    struct State {
        FanState fanState;
        float fanTargetSpeed;
        float fanMeasuredRpm;
        HeatpadState heatpadState;
        float heatpadDuty;
        float heatpadVoltage;
        float heatpadCurrent;
        uint32_t renderSkippedCount = 0xFFFFFFFF;
    };
    
    DisplayController(ComponentId id);

    void onTick();

private:

    Display& mDisplay;
    State mState {};

    uint32_t mRenderSkippedCount = 0;

    void onInit() final;
    void onStart() final;


};

} // namespace