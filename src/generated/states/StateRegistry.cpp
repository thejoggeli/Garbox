#include "StateRegistry.h"
#include "core/assert/Assert.h"

namespace Garbox {

StateRegistry::StateRegistry(){
    // nothing to do
}

// states accessors
DisplayStatusState& StateRegistry::getDisplayStatus(){
    return mDisplayStatus;
}

DisplayDiagnosticsState& StateRegistry::getDisplayDiagnostics(){
    return mDisplayDiagnostics;
}

FanStatusState& StateRegistry::getFanStatus(){
    return mFanStatus;
}

FanSampleState& StateRegistry::getFanSample(){
    return mFanSample;
}

FermentationStatusState& StateRegistry::getFermentationStatus(){
    return mFermentationStatus;
}

HeatpadStatusState& StateRegistry::getHeatpadStatus(){
    return mHeatpadStatus;
}

HeatpadSampleState& StateRegistry::getHeatpadSample(){
    return mHeatpadSample;
}

HeatpadProgressState& StateRegistry::getHeatpadProgress(){
    return mHeatpadProgress;
}

TemperatureStatusState& StateRegistry::getTemperatureStatus(){
    return mTemperatureStatus;
}

TemperatureSampleState& StateRegistry::getTemperatureSample(){
    return mTemperatureSample;
}

ActiveBehaviourState& StateRegistry::getActiveBehaviour(){
    return mActiveBehaviour;
}

ActiveScreenState& StateRegistry::getActiveScreen(){
    return mActiveScreen;
}

} // namespace Garbox