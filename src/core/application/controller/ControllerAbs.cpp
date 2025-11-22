#include "ControllerAbs.h"

#include "core/assert/Assert.h"

namespace Garbox {

ControllerAbs::ControllerAbs(ComponentId id): 
    // init members    
    mComponentDescriptor{ComponentType::Controller, id}{
    // constructor body
}

ControllerAbs::~ControllerAbs(){
    TriggerExit("ControllerAbs", "controllers must not be destroyed");
}

void ControllerAbs::init(ControllerHostIfc& host){
    AssertExit(!mInitialized, "ControllerAbs", "already initialized");
    mHost = &host;
    mEventFactory = &mHost->getEventFactory();
    mContext = &mHost->getContext();
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
    mHost->publishEvent(header);
}

bool ControllerAbs::isInitialized() const {
    return mInitialized;
}

ComponentId ControllerAbs::getComponentId() const {
    return mComponentDescriptor.id;
}

const RuntimeContext* ControllerAbs::getContext() const {
    return mContext;
}

ControllerHostIfc* ControllerAbs::getHost(){
    return mHost;
}

} // namespace