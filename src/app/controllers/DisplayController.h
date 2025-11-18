#pragma once

#include "core/controller/ControllerAbs.h"
#include "core/event/EventView.h"

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
        uint32_t renderSkippedCount;
    };
    
    DisplayController(ControllerId id);

    void onRenderTick();

private:

    Display& mDisplay;
    State mState {};

    uint32_t mRenderSkippedCount = 0;

    void onInit() final;
    void onStart() final;


};

} // namespace