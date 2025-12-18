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

const FanStatusState& CalibrationBehaviourAbs::stateFanStatus(){
    return *mFanStatusState;
}

const FanSampleState& CalibrationBehaviourAbs::stateFanSample(){
    return *mFanSampleState;
}

void CalibrationBehaviourAbs::injectFanStatusState(const FanStatusState* state){
    mFanStatusState = state;
}

void CalibrationBehaviourAbs::injectFanSampleState(const FanSampleState* state){
    mFanSampleState = state;
}

} // namespace Garbox