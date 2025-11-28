#include "ScreenAbs.h"

#include "core/assert/Assert.h"

namespace Garbox {

ScreenAbs::ScreenAbs(ComponentId componentId, ScreenId ScreenId): 
    // init memberes
    ComponentAbs(ComponentType::Screen, componentId),
    mScreenId(ScreenId){
    // nothing to do
}

void ScreenAbs::setActive(bool active){
    if(!mInitialized){
        TriggerExit("ScreenAbs", "not initialized");
    }
    if(active == mActive){
        TriggerDebug("ScreenAbs", "already in active state", active);
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

void ScreenAbs::requestChangeScreen(ScreenId screenId){
    RequestChangeScreenEvent event = makeEvent<EventType::RequestChangeScreen>();
    event->screen = screenId;
    publishEvent(event.header());
}

} // namespace
