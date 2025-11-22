// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "CalibrationBehaviour.h"

#include "app/providers/PartsProvider.h"
#include "modules/parts/piezo/PiezoPlayer.h"

namespace Garbox {

CalibrationBehaviour::CalibrationBehaviour():
    // init members
    CalibrationBehaviourAbs(){
    // nothing to do
}

void CalibrationBehaviour::onInit(){
    // nothing to do
}

void CalibrationBehaviour::onStart(){
    // nothing to do
}

void CalibrationBehaviour::onBecomeActive(){
    // nothing to do
}

void CalibrationBehaviour::onBecomeInactive(){
    // nothing to do
}

void CalibrationBehaviour::onLogicTick(){
    // nothing to do
}

void CalibrationBehaviour::onHeartbeat(const Heartbeat& event){
    static PiezoPlayer& piezo = PartsProvider::GetPiezoPlayer(); 
    if(getContext()->tickCount > 10*30){
        getHost()->requestChangeBehaviour(BehaviourId::Fermentation);
        piezo.playTone(Tone(400_ms).sweep(1000, 2500));
    }
    else {
        piezo.playTone(Tone(125_ms).sweep(1000, 1500), 125_ms);
        piezo.playTone(Tone(125_ms).sweep(1250, 2000));
    }
}

void CalibrationBehaviour::onFanStatus(const FanStatus& event){
    // nothing to do
}

void CalibrationBehaviour::onFanSample(const FanSample& event){
    // nothing to do
}

} // namespace Garbox