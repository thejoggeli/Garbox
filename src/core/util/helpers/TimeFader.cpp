#include "TimeFader.h"

#include "core/assert/Assert.h"
#include "core/time/Time.h"
#include "core/util/function/default/EasingFunctions.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {



TimeFader::TimeFader(): 
    mEasingFunction(&EasingFunctions::GetOutSine()) {}

TimeFader::TimeFader(const MathFunctionIfc& easingFunction):
    mEasingFunction(&easingFunction) {}

void TimeFader::setEasingFunction(const MathFunctionIfc& fn){
    mEasingFunction = &fn;
}

void TimeFader::setClampEnabled(bool enabled){
    mClampEnabled = enabled;
}

void TimeFader::start(float targetValue, uint32_t durationMicros){
    mStartValue = mCurrentValue;
    mTargetValue = targetValue;
    mDurationMicros = durationMicros;
    mStartTimeMicros = Time::GetMicros();
    mActive = true;
    mFinished = false;
}

void TimeFader::start(float startValue, float targetValue, uint32_t durationMicros){
    mStartValue = startValue;
    mTargetValue = targetValue;
    mDurationMicros = durationMicros;
    mStartTimeMicros = Time::GetMicros();
    mActive = true;
    mFinished = false;
}

float TimeFader::updateValue(){
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
        if(mStartValue < mTargetValue){
            mCurrentValue = MathUtils::Clamp(mappedValue, mStartValue, mTargetValue);
        }
        else {
            mCurrentValue = MathUtils::Clamp(mappedValue, mTargetValue, mStartValue);
        }
    }

    return mCurrentValue;
}

bool TimeFader::isActive() const{
    return mActive;
}

bool TimeFader::isFinished() const{
    return mFinished;
}

float TimeFader::getCurrentValue() const{
    return mCurrentValue;
}

float TimeFader::getTargetValue() const{
    return mTargetValue;
}

} // namespace
