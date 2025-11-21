#include "SmoothTransition.h"

#include "core/assert/Assert.h"
#include "core/time/Time.h"
#include "core/util/function/default/EasingFunctions.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {



SmoothTransition::SmoothTransition(): 
    mEasingFunction(&EasingFunctions::GetOutSine()) {}

SmoothTransition::SmoothTransition(const MathFunctionIfc& easingFunction):
    mEasingFunction(&easingFunction) {}

void SmoothTransition::setEasingFunction(const MathFunctionIfc& fn){
    mEasingFunction = &fn;
}

void SmoothTransition::setClampEnabled(bool enabled){
    mClampEnabled = enabled;
}

void SmoothTransition::start(float targetValue, uint32_t durationMicros){
    mStartValue = mCurrentValue;
    mTargetValue = targetValue;
    mDurationMicros = durationMicros;
    mStartTimeMicros = Time::GetMicros();
    mActive = true;
    mFinished = false;
}

void SmoothTransition::start(float startValue, float targetValue, uint32_t durationMicros){
    mStartValue = startValue;
    mTargetValue = targetValue;
    mDurationMicros = durationMicros;
    mStartTimeMicros = Time::GetMicros();
    mActive = true;
    mFinished = false;
}

float SmoothTransition::updateValue(){
    if(!mActive){
        return mCurrentValue;
    }

    const uint32_t nowMicros = Time::GetMicros();
    const uint32_t elapsedMicros = nowMicros - mStartTimeMicros;

    if(elapsedMicros >= mDurationMicros){
        mCurrentValue = mTargetValue;
        mActive = false;
        mFinished = true;
        return mCurrentValue;
    }

    const float fraction = (float)elapsedMicros / (float)mDurationMicros;
    const float eased = mEasingFunction->evaluate(fraction);

    const float mappedValue = MathUtils::Map(eased, 0.0f, 1.0f, mStartValue, mTargetValue);
    if(mClampEnabled){
        mCurrentValue = MathUtils::Clamp(mappedValue, mStartValue, mTargetValue);
    }

    return mCurrentValue;
}

bool SmoothTransition::isActive() const{
    return mActive;
}

bool SmoothTransition::isFinished() const{
    return mFinished;
}

float SmoothTransition::getCurrentValue() const{
    return mCurrentValue;
}

float SmoothTransition::getTargetValue() const{
    return mTargetValue;
}

} // namespace
