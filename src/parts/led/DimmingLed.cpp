#include "DimmingLed.h"

#include "assert/Assert.h"
#include "core/hardware/ledc/LedcChannel.h"
#include "global/util/FunctionInstances.h"
#include "util/math/MathUtils.h"

namespace Garbox {

DimmingLed::DimmingLed(LedcChannel& ledcChannel):
    // initialize members    
    mLedcChannel(ledcChannel){
    // constructor body
}

void DimmingLed::init(){
    AssertExit(!mInitialized, "AnimatedLed", "already initialized");
    
    // set default correction function
    if(mCorrectionFunction == nullptr){
        setCorrectionFunction(FunctionInstances::GetGamma22Sampled());
    }

    mInitialized = true;
}

void DimmingLed::setCorrectionFunction(const FunctionIfc& function){
    mCorrectionFunction = &function;
}

void DimmingLed::setBrightness(float brightness){
    if(!mInitialized){
        TriggerDebug("DimmingLed", "not initialized");
        return;
    }

    mBrightness = MathUtils::Clamp(brightness, 0.0f, 1.0f);
    
    // brightness correction
    if(mCorrectionFunction == nullptr){
        TriggerDebug("DimmingLed", "no smoothing function");
        mBrightnessCorrected = mBrightness;
    }
    else {
        mBrightnessCorrected = mCorrectionFunction->evaluate(mBrightness);
    }

    // set pwm duty cycle to corrected brightness
    mLedcChannel.setDutyRelative(mBrightnessCorrected);
}

float DimmingLed::getBrightness() const {
    return mBrightness;
}

float DimmingLed::getBrightnessCorrected() const {
    return mBrightnessCorrected;
}

} // namespace