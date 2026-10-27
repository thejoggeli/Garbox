// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "FermentationBehaviourStates.h"

namespace Garbox {

FermentationBehaviourStates::FermentationBehaviourStates(
    FermentationStatusState& fermentationStatusState, // write
    const FanSampleState& fanSampleState, // read
    const FanStatusState& fanStatusState, // read
    const HeatpadSampleState& heatpadSampleState, // read
    const TemperatureSampleState& temperatureSampleState, // read
    const TemperatureStatusState& temperatureStatusState // read
):
    fermentationStatus(fermentationStatusState),
    fanSample(fanSampleState),
    fanStatus(fanStatusState),
    heatpadSample(heatpadSampleState),
    temperatureSample(temperatureSampleState),
    temperatureStatus(temperatureStatusState){
}

} // namespace