#pragma once

#include "app/controllers/DisplayController.h"
#include "app/controllers/FanController.h"
#include "app/controllers/GarboxController.h"
#include "app/controllers/HeartbeatController.h"
#include "app/controllers/HeatpadController.h"

#include "core/application/system/SystemRuntimeAbs.h"

namespace Garbox {

class SystemRuntime : public SystemRuntimeAbs {
public:

    SystemRuntime();

    void onMainTick();
    void onDisplayTick();

private:

    // controllers
    DisplayController mDisplayController;
    FanController mFanController;
    GarboxController mGarboxController;
    HeatpadController mHeatpadController;
    HeartbeatController mHeartbeatController;

    void onInit() final;
    void onStart() final;
    void onRouteEvent(const Event* event) final;
    void onRegisterControllers() final;
};

}