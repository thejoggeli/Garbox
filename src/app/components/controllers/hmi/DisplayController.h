#pragma once

#include <functional>
#include "app_generated/controllers/DisplayControllerAbs.h"
#include "core/time/SoftwareTimer.h"
#include "core/util/container/static/VectorStatic.h"
#include "core/util/helpers/TimeFader.h"
#include "modules/parts/fan/FanState.h"
#include "modules/parts/heatpad/HeatpadState.h"
#include "shared/types/BehaviourId.h"

namespace Garbox {

class Display;

class DisplayController : public DisplayControllerAbs {
public:
    
    DisplayController();

    void onRenderTick() final;
    void onDisplayCommandEvent(const DisplayCommandEvent& event) final;

private:

    Display& mDisplay;

    TimeFader mBacklightFader;

    uint32_t mRenderSkippedCount = 0;

    void onInit() final;
    void onStart() final;

    void setBrightnessSmooth(float brightness, uint32_t durationMicros);

};

} // namespace