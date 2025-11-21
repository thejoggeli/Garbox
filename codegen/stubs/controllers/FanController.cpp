#include "FanController.h"

namespace Garbox {

FanController::FanController(const RuntimeContext& context): FanControllerAbs(context){
    // nothing to do
}

void FanController::onInit(){
    // nothing to do
}

void FanController::onStart(){
    // nothing to do
}

void FanController::onInputTick(){
    // nothing to do
}
void FanController::onOutputTick(){
    // nothing to do
}

void FanController::onFanCommand(const EventRead<EventPayload::FanCommand>& event){
    // nothing to do
}

} // namespace Garbox