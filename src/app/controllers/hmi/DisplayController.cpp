#include "DisplayController.h"

#include <esp_heap_caps.h>
#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"
#include "core/util/function/default/EasingFunctions.h"
#include "core/util/math/MathUtils.h"
#include "modules/parts/display/Display.h"

namespace Garbox {

DisplayController::DisplayController(): 
    // init members
    DisplayControllerAbs(),
    mDisplay(PartsProvider::GetDisplay()){
    // nothing to do
}

void DisplayController::onInit(){
    mBacklightFader.setEasingFunction(EasingFunctions::GetOutSine());
}

void DisplayController::onStart(){
    setBrightnessSmooth(0.65f, 1500_ms);
}

void DisplayController::onRenderTick(){

    bool changed = false;

    // update display brightness
    if(mBacklightFader.isActive()){
        float brightness = mBacklightFader.updateValue();
        mDisplay.setBrightness(brightness);
        changed = true;
    }

    // check if display is ready to render the next frame
    if(mDisplay.tryTakeRenderReady()){
        getHost()->requestUpdateScreenNow();
        mDisplay.giveRenderTrigger();
    }
    else {
        mRenderSkippedCount++;
        changed = true;
        LogDebug("DisplayController", "render skipped! total skip count = %" PRIi32, mRenderSkippedCount);
    }

    // send display status event if something changed
    if(changed){
        DisplayStatusEvent event = makeDisplayStatusEvent();
        event->brightness = mDisplay.getBrightness();
        event->skipped = mRenderSkippedCount;
        sendEvent(event);
    }
}

void DisplayController::onDisplayCommand(const DisplayCommandEvent& event){
    setBrightnessSmooth(event->brightness, 1000_ms);
};

void DisplayController::setBrightnessSmooth(float targetBrightness, uint32_t durationMicros){
    const float startBrightness = mDisplay.getBrightness();
    mBacklightFader.start(startBrightness, targetBrightness, durationMicros);
}

} // namespace