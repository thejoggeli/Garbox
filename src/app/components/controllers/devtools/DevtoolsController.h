#pragma once

#include "app_generated/controllers/DevtoolsControllerAbs.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class DevtoolsController : public DevtoolsControllerAbs {
public:

    DevtoolsController();

    void onLoggingTick() final;

private:

    SoftwareTimer mProfilerTimer;

    void onInit() final;
    void onStart() final;

    void logProfiler();

};

} // namespace Garbox