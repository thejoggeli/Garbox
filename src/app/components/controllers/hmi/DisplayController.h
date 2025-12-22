#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/controllers/DisplayControllerAbs.h"
// ==== GENERATED END: include ====================================================

#include <functional>
#include "core/util/helpers/TimeFader.h"

namespace Garbox {

class Display;

class DisplayController : public DisplayControllerAbs {
private:

    Display& mDisplay;
    TimeFader mBacklightFader;
    uint32_t mRenderSkippedCount = 0;
    
    void setBrightnessSmooth(float brightness, uint32_t durationMicros);

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    DisplayController();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;

    // generated tick handlers
    void onRenderTick() final;

    // generated event handlers
    void onDisplayCommandEvent(const DisplayCommandEvent& event) final;

// ==== GENERATED END: interface ==================================================

};

} // namespace