#include "ControllerAbs.h"

#include "core/assert/Assert.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/EventForwarder.h"

namespace Garbox {

ControllerAbs::ControllerAbs(ComponentId id, const RuntimeContext& context): 
    // init members    
    mComponentDescriptor{ComponentType::Controller, id},
    mContext(context){
    // constructor body
}

ControllerAbs::~ControllerAbs(){
    TriggerExit("ControllerAbs", "controllers must not be destroyed");
}

void ControllerAbs::init(EventFactory& factory, EventForwarder& forwarder){
    AssertExit(!mInitialized, "ControllerAbs", "already initialized");
    mEventFactory = &factory;
    mEventForwarder = &forwarder;
    onInit();
    mInitialized = true;
}

void ControllerAbs::start(){
    if(!mInitialized){
        TriggerExit("ControllerAbs", "not initialized");
    }
    onStart();
}

void ControllerAbs::sendEvent(EventHeader* header){
    if(!mInitialized){
        TriggerExit("ControllerAbs", "not initialized");
    }
    mEventForwarder->forward(header);
}

bool ControllerAbs::isInitialized() const {
    return mInitialized;
}

ComponentId ControllerAbs::getComponentId() const {
    return mComponentDescriptor.id;
}

const RuntimeContext& ControllerAbs::getContext() const {
    return mContext;
}

} // namespace