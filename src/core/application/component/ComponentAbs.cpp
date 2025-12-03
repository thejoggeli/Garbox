#include "ComponentAbs.h"

#include "core/application/component/ComponentAbs.h"
#include "core/assert/Assert.h"
#include "core/log/Log.h"

namespace Garbox {

ComponentAbs::ComponentAbs(ComponentType type, ComponentId id) : mComponentDescriptor{type, id}{
    LogDebug("ComponentAbs", "ctor id=%s", ComponentIdToString(id));
}

ComponentAbs::~ComponentAbs(){
    TriggerExit("ComponentAbs", "components must not be destroyed");
}

void ComponentAbs::init(ComponentHostIfc& host){
    AssertExit(!mInitialized, "ComponentAbs", "already initialized");
    mHost = &host;
    mContext = &host.getContext();
    mEventFactory = &host.getEventFactory();
    onInit();
    mInitialized = true;
}

void ComponentAbs::start(){
    if(!mInitialized){
        TriggerExit("ComponentAbs", "not initialized");
    }
    onStart();
}

void ComponentAbs::setEnabled(bool enabled){
    if(!mInitialized){
        TriggerExit("ComponentAbs", "not initialized");
    }
    if(enabled == mEnabled){
        TriggerDebug("ComponentAbs", "already in enabled state", enabled);
        return;
    }
    mEnabled = enabled;
    if(enabled){
        becomeEnabled();
    }
    else {
        becomeDisabled();
    }
}

void ComponentAbs::becomeEnabled(){
    onBecomeEnabled();
}

void ComponentAbs::becomeDisabled(){
    onBecomeDisabled();
}

void ComponentAbs::publishEvent(const EventHeader* header){
    if(!mInitialized){
        TriggerExit("ComponentAbs", "not initialized");
    }
    mHost->publishEvent(header);
}

} // namespace
