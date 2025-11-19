#include "HeartbeatController.h"

#include "app/config/AppConfig.h"
#include "app/parts/StatusLeds.h"
#include "app/providers/PartsProvider.h"
#include "core/util/function/default/EasingFunctions.h"

namespace Garbox {

HeartbeatController::HeartbeatController(ComponentId id): 
    // initialize members
    ControllerAbs(id),
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
    mHeartbeatLed.animationStart();
    mHeartbeatTimer.start(mIntervalMicros);
}

void HeartbeatController::onTick(){
    if(mHeartbeatTimer.isExpired()){
        mHeartbeatLed.animationStart();
        mHeartbeatTimer.restart();

        // send heartbeat event
        EventWrapper wrapper = makeEvent<EventData::Heartbeat>();
        sendEvent(wrapper.event);
    }
}

} // namespace

