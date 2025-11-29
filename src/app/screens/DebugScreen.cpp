// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "DebugScreen.h"

#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"
#include "modules/parts/display/Display.h"

namespace Garbox {

DebugScreen::DebugScreen() : DebugScreenAbs(), mObjects(PartsProvider::GetDisplay().getLvglHandler().getObjects()) {
    // nothing to do
}

void DebugScreen::onInit(){
    // nothing to do
}

void DebugScreen::onStart(){
    // nothing to do
}

void DebugScreen::onBecomeEnabled(){
    mFirstUpdate = true;
}

void DebugScreen::onBecomeDisabled(){
    // nothing to do
}

void DebugScreen::onUpdateScreen(){
    if(mFirstUpdate){
        mObjects.setBackgroundColor(0xFF7700);
    }
    mFirstUpdate = false;
}

void DebugScreen::onHeartbeat(const HeartbeatEvent& event){
    // nothing to do
}

} // namespace Garbox