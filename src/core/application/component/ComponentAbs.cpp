#include "ComponentAbs.h"

#include "core/application/component/ComponentAbs.h"
#include "core/assert/Assert.h"

namespace Garbox {

ComponentAbs::ComponentAbs(ComponentType type, ComponentId id) : mComponentDescriptor{type, id}{
    // nothing to do
}

ComponentAbs::~ComponentAbs(){
    TriggerExit("ComponentAbs", "components must not be destroyed");
}

void ComponentAbs::init(){
    AssertExit(!mInitialized, "ComponentAbs", "already initialized");
    onInit();
    mInitialized = true;
}

void ComponentAbs::start(){
    if(!mInitialized){
        TriggerExit("ComponentAbs", "not initialized");
    }
    onStart();
}

void ComponentAbs::setComponentHost(ComponentHostIfc& host){
    mHost = &host;
    mContext = &host.getContext();
    mEventFactory = &host.getEventFactory();
}

void ComponentAbs::publishEvent(const EventHeader* header){
    if(!mInitialized){
        TriggerExit("BehaviourAbs", "not initialized");
    }
    mHost->publishEvent(header);
}

} // namespace
