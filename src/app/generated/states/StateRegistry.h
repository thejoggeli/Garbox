// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "app_generated/states/types/DisplayStatusState.h"
#include "app_generated/states/types/DisplayDiagnosticsState.h"
#include "app_generated/states/types/FanStatusState.h"
#include "app_generated/states/types/FanSampleState.h"
#include "app_generated/states/types/FermentationStatusState.h"
#include "app_generated/states/types/HeatpadStatusState.h"
#include "app_generated/states/types/HeatpadSampleState.h"
#include "app_generated/states/types/TemperatureStatusState.h"
#include "app_generated/states/types/TemperatureSampleState.h"
#include "app_generated/states/types/ActiveBehaviourState.h"
#include "app_generated/states/types/ActiveScreenState.h"

namespace Garbox {

class StateRegistry final {
public:

    StateRegistry();
    ~StateRegistry() = default;

    // states accessors
    DisplayStatusState& getDisplayStatus();
    DisplayDiagnosticsState& getDisplayDiagnostics();
    FanStatusState& getFanStatus();
    FanSampleState& getFanSample();
    FermentationStatusState& getFermentationStatus();
    HeatpadStatusState& getHeatpadStatus();
    HeatpadSampleState& getHeatpadSample();
    TemperatureStatusState& getTemperatureStatus();
    TemperatureSampleState& getTemperatureSample();
    ActiveBehaviourState& getActiveBehaviour();
    ActiveScreenState& getActiveScreen();

private:

    // states instances
    DisplayStatusState mDisplayStatus;
    DisplayDiagnosticsState mDisplayDiagnostics;
    FanStatusState mFanStatus;
    FanSampleState mFanSample;
    FermentationStatusState mFermentationStatus;
    HeatpadStatusState mHeatpadStatus;
    HeatpadSampleState mHeatpadSample;
    TemperatureStatusState mTemperatureStatus;
    TemperatureSampleState mTemperatureSample;
    ActiveBehaviourState mActiveBehaviour;
    ActiveScreenState mActiveScreen;

    // disallow copy and move
    StateRegistry(const StateRegistry&) = delete;
    StateRegistry& operator=(const StateRegistry&) = delete;
    StateRegistry(StateRegistry&&) = delete;
    StateRegistry& operator=(StateRegistry&&) = delete;

};

} // namespace Garbox