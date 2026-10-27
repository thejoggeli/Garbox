#include "FermentationSafetySupervisor.h"

#include <cmath>

namespace Garbox {

void FermentationSafetySupervisor::update(const Input& input){
    const bool finiteTemperature = std::isfinite(input.temperatureCelcius);
    const bool finiteHumidity = std::isfinite(input.humidityRelative);
    const bool sampleTimeValid = input.sampleMicros <= input.nowMicros;
    const bool sampleFresh = sampleTimeValid
        && (input.nowMicros - input.sampleMicros) <= MaxSampleAgeMicros;
    const bool samplePlausible = finiteTemperature
        && input.temperatureCelcius >= MinPlausibleTemperatureCelcius
        && input.temperatureCelcius <= MaxPlausibleTemperatureCelcius
        && finiteHumidity
        && input.humidityRelative >= MinPlausibleHumidityRelative
        && input.humidityRelative <= MaxPlausibleHumidityRelative;

    mSensorValid = input.sensorAvailable && sampleFresh && samplePlausible;
    mTransientFault = SafetyFault::None;
    if(input.regulationRequested){
        if(!input.sensorAvailable){
            mTransientFault = SafetyFault::SensorUnavailable;
        }
        else if(!sampleFresh){
            mTransientFault = SafetyFault::SensorStale;
        }
        else if(!samplePlausible){
            mTransientFault = SafetyFault::SensorImplausible;
        }
    }

    if(finiteTemperature
        && input.temperatureCelcius >= AbsoluteTemperatureLimitCelcius){
        latch(SafetyFault::AbsoluteOverTemperature);
    }

    const bool measurementFinite = std::isfinite(input.heaterMeasuredVoltage)
        && std::isfinite(input.heaterMeasuredCurrent);
    const bool unexpectedOutput = measurementFinite
        && !mLastHeaterCommandedOn
        && (input.heaterMeasuredVoltage >= UnexpectedHeaterVoltageThreshold
            || input.heaterMeasuredCurrent >= UnexpectedHeaterCurrentThreshold);
    if(unexpectedOutput){
        if(!mUnexpectedOutputTiming){
            mUnexpectedOutputTiming = true;
            mUnexpectedOutputStartMicros = input.nowMicros;
        }
        else if(input.nowMicros - mUnexpectedOutputStartMicros
            >= UnexpectedHeaterPersistenceMicros){
            latch(SafetyFault::UnexpectedHeaterOutput);
        }
    }
    else {
        mUnexpectedOutputTiming = false;
    }

    mActiveFault = mLatchedFault != SafetyFault::None
        ? mLatchedFault
        : mTransientFault;
}

bool FermentationSafetySupervisor::permitHeater(
    bool requestedEnabled,
    float requestedDuty){
    if(!std::isfinite(requestedDuty) || requestedDuty < 0.0f || requestedDuty > 1.0f){
        mTransientFault = SafetyFault::InvalidHeaterCommand;
        if(mLatchedFault == SafetyFault::None){
            mActiveFault = mTransientFault;
        }
        return false;
    }
    return requestedEnabled
        && requestedDuty > 0.0f
        && mSensorValid
        && mActiveFault == SafetyFault::None;
}

void FermentationSafetySupervisor::recordFinalHeaterCommand(bool enabled, float duty){
    mLastHeaterCommandedOn = enabled && std::isfinite(duty) && duty > 0.0f;
}

void FermentationSafetySupervisor::latch(SafetyFault fault){
    if(mLatchedFault == SafetyFault::None){
        mLatchedFault = fault;
    }
}

} // namespace Garbox
