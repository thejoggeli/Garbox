#include "DebugScreen.h"

#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"
#include "core/lvgl/LvglContext.h"

namespace Garbox {

DebugScreen::DebugScreen() : DebugScreenAbs(), mContext(PartsProvider::GetLvglContext()) {
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
        mContext.setBackgroundColor(0xFF7700);
    }
    mFirstUpdate = false;
}

void DebugScreen::onHeartbeat(const HeartbeatEvent& event){
    // nothing to do
}

} // namespace Garbox