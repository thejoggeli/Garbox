#include "FanController.h"

#include "assert/Assert.h"

namespace Garbox {

FanController::FanController(){
    // nothing to do
}

void FanController::onInit(){
    // nothing to do
}

void FanController::onStart(){
    // nothing to do
}

void FanController::onTick(){
    // nothing to do
}

void FanController::onFanEvent(const EventWrapper<FanEventData>& event){
    EventWrapper<FanEventData> response = getEventFactory().make<FanEventData>();
    response.data->measuredRpm = 1234.5f;
    response.data->targetSpeed = 50.0f;
    sendEvent(response.header);
}

void FanController::onButtonEvent(const EventWrapper<ButtonEventData>& event){
    // nothing to do
}

} // namespace