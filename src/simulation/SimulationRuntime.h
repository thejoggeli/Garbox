#pragma once

#include <cstdint>

#include "app/components/behaviours/FermentationBehaviour.h"
#include "core/application/runtime/RuntimeAbs.h"
#include "generated/states/types/FanSampleState.h"
#include "generated/states/types/FanStatusState.h"
#include "generated/states/types/FermentationStatusState.h"
#include "generated/states/types/HeatpadSampleState.h"
#include "generated/states/types/TemperatureSampleState.h"
#include "generated/states/types/TemperatureStatusState.h"
#include "shared/types/EventPayload.h"

#if defined(GARBOX_SIMULATION_UI)
#include "app/components/screens/debug/DebugScreen.h"
#include "app/components/screens/log/EventLogScreen.h"
#include "app/components/screens/log/StateLogScreen.h"
#include "app/components/screens/main/MainScreen.h"
#include "app/components/screens/simple/SimpleScreen.h"
#include "generated/states/types/ActiveBehaviourState.h"
#include "generated/states/types/ActiveScreenState.h"
#include "generated/states/types/DisplayDiagnosticsState.h"
#include "generated/states/types/DisplayStatusState.h"
#include "generated/states/types/HeatpadProgressState.h"
#include "generated/states/types/HeatpadSampleState.h"
#include "generated/states/types/HeatpadStatusState.h"
#endif

namespace Garbox {

/**
 * Deterministic, headless application runtime.
 *
 * It uses the real fermentation behaviour, generated state/event contracts,
 * and RuntimeAbs dispatch machinery. Physical controllers are replaced at the
 * semantic boundary by explicit inputs and recorded command payloads.
 */
class SimulationRuntime final : public RuntimeAbs {
public:
    struct Inputs {
        bool sensorAvailable = false;
        bool sensorSampleUpdated = true;
        float temperatureCelcius = 20.0f;
        float humidityRelative = 50.0f;
        bool fanStalled = false;
        float fanMeasuredRpm = 0.0f;
        bool heaterMeasurementOverride = false;
        float heaterMeasuredVoltage = 0.0f;
        float heaterMeasuredCurrent = 0.0f;
    };

    struct Outputs {
        FanCommandPayload fan {};
        HeatpadCommandPayload heatpad {};
        DisplayCommandPayload display {};
        uint32_t fanCommandCount = 0;
        uint32_t heatpadCommandCount = 0;
        uint32_t displayCommandCount = 0;
    };

    SimulationRuntime();

    void boot();
    void step();

    void setInputs(const Inputs& inputs);
    const Inputs& inputs() const;
    const Outputs& outputs() const;

    void requestFermentationEnabled(bool enabled);
    void requestTargetTemperature(float temperatureCelcius);
    void requestFanControl(bool automatic, float manualSpeed);
    void requestAutomaticFan(bool enabled);

    const FermentationStatusState& fermentationStatus() const;
    const TemperatureStatusState& temperatureStatus() const;
    const TemperatureSampleState& temperatureSample() const;
    uint32_t tickCount() const;

#if defined(GARBOX_SIMULATION_UI)
    void render();
    void injectButton(ButtonState oldState, ButtonState newState);
    void injectEncoderSteps(int32_t steps);
#endif

protected:
    void onInit() override;
    void onStart() override;
    void onRun(const TaskStopToken&) override;
    void onRouteStateChanged(const StateAbs& state) override;
    void onRouteEvent(const EventHeader* event) override;
    void onActiveBehaviourChanged() override;
    void onActiveScreenChanged() override;

    BehaviourAbs* resolveBehaviour(BehaviourId id) override;
    ControllerAbs* resolveController(ControllerId) override;
    ScreenAbs* resolveScreen(ScreenId) override;

private:
    Inputs mInputs {};
    Outputs mOutputs {};
    bool mBooted = false;

    FermentationStatusState mFermentationStatus;
    FanSampleState mFanSample;
    FanStatusState mFanStatus;
    TemperatureSampleState mTemperatureSample;
    TemperatureStatusState mTemperatureStatus;
    HeatpadSampleState mHeatpadSample;
    FermentationBehaviour mFermentationBehaviour;

#if defined(GARBOX_SIMULATION_UI)
    HeatpadStatusState mHeatpadStatus;
    HeatpadProgressState mHeatpadProgress;
    DisplayStatusState mDisplayStatus;
    DisplayDiagnosticsState mDisplayDiagnostics;
    ActiveBehaviourState mActiveBehaviourState;
    ActiveScreenState mActiveScreenState;
    SimpleScreen mSimpleScreen;
    MainScreen mMainScreen;
    DebugScreen mDebugScreen;
    EventLogScreen mEventLogScreen;
    StateLogScreen mStateLogScreen;
#endif

    void publishInputs();
};

} // namespace Garbox
