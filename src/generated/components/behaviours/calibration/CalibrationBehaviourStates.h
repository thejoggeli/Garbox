// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "generated/states/types/FanStatusState.h"
#include "generated/states/types/FanSampleState.h"

namespace Garbox {

class CalibrationBehaviourStates final {
public:

    CalibrationBehaviourStates(
        const FanStatusState& fanStatusState, // read
        const FanSampleState& fanSampleState // read
    );

    // disallow copy and move
    CalibrationBehaviourStates(const CalibrationBehaviourStates&) = delete;
    CalibrationBehaviourStates& operator=(const CalibrationBehaviourStates&) = delete;
    CalibrationBehaviourStates(CalibrationBehaviourStates&&) = delete;
    CalibrationBehaviourStates& operator=(CalibrationBehaviourStates&&) = delete;

    // readable states
    const FanStatusState& fanStatus;
    const FanSampleState& fanSample;

};

} // namespace