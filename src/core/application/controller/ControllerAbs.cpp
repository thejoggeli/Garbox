#include "ControllerAbs.h"

#include "core/assert/Assert.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/EventForwarder.h"

namespace Garbox {

ControllerAbs::ControllerAbs(ComponentId id): 
    // init members    
    mComponentDescriptor{ComponentType::Controller, id} {
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

void ControllerAbs::sendEvent(Event* event){
    if(!mInitialized){
        TriggerExit("ControllerAbs", "not initialized");
    }
    mEventForwarder->forward(event);
}

bool ControllerAbs::isInitialized() const {
    return mInitialized;
}

ComponentId ControllerAbs::getComponentId() const {
    return mComponentDescriptor.id;
}

} // namespace