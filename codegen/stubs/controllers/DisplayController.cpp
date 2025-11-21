// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#include "DisplayController.h"

namespace Garbox {

DisplayController::DisplayController(const RuntimeContext& context): DisplayControllerAbs(context){
    // nothing to do
}

void DisplayController::onInit(){
    // nothing to do
}

void DisplayController::onStart(){
    // nothing to do
}

void DisplayController::onRenderTick(){
    // nothing to do
}

void DisplayController::onTemperatureStatus(const EventRead<EventPayload::TemperatureStatus>& event){
    // nothing to do
}
void DisplayController::onTemperatureSample(const EventRead<EventPayload::TemperatureSample>& event){
    // nothing to do
}
void DisplayController::onBacklightCommand(const EventRead<EventPayload::BacklightCommand>& event){
    // nothing to do
}

} // namespace Garbox