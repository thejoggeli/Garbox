#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "generated/states/StateRegistry.h"
#include "core/application/runtime/RuntimeAbs.h"
#include "core/application/runtime/TickRunner.h"
// include all components
#include "app/components/behaviours/CalibrationBehaviour.h"
#include "app/components/behaviours/FermentationBehaviour.h"
#include "app/components/controllers/actuators/FanController.h"
#include "app/components/controllers/actuators/HeatpadController.h"
#include "app/components/controllers/devtools/DevtoolsController.h"
#include "app/components/controllers/heartbeat/HeartbeatController.h"
#include "app/components/controllers/hmi/DisplayController.h"
#include "app/components/controllers/hmi/InputController.h"
#include "app/components/controllers/hmi/TimeSeriesController.h"
#include "app/components/controllers/sensors/I2cPartsController.h"
#include "app/components/screens/debug/DebugScreen.h"
#include "app/components/screens/log/EventLogScreen.h"
#include "app/components/screens/log/StateLogScreen.h"
#include "app/components/screens/main/MainScreen.h"

namespace Garbox {

class Runtime : public RuntimeAbs {
public:

    Runtime();

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
    TimeSeriesController mTimeSeriesController;

    // screens instances
    MainScreen mMainScreen;
    DebugScreen mDebugScreen;
    EventLogScreen mEventLogScreen;
    StateLogScreen mStateLogScreen;

    static void handleTickStart(void* ctx);
    static void handleTickEnd(void* ctx);

    static void handleHeartbeatTick(void* ctx);
    static void handleSensorReadTick(void* ctx);
    static void handleUserInputTick(void* ctx);
    static void handleLogicTick(void* ctx);
    static void handleActorWriteTick(void* ctx);
    static void handleLoggingTick(void* ctx);
    static void handleRenderTick(void* ctx);

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