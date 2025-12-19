// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "generated/states/types/DisplayStatusState.h"
#include "generated/states/types/DisplayDiagnosticsState.h"
#include "generated/states/types/FanStatusState.h"
#include "generated/states/types/FanSampleState.h"
#include "generated/states/types/FermentationStatusState.h"
#include "generated/states/types/HeatpadStatusState.h"
#include "generated/states/types/HeatpadSampleState.h"
#include "generated/states/types/HeatpadProgressState.h"
#include "generated/states/types/TemperatureStatusState.h"
#include "generated/states/types/TemperatureSampleState.h"
#include "generated/states/types/ActiveBehaviourState.h"
#include "generated/states/types/ActiveScreenState.h"

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
    HeatpadProgressState& getHeatpadProgress();
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
    HeatpadProgressState mHeatpadProgress;
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