#include "HeartbeatController.h"

#include "app/parts/StatusLeds.h"
#include "global/config/AppConfig.h"
#include "global/providers/PartsProvider.h"
#include "util/function/default/EasingFunctions.h"

namespace Garbox {

HeartbeatController::HeartbeatController(): 
    // initialize members
    mIntervalMicros(AppConfig::HeartbeatIntervalMicros),
    mHeartbeatLed(PartsProvider::GetStatusLed(StatusLedId::Heartbeat)){
    // nothing to do
}

void HeartbeatController::onInit(){
    mHeartbeatLed.animationClear();
    mHeartbeatLed.animationAddFrame(EasingFunctions::GetInOutSine(), 800_ms, 0.0f,  1.0f);
    mHeartbeatLed.animationAddDelay(200_ms);
    mHeartbeatLed.animationAddFrame(EasingFunctions::GetInOutSine(), 800_ms, 1.0f,  0.0f);
}

void HeartbeatController::onStart(){
    mHeartbeatLed.animationStart();
    mHeartbeatTimer.start(mIntervalMicros);
}

void HeartbeatController::onTick(){
    if(mHeartbeatTimer.isExpired()){
        mHeartbeatLed.animationStart();
        mHeartbeatTimer.restart();
    }
}

} // namespace

