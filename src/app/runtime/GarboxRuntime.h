#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/runtime/EventReplay.h"
#include "core/application/runtime/RuntimeAbs.h"
#include "core/application/runtime/TickRunner.h"
// include all components
#include "app/behaviours/CalibrationBehaviour.h"
#include "app/behaviours/FermentationBehaviour.h"
#include "app/controllers/actuators/FanController.h"
#include "app/controllers/actuators/HeatpadController.h"
#include "app/controllers/devtools/DevtoolsController.h"
#include "app/controllers/heartbeat/HeartbeatController.h"
#include "app/controllers/hmi/DisplayController.h"
#include "app/controllers/hmi/InputController.h"
#include "app/controllers/sensors/I2cPartsController.h"
#include "app/screens/debug/DebugScreen.h"
#include "app/screens/eventlog/EventLogScreen.h"
#include "app/screens/main/MainScreen.h"

namespace Garbox {

class GarboxRuntime : public RuntimeAbs {
public:

    GarboxRuntime();

private:

    TickRunner mTickRunner;
    EventReplay mEventReplay;

    CalibrationBehaviour mCalibrationBehaviour;
    FermentationBehaviour mFermentationBehaviour;

    DisplayController mDisplayController;
    DevtoolsController mDevtoolsController;
    FanController mFanController;
    HeartbeatController mHeartbeatController;
    HeatpadController mHeatpadController;
    InputController mInputController;
    I2cPartsController mI2cPartsController;

    MainScreen mMainScreen;
    DebugScreen mDebugScreen;
    EventLogScreen mEventLogScreen;

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
    void onActiveBehaviourChanged() final;
    void onActiveScreenChanged() final;

    BehaviourAbs* resolveBehaviour(BehaviourId id) final;
    ControllerAbs* resolveController(ControllerId id) final;
    ScreenAbs* resolveScreen(ScreenId id) final;
    
};

} // namespace Garbox