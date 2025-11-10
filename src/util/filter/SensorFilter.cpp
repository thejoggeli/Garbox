#include "SensorFilter.h"

#include "assert/Assert.h"

namespace Garbox {

SensorFilter::SensorFilter(){
    // nothing to do
}

void SensorFilter::setCalibrationPoints(const Point& p1, const Point& p2){
    setCalibrationPoints(p1.x, p1.y, p2.x, p2.y);
}

void SensorFilter::setCalibrationPoints(float x1, float y1, float x2, float y2){
    AssertExit((x1 != x2), "SensorFilter", "invalid calibration points (identical x)");
    mSlope = (y2 - y1) / (x2 - x1);
    mOffset = y1 - (mSlope * x1);
    mCalibrated = true;
}

void SensorFilter::setAlpha(float alpha){
    mFilter.setAlpha(alpha);
}

void SensorFilter::setAlphaComputed(float fraction, uint32_t ticks){
    mFilter.setAlphaComputed(fraction, ticks);
}

void SensorFilter::setThreshold(float threshold){
    mFilter.setThreshold(threshold);
}

void SensorFilter::setSnapResolution(float snapResolution){
    mSnapResolution = (snapResolution > 0.0f) ? snapResolution : 0.0f;
}

void SensorFilter::update(float rawValue){
    if(!mCalibrated){
        TriggerDebug("SensorFilter", "update without calibration");
        return;
    }

    // apply calibration
    float calibratedValue = applyCalibration(rawValue);

    // optional snapping before filtering
    float snappedValue = applySnap(calibratedValue);

    // feed into filter
    mFilter.update(snappedValue);
}

float SensorFilter::getValue() const{
    return mFilter.getFilteredValue();
}

float SensorFilter::applyCalibration(float rawValue) const{
    return (mSlope * rawValue) + mOffset;
}

float SensorFilter::applySnap(float value) const{
    if(mSnapResolution <= 0.0f){
        return value;
    }
    return std::round(value / mSnapResolution) * mSnapResolution;
}

} // namespace Garbox
