// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "TimeSeriesControllerStates.h"

namespace Garbox {

TimeSeriesControllerStates::TimeSeriesControllerStates(
    const HeatpadStatusState& heatpadStatusState, // read
    const TemperatureStatusState& temperatureStatusState, // read
    const TemperatureSampleState& temperatureSampleState // read
):
    heatpadStatus(heatpadStatusState),
    temperatureStatus(temperatureStatusState),
    temperatureSample(temperatureSampleState){
}

} // namespace