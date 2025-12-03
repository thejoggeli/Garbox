#include "DebugScreen.h"

#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"

namespace Garbox {

DebugScreen::DebugScreen() : DebugScreenAbs(PartsProvider::GetLvglContext()) {
    // nothing to do
}

void DebugScreen::onInit(){
    setBackgroundColor(0xFF7700);
}

void DebugScreen::onStart(){
    // nothing to do
}

void DebugScreen::onBecomeEnabled(){
    // nothing to do
}

void DebugScreen::onBecomeDisabled(){
    // nothing to do
}

void DebugScreen::onUpdateScreen(){
    // nothing to do
}

void DebugScreen::onHeartbeat(const HeartbeatEvent& event){
    // nothing to do
}

} // namespace Garbox