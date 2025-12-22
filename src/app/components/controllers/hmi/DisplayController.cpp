#include "DisplayController.h"

#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"
#include "core/time/TimeLiterals.h"
#include "core/util/function/default/EasingFunctions.h"
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

    // update display brightness
    if(mBacklightFader.isActive()){
        float brightness = mBacklightFader.updateValue();
        mDisplay.setBrightness(brightness);
        states().displayStatus.setBrightness(mDisplay.getBrightness());
    }

    // check if display is ready to render the next frame
    if(mDisplay.tryTakeRenderReady()){
        getHost()->requestRenderScreenNow();
        mDisplay.giveRenderTrigger();
    }
    else {
        mRenderSkippedCount++;
        states().displayDiagnostics.setSkippedFrames(mRenderSkippedCount);
        LogDebug("DisplayController", "render skipped! total skip count = %" PRIi32, mRenderSkippedCount);
    }
}

void DisplayController::onDisplayCommandEvent(const DisplayCommandEvent& event){
    setBrightnessSmooth(event->brightness, 1000_ms);
};

void DisplayController::setBrightnessSmooth(float targetBrightness, uint32_t durationMicros){
    const float startBrightness = mDisplay.getBrightness();
    mBacklightFader.start(startBrightness, targetBrightness, durationMicros);
}

} // namespace