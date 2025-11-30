#include "HeartbeatController.h"

#include "app/config/AppConfig.h"
#include "app/providers/PartsProvider.h"
#include "core/util/function/default/EasingFunctions.h"
#include "modules/parts/led/single/AnimatedLed.h"

namespace Garbox {

HeartbeatController::HeartbeatController(): 
    // initialize members
    HeartbeatControllerAbs(),
    mIntervalMicros(AppConfig::HeartbeatIntervalMicros),
    mHeartbeatLed(PartsProvider::GetStatusLed(StatusLedId::Heartbeat)){
    // nothing to do
}

void HeartbeatController::onInit(){
    // nothing to do
}

void HeartbeatController::onStart(){
    mHeartbeatLed.animationClear();
    mHeartbeatLed.animationAddFrame(EasingFunctions::GetInOutSine(), 800_ms, 0.0f,  1.0f);
    mHeartbeatLed.animationAddDelay(200_ms);
    mHeartbeatLed.animationAddFrame(EasingFunctions::GetInOutSine(), 800_ms, 1.0f,  0.0f);
    mHeartbeatTimer.start(0);
}

void HeartbeatController::onHeartbeatTick(){
    if(mHeartbeatTimer.isExpired()){
        mHeartbeatLed.animationStart();
        mHeartbeatTimer.restart(mIntervalMicros);

        // send heartbeat event
        HeartbeatEvent event = makeHeartbeatEvent();
        sendEvent(event);
    }
}

} // namespace

