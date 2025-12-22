// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "generated/states/types/FanStatusState.h"
#include "generated/states/types/FanSampleState.h"

namespace Garbox {

class FanControllerStates final {
public:

    FanControllerStates(
        FanStatusState& fanStatusState, // write
        FanSampleState& fanSampleState // write
    );

    // disallow copy and move
    FanControllerStates(const FanControllerStates&) = delete;
    FanControllerStates& operator=(const FanControllerStates&) = delete;
    FanControllerStates(FanControllerStates&&) = delete;
    FanControllerStates& operator=(FanControllerStates&&) = delete;

    // writable states
    FanStatusState& fanStatus;
    FanSampleState& fanSample;

};

} // namespace