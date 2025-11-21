// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/application/runtime/RuntimeAbs.h"

#include "app/behaviours/FermentationBehaviour.h"

#include "app/controllers/DisplayController.h"
#include "app/controllers/FanController.h"
#include "app/controllers/HeartbeatController.h"
#include "app/controllers/HeatpadController.h"
#include "app/controllers/InputController.h"
#include "app/controllers/I2cPartsController.h"

namespace Garbox {

class GarboxRuntime : public RuntimeAbs {
public:

    GarboxRuntime();

    void onHeartbeatTick();
    void onRenderTick();
    void onInputTick();
    void onOutputTick();
    void onLogicTick();

private:

    FermentationBehaviour mFermentationBehaviour;

    DisplayController mDisplayController;
    FanController mFanController;
    HeartbeatController mHeartbeatController;
    HeatpadController mHeatpadController;
    InputController mInputController;
    I2cPartsController mI2cPartsController;

    void onInit() final;
    void onStart() final;
    void onRouteEvent(const EventHeader* header) final;
    void onRegisterBehaviours() final;
    void onRegisterControllers() final;
};

} // namespace Garbox