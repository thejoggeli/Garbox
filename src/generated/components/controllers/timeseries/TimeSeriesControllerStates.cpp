// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "TimeSeriesControllerStates.h"

namespace Garbox {

TimeSeriesControllerStates::TimeSeriesControllerStates(
    const HeatpadStatusState& heatpadStatusState, // read
    const TemperatureStatusState& temperatureStatusState, // read
    const TemperatureSampleState& temperatureSampleState, // read
    const FermentationStatusState& fermentationStatusState // read
):
    heatpadStatus(heatpadStatusState),
    temperatureStatus(temperatureStatusState),
    temperatureSample(temperatureSampleState),
    fermentationStatus(fermentationStatusState){
}

} // namespace