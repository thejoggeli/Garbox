#include "PidControl.h"

#include <cmath>

#include "core/assert/Assert.h"

namespace Garbox {

PidControl::PidControl(
    float Kp,
    float Ki,
    float Kd,
    float minOutput,
    float maxOutput
):
    // initialize members
    mKp(Kp),
    mKi(Ki),
    mKd(Kd),
    mMinOutput(minOutput),
    mMaxOutput(maxOutput)
{
    // constructor body
    AssertExit(std::isfinite(Kp) && std::isfinite(Ki) && std::isfinite(Kd), "PidControl", "gains must be finite");
    AssertExit(std::isfinite(minOutput) && std::isfinite(maxOutput), "PidControl", "limits must be finite");
    AssertExit(minOutput <= maxOutput, "PidControl", "invalid output limits");
}

void PidControl::reset(){
    mOutput = 0.0f;
    mIntegral = 0.0f;
    mPrevError = 0.0f;
    mFirst = true;
}

float PidControl::step(float measured, float target, float dtSeconds){

    if(!std::isfinite(measured)
        || !std::isfinite(target)
        || !std::isfinite(dtSeconds)
        || dtSeconds <= 0.0f){
        TriggerDebug("PidControl", "invalid step input");
        return mOutput;
    }

    // compute error
    const float error = target - measured;

    // store old integral for anti windup
    const float oldIntegral = mIntegral;

    // Integrate tentatively. Conditional integration below keeps only updates
    // that are not pushing farther into an active output limit.
    const float candidateIntegral = mIntegral + error * dtSeconds;

    // derivative
    float derivative = 0.0f;
    if(mFirst){
        derivative = 0.0f;
        mFirst = false;
    }
    else {
        derivative = (error - mPrevError) / dtSeconds;
    }

    mPrevError = error;

    // compute raw output
    const float rawOutput = mKp * error + mKi * candidateIntegral + mKd * derivative;
    if(!std::isfinite(rawOutput) || !std::isfinite(candidateIntegral)){
        TriggerDebug("PidControl", "non-finite control result");
        return mOutput;
    }
    float u = rawOutput;

    // saturate output
    if(u > mMaxOutput){
        u = mMaxOutput;
    }
    else if(u < mMinOutput){
        u = mMinOutput;
    }

    // anti windup
    const bool pushesFurtherHigh = rawOutput > mMaxOutput && error > 0.0f;
    const bool pushesFurtherLow = rawOutput < mMinOutput && error < 0.0f;
    mIntegral = (pushesFurtherHigh || pushesFurtherLow) ? oldIntegral : candidateIntegral;

    mOutput = u;
    return mOutput;
}

} // namespace Garbox
