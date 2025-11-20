#include "BehaviourAbs.h"

#include "core/assert/Assert.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/EventForwarder.h"

namespace Garbox {

BehaviourAbs::BehaviourAbs(ComponentId id):
    // init members
    mComponentDescriptor{ComponentType::Behaviour, id} {
    // nothing to do
}

BehaviourAbs::~BehaviourAbs(){
    TriggerExit("BehaviourAbs", "behaviours must not be destroyed");
}

void BehaviourAbs::init(EventFactory& factory, EventForwarder& forwarder){
    AssertExit(!mInitialized, "BehaviourAbs", "already initialized");
    mEventFactory = &factory;
    mEventForwarder = &forwarder;
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
    mEventForwarder->forward(header);
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

bool BehaviourAbs::isActive() const {
    return mActive;
}

bool BehaviourAbs::isInitialized() const {
    return mInitialized;
}

ComponentId BehaviourAbs::getComponentId() const {
    return mComponentDescriptor.id;
}

} // namespace
