// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "generated/states/types/FanStatusState.h"
#include "generated/states/types/FanSampleState.h"
#include "generated/states/types/HeatpadStatusState.h"
#include "generated/states/types/HeatpadSampleState.h"
#include "generated/states/types/TemperatureStatusState.h"
#include "generated/states/types/TemperatureSampleState.h"
#include "generated/states/types/FermentationStatusState.h"

namespace Garbox {

class SimpleScreenStates final {
public:

    SimpleScreenStates(
        const FanStatusState& fanStatusState, // read
        const FanSampleState& fanSampleState, // read
        const HeatpadStatusState& heatpadStatusState, // read
        const HeatpadSampleState& heatpadSampleState, // read
        const TemperatureStatusState& temperatureStatusState, // read
        const TemperatureSampleState& temperatureSampleState, // read
        const FermentationStatusState& fermentationStatusState // read
    );

    // disallow copy and move
    SimpleScreenStates(const SimpleScreenStates&) = delete;
    SimpleScreenStates& operator=(const SimpleScreenStates&) = delete;
    SimpleScreenStates(SimpleScreenStates&&) = delete;
    SimpleScreenStates& operator=(SimpleScreenStates&&) = delete;

    // readable states
    const FanStatusState& fanStatus;
    const FanSampleState& fanSample;
    const HeatpadStatusState& heatpadStatus;
    const HeatpadSampleState& heatpadSample;
    const TemperatureStatusState& temperatureStatus;
    const TemperatureSampleState& temperatureSample;
    const FermentationStatusState& fermentationStatus;

};

} // namespace