#include "PidControl.h"

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
}

void PidControl::reset(){
    mOutput = 0.0f;
    mIntegral = 0.0f;
    mPrevError = 0.0f;
    mFirst = true;
}

float PidControl::step(float measured, float target, float dtSeconds){

    // compute error
    const float error = target - measured;

    // store old integral for anti windup
    const float oldIntegral = mIntegral;

    // integrate
    mIntegral += error * dtSeconds;

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
    float u = (mKp * error + mKi * mIntegral + mKd * derivative);

    // saturate output
    if(u > mMaxOutput){
        u = mMaxOutput;
    }
    else if(u < mMinOutput){
        u = mMinOutput;
    }

    // anti windup
    const bool saturated = (u == mMinOutput) || (u == mMaxOutput);
    if(saturated){
        if(error > 0.0f){
            mIntegral = oldIntegral;
        }
        else if(error < 0.0f){
            mIntegral = oldIntegral;
        }
    }

    mOutput = u;
    return mOutput;
}

} // namespace Garbox
