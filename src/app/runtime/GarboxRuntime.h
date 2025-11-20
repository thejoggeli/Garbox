#pragma once

#include "app/behaviours/FermentationBehaviour.h"
#include "app/controllers/DisplayController.h"
#include "app/controllers/FanController.h"
#include "app/controllers/HeartbeatController.h"
#include "app/controllers/HeatpadController.h"
#include "app/controllers/InputController.h"
#include "app/controllers/I2cPartsController.h"
#include "core/application/runtime/RuntimeAbs.h"

namespace Garbox {

class GarboxRuntime : public RuntimeAbs {
public:

    GarboxRuntime();

    void onMainTick();
    void onDisplayTick();

private:

    // behaviours
    FermentationBehaviour mFermentationBehaviour;

    // controllers
    DisplayController mDisplayController;
    FanController mFanController;
    InputController mInputController;
    HeatpadController mHeatpadController;
    HeartbeatController mHeartbeatController;
    I2cPartsController mI2cPartsController;

    void onInit() final;
    void onStart() final;
    void onRouteEvent(const EventHeader* header) final;
    void onRegisterBehaviours() final;
    void onRegisterControllers() final;
};

}
