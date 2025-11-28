// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "CalibrationBehaviour.h"

#include "app/providers/PartsProvider.h"
#include "core/log/Log.h"
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
    mCounter = 0;
    static PiezoPlayer& piezo = PartsProvider::GetPiezoPlayer(); 
    piezo.playTone(Tone(75_ms).sweep(1000, 1500), 50_ms); 
    piezo.playTone(Tone(75_ms).sweep(1000, 2000), 50_ms); 
    piezo.playTone(Tone(75_ms).sweep(1000, 3000), 50_ms);
}

void CalibrationBehaviour::onBecomeInactive(){
    static PiezoPlayer& piezo = PartsProvider::GetPiezoPlayer(); 
    piezo.playTone(Tone(400_ms).sweep(1000, 2500));
}

void CalibrationBehaviour::onLogicTick(){
    // nothing to do
}

void CalibrationBehaviour::onHeartbeat(const HeartbeatEvent& event){
    static PiezoPlayer& piezo = PartsProvider::GetPiezoPlayer();
    if(mCounter++ > 3){
        getHost()->requestChangeBehaviour(BehaviourId::Fermentation);
    }
    else {
        piezo.playTone(Tone(125_ms).sweep(1000, 1500), 125_ms);
        piezo.playTone(Tone(125_ms).sweep(1250, 2000));
    }
}

void CalibrationBehaviour::onFanStatus(const FanStatusEvent& event){
    // nothing to do
}

void CalibrationBehaviour::onFanSample(const FanSampleEvent& event){
    // nothing to do
}

} // namespace Garbox