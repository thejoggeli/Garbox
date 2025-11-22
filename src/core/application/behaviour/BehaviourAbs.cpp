#include "BehaviourAbs.h"

#include "core/assert/Assert.h"

namespace Garbox {

BehaviourAbs::BehaviourAbs(ComponentId componentId, BehaviourId behaviourId): 
    // init memberes
    ComponentAbs(ComponentType::Behaviour, componentId),
    mBehaviourId(behaviourId){
    // nothing to do
}

BehaviourAbs::~BehaviourAbs(){
    TriggerExit("BehaviourAbs", "behaviours must not be destroyed");
}

void BehaviourAbs::init(BehaviourHostIfc& host){
    AssertExit(!mInitialized, "BehaviourAbs", "already initialized");
    mHost = &host;
    mContext = &host.getContext();
    mEventFactory = &host.getEventFactory();
    onInit();
    mInitialized = true;
}

void BehaviourAbs::start(){
    if(!mInitialized){
        TriggerExit("BehaviourAbs", "not initialized");
    }
    onStart();
}

void BehaviourAbs::sendEvent(EventHeader* header){
    if(!mInitialized){
        TriggerExit("BehaviourAbs", "not initialized");
    }
    mHost->publishEvent(header);
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

} // namespace
