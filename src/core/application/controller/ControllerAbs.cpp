#include "ControllerAbs.h"

#include "core/assert/Assert.h"

namespace Garbox {

ControllerAbs::ControllerAbs(ComponentId componentId, ControllerId controllerId): 
    // init memberes
    ComponentAbs(ComponentType::Controller, componentId),
    mControllerId(controllerId){
    // nothing to do
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

} // namespace