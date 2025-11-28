#include "ScreenAbs.h"

#include "core/assert/Assert.h"

namespace Garbox {

ScreenAbs::ScreenAbs(ComponentId componentId, ScreenId ScreenId): 
    // init memberes
    ComponentAbs(ComponentType::Screen, componentId),
    mScreenId(ScreenId){
    // nothing to do
}

void ScreenAbs::updateScreen(){
    onUpdateScreen();
}

void ScreenAbs::setActive(bool active){
    if(!isInitialized()){
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

} // namespace
