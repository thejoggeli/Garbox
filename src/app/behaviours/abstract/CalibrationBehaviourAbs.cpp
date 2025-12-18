// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "CalibrationBehaviourAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

CalibrationBehaviourAbs::CalibrationBehaviourAbs() : BehaviourAbs(ComponentId::CalibrationBehaviour, BehaviourId::Calibration){
    // nothing to do
}

FanCommandEvent CalibrationBehaviourAbs::makeFanCommandEvent(){
    return ComponentAbs::makeEvent<EventType::FanCommand>();
}

void CalibrationBehaviourAbs::sendEvent(const FanCommandEvent& event){
    sendEventToHost(event.header());
}

const FanStatusState& CalibrationBehaviourAbs::readState(FanStatusType type){
    return *mFanStatusState;
}

const FanSampleState& CalibrationBehaviourAbs::readState(FanSampleType type){
    return *mFanSampleState;
}

void CalibrationBehaviourAbs::injectReadableState(const FanStatusState* state){
    mFanStatusState = state;
}

void CalibrationBehaviourAbs::injectReadableState(const FanSampleState* state){
    mFanSampleState = state;
}

} // namespace Garbox