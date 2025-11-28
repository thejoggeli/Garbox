#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/runtime/RuntimeAbs.h"
#include "core/application/runtime/TickRunner.h"
#include "app/behaviours/CalibrationBehaviour.h"
#include "app/behaviours/FermentationBehaviour.h"
#include "app/controllers/actuators/FanController.h"
#include "app/controllers/actuators/HeatpadController.h"
#include "app/controllers/devtools/DevtoolsController.h"
#include "app/controllers/heartbeat/HeartbeatController.h"
#include "app/controllers/hmi/DisplayController.h"
#include "app/controllers/hmi/InputController.h"
#include "app/controllers/sensors/I2cPartsController.h"

namespace Garbox {

class GarboxRuntime : public RuntimeAbs {
public:

    GarboxRuntime();

private:

    TickRunner mTickRunner;

    CalibrationBehaviour mCalibrationBehaviour;
    FermentationBehaviour mFermentationBehaviour;

    DisplayController mDisplayController;
    DevtoolsController mDevtoolsController;
    FanController mFanController;
    HeartbeatController mHeartbeatController;
    HeatpadController mHeatpadController;
    InputController mInputController;
    I2cPartsController mI2cPartsController;

    void handleTickStart();
    void handleTickEnd();

    void handleHeartbeatTick();
    void handleInputTick();
    void handleLogicTick();
    void handleOutputTick();
    void handleLoggingTick();
    void handleRenderTick();

    void onInit() final;
    void onStart() final;
    void onRun() final;
    void onRouteEvent(const EventHeader* header) final;
    void onRegister() final;

    ControllerAbs* resolveController(ControllerId id) final;
    BehaviourAbs* resolveBehaviour(BehaviourId id) final;
    
};

} // namespace Garbox