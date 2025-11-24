#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/runtime/RuntimeAbs.h"

// include all behaviours
#include "app/behaviours/CalibrationBehaviour.h"
#include "app/behaviours/FermentationBehaviour.h"

// include all controllers
#include "app/controllers/DisplayController.h"
#include "app/controllers/FanController.h"
#include "app/controllers/HeartbeatController.h"
#include "app/controllers/HeatpadController.h"
#include "app/controllers/InputController.h"
#include "app/controllers/I2cPartsController.h"

namespace Garbox {

class GarboxRuntime : public RuntimeAbs {
public:

    GarboxRuntime(const RuntimeAbs::Config& config);

    void onHeartbeatTick();
    void onInputTick();
    void onLogicTick();
    void onOutputTick();
    void onRenderTick();

private:

    ControllerAbs* resolveController(ControllerId id) final;
    BehaviourAbs* resolveBehaviour(BehaviourId id) final;

    CalibrationBehaviour mCalibrationBehaviour;
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