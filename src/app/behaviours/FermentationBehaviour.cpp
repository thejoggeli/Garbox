#include "FermentationBehaviour.h"

#include "core/time/TimeLiterals.h"

namespace Garbox {

FermentationBehaviour::FermentationBehaviour(ComponentId id) : AppBehaviourAbs(id) {
    // nothing to do
}

void FermentationBehaviour::onInit(){
    // nothing to do
}

void FermentationBehaviour::onStart(){
    // nothing to do
}

void FermentationBehaviour::onBecomeActive(){
    // nothing to do
}

void FermentationBehaviour::onBecomeInactive(){
    // nothing to do
}

void FermentationBehaviour::onLogicTick(){
    if(mHeartbeatReceived){
        constexpr static uint32_t SwitchStatesCount = 11;
        mSwitchState++;
        if(mSwitchState >= SwitchStatesCount){
            mSwitchState = 0;
        }
        applySwitchState();
        mHeartbeatReceived = false;
    }
}

void FermentationBehaviour::onHeartbeat(const EventView<EventData::Heartbeat>& event){
    mHeartbeatReceived = true;
}

void FermentationBehaviour::applySwitchState(){
    switch(mSwitchState){
    case  0: sendFanCommand(false, 0.0f); break;
    case  1: break; // stay
    case  2: sendFanCommand(true, 0.4f); break;
    case  3: sendFanCommand(true, 0.6f); break;
    case  4: sendFanCommand(true, 0.8f); break;
    case  5: sendFanCommand(true, 1.0f); break;
    case  6: break; // stay
    case  7: break; // stay
    case  8: sendFanCommand(true, 0.8f); break;
    case  9: sendFanCommand(true, 0.6f); break;
    case 10: sendFanCommand(true, 0.4f); break;
    default:
        TriggerDebug("FanController", "unhandled fan state", mSwitchState);
    }
}

void FermentationBehaviour::sendFanCommand(bool enabled, float speed){
    EventWrapper wrapper = makeEvent<EventData::FanCommand>();
    wrapper.data->enabled = enabled;
    wrapper.data->targetSpeed = speed;
    sendEvent(wrapper.event);
}

} // namespace
