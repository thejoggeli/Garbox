#include "BehaviourAbs.h"

#include "core/assert/Assert.h"

namespace Garbox {

BehaviourAbs::BehaviourAbs(ComponentId componentId, BehaviourId behaviourId): 
    // init memberes
    ComponentAbs(ComponentType::Behaviour, componentId),
    mBehaviourId(behaviourId){
    // nothing to do
}

void BehaviourAbs::setActive(bool active){
    if(!mInitialized){
        TriggerExit("BehaviourAbs", "not initialized");
    }
    if(active == mActive){
        TriggerDebug("BehaviourAbs", "already in active state", active);
        return;
    }
    mActive = active;
    if(active){
        onBecomeActive();
    }
    else {
        onBecomeInactive();
    }
}

void BehaviourAbs::requestChangeBehaviour(BehaviourId behaviourId){
    RequestChangeBehaviourEvent event = makeEvent<EventType::RequestChangeBehaviour>();
    event->behaviour = behaviourId;
    publishEvent(event.header());
}

} // namespace
