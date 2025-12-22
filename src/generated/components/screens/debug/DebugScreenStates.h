// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "generated/states/types/FanStatusState.h"
#include "generated/states/types/FanSampleState.h"
#include "generated/states/types/HeatpadStatusState.h"
#include "generated/states/types/HeatpadSampleState.h"
#include "generated/states/types/HeatpadProgressState.h"
#include "generated/states/types/DisplayStatusState.h"
#include "generated/states/types/DisplayDiagnosticsState.h"
#include "generated/states/types/TemperatureStatusState.h"
#include "generated/states/types/TemperatureSampleState.h"
#include "generated/states/types/ActiveBehaviourState.h"
#include "generated/states/types/ActiveScreenState.h"
#include "generated/states/types/FermentationStatusState.h"

namespace Garbox {

class DebugScreenStates final {
public:

    DebugScreenStates(
        const FanStatusState& fanStatusState, // read
        const FanSampleState& fanSampleState, // read
        const HeatpadStatusState& heatpadStatusState, // read
        const HeatpadSampleState& heatpadSampleState, // read
        const HeatpadProgressState& heatpadProgressState, // read
        const DisplayStatusState& displayStatusState, // read
        const DisplayDiagnosticsState& displayDiagnosticsState, // read
        const TemperatureStatusState& temperatureStatusState, // read
        const TemperatureSampleState& temperatureSampleState, // read
        const ActiveBehaviourState& activeBehaviourState, // read
        const ActiveScreenState& activeScreenState, // read
        const FermentationStatusState& fermentationStatusState // read
    );

    // disallow copy and move
    DebugScreenStates(const DebugScreenStates&) = delete;
    DebugScreenStates& operator=(const DebugScreenStates&) = delete;
    DebugScreenStates(DebugScreenStates&&) = delete;
    DebugScreenStates& operator=(DebugScreenStates&&) = delete;

    // readable states
    const FanStatusState& fanStatus;
    const FanSampleState& fanSample;
    const HeatpadStatusState& heatpadStatus;
    const HeatpadSampleState& heatpadSample;
    const HeatpadProgressState& heatpadProgress;
    const DisplayStatusState& displayStatus;
    const DisplayDiagnosticsState& displayDiagnostics;
    const TemperatureStatusState& temperatureStatus;
    const TemperatureSampleState& temperatureSample;
    const ActiveBehaviourState& activeBehaviour;
    const ActiveScreenState& activeScreen;
    const FermentationStatusState& fermentationStatus;

};

} // namespace