#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/controllers/DevtoolsControllerAbs.h"
// ==== GENERATED END: include ====================================================

#include "core/time/SoftwareTimer.h"

namespace Garbox {

class DevtoolsController : public DevtoolsControllerAbs {
private:

    SoftwareTimer mProfilerTimer;
    void logProfiler();

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    DevtoolsController();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;

    // generated tick handlers
    void onLoggingTick() final;

// ==== GENERATED END: interface ==================================================

};

} // namespace Garbox