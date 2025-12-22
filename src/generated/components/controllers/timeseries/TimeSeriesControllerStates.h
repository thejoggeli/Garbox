// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "generated/states/types/HeatpadStatusState.h"
#include "generated/states/types/TemperatureStatusState.h"
#include "generated/states/types/TemperatureSampleState.h"

namespace Garbox {

class TimeSeriesControllerStates final {
public:

    TimeSeriesControllerStates(
        const HeatpadStatusState& heatpadStatusState, // read
        const TemperatureStatusState& temperatureStatusState, // read
        const TemperatureSampleState& temperatureSampleState // read
    );

    // disallow copy and move
    TimeSeriesControllerStates(const TimeSeriesControllerStates&) = delete;
    TimeSeriesControllerStates& operator=(const TimeSeriesControllerStates&) = delete;
    TimeSeriesControllerStates(TimeSeriesControllerStates&&) = delete;
    TimeSeriesControllerStates& operator=(TimeSeriesControllerStates&&) = delete;

    // readable states
    const HeatpadStatusState& heatpadStatus;
    const TemperatureStatusState& temperatureStatus;
    const TemperatureSampleState& temperatureSample;

};

} // namespace