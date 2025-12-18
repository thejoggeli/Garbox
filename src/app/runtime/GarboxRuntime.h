#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/states/StateRegistry.h"
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

    // state registry instance
    StateRegistry mStateRegistry;

    // behaviours instances
    CalibrationBehaviour mCalibrationBehaviour;
    FermentationBehaviour mFermentationBehaviour;

    // controllers instances
    DisplayController mDisplayController;
    DevtoolsController mDevtoolsController;
    FanController mFanController;
    HeartbeatController mHeartbeatController;
    HeatpadController mHeatpadController;
    InputController mInputController;
    I2cPartsController mI2cPartsController;

    // screens instances
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
    void onRouteStateChanged(const StateAbs& state) final;
    void onRouteEvent(const EventHeader* header) final;
    void onActiveBehaviourChanged() final;
    void onActiveScreenChanged() final;

    BehaviourAbs* resolveBehaviour(BehaviourId id) final;
    ControllerAbs* resolveController(ControllerId id) final;
    ScreenAbs* resolveScreen(ScreenId id) final;
    
};

} // namespace Garbox