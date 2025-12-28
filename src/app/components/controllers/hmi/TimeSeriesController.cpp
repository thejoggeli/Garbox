#include "TimeSeriesController.h"
// This file was initially generated and is no longer modified by the generator.
// Interface changes must be applied manually in this file.
// See the corresponding header for the interface method declarations.

namespace Garbox {

TimeSeriesController::TimeSeriesController() : 
    TimeSeriesControllerAbs(),
    mHistory(GarboxHistory::Instance()){}

void TimeSeriesController::onInit(){
    // to be implemented
}

void TimeSeriesController::onStart(){
    mHistory.reset();
}

void TimeSeriesController::onActorWriteTick(){
    // add sample
    if(mStarted){
        float measuredTemperature = states().temperatureSample.getTemperatureCelcius();
        float targetTemperature = states().fermentationStatus.getTargetTemperature();
        float power = states().heatpadStatus.getNextDutyCycle() * 100.0f;
        mHistory.measuredTemperatureSample(measuredTemperature * GarboxHistory::TemperaturScaleFactor);
        mHistory.targetTemperatureSample(targetTemperature * GarboxHistory::TemperaturScaleFactor);
        mHistory.powerSample(power * GarboxHistory::PowerScaleFactor);
    }
}

void TimeSeriesController::onTemperatureStatusStateChanged(const TemperatureStatusState& state){
    if(state.getHasFirstSample()){
        mStarted = true;
    }
}

void TimeSeriesController::onHeatpadStatusStateChanged(const HeatpadStatusState& state){}
void TimeSeriesController::onTemperatureSampleStateChanged(const TemperatureSampleState& state){}
void TimeSeriesController::onFermentationStatusStateChanged(const FermentationStatusState& state){};

} // namespace Garbox