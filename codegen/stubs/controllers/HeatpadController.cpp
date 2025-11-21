#include "HeatpadController.h"

namespace Garbox {

HeatpadController::HeatpadController(const RuntimeContext& context): HeatpadControllerAbs(context){
    // nothing to do
}

void HeatpadController::onInit(){
    // nothing to do
}

void HeatpadController::onStart(){
    // nothing to do
}

void HeatpadController::onInputTick(){
    // nothing to do
}
void HeatpadController::onOutputTick(){
    // nothing to do
}

void HeatpadController::onHeatpadCommand(const EventRead<EventPayload::HeatpadCommand>& event){
    // nothing to do
}

} // namespace Garbox