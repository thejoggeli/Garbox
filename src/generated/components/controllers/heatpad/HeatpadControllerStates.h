// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "generated/states/types/HeatpadStatusState.h"
#include "generated/states/types/HeatpadSampleState.h"
#include "generated/states/types/HeatpadProgressState.h"

namespace Garbox {

class HeatpadControllerStates final {
public:

    HeatpadControllerStates(
        HeatpadStatusState& heatpadStatusState, // write
        HeatpadSampleState& heatpadSampleState, // write
        HeatpadProgressState& heatpadProgressState // write
    );

    // disallow copy and move
    HeatpadControllerStates(const HeatpadControllerStates&) = delete;
    HeatpadControllerStates& operator=(const HeatpadControllerStates&) = delete;
    HeatpadControllerStates(HeatpadControllerStates&&) = delete;
    HeatpadControllerStates& operator=(HeatpadControllerStates&&) = delete;

    // writable states
    HeatpadStatusState& heatpadStatus;
    HeatpadSampleState& heatpadSample;
    HeatpadProgressState& heatpadProgress;

};

} // namespace