// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "DebugScreenStates.h"

namespace Garbox {

DebugScreenStates::DebugScreenStates(
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
):
    fanStatus(fanStatusState),
    fanSample(fanSampleState),
    heatpadStatus(heatpadStatusState),
    heatpadSample(heatpadSampleState),
    heatpadProgress(heatpadProgressState),
    displayStatus(displayStatusState),
    displayDiagnostics(displayDiagnosticsState),
    temperatureStatus(temperatureStatusState),
    temperatureSample(temperatureSampleState),
    activeBehaviour(activeBehaviourState),
    activeScreen(activeScreenState),
    fermentationStatus(fermentationStatusState){
}

} // namespace