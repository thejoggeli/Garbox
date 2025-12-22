// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "generated/states/types/FermentationStatusState.h"
#include "generated/states/types/FanSampleState.h"
#include "generated/states/types/FanStatusState.h"
#include "generated/states/types/TemperatureSampleState.h"
#include "generated/states/types/TemperatureStatusState.h"

namespace Garbox {

class FermentationBehaviourStates final {
public:

    FermentationBehaviourStates(
        FermentationStatusState& fermentationStatusState, // write
        const FanSampleState& fanSampleState, // read
        const FanStatusState& fanStatusState, // read
        const TemperatureSampleState& temperatureSampleState, // read
        const TemperatureStatusState& temperatureStatusState // read
    );

    // disallow copy and move
    FermentationBehaviourStates(const FermentationBehaviourStates&) = delete;
    FermentationBehaviourStates& operator=(const FermentationBehaviourStates&) = delete;
    FermentationBehaviourStates(FermentationBehaviourStates&&) = delete;
    FermentationBehaviourStates& operator=(FermentationBehaviourStates&&) = delete;

    // writable states
    FermentationStatusState& fermentationStatus;

    // readable states
    const FanSampleState& fanSample;
    const FanStatusState& fanStatus;
    const TemperatureSampleState& temperatureSample;
    const TemperatureStatusState& temperatureStatus;

};

} // namespace