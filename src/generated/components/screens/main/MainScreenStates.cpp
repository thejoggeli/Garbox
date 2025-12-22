// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "MainScreenStates.h"

namespace Garbox {

MainScreenStates::MainScreenStates(
    const FanStatusState& fanStatusState, // read
    const FanSampleState& fanSampleState, // read
    const HeatpadStatusState& heatpadStatusState, // read
    const HeatpadSampleState& heatpadSampleState, // read
    const TemperatureStatusState& temperatureStatusState, // read
    const TemperatureSampleState& temperatureSampleState, // read
    const FermentationStatusState& fermentationStatusState // read
):
    fanStatus(fanStatusState),
    fanSample(fanSampleState),
    heatpadStatus(heatpadStatusState),
    heatpadSample(heatpadSampleState),
    temperatureStatus(temperatureStatusState),
    temperatureSample(temperatureSampleState),
    fermentationStatus(fermentationStatusState){
}

} // namespace