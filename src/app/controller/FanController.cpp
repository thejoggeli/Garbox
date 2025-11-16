#include "FanController.h"

#include "assert/Assert.h"

namespace Garbox {

void FanController::onTick(){
    // nothing to do
}

void FanController::handleButtonEvent(const ButtonEvent& event){

    FanEvent out;
    out.measuredRpm = 1234.5f;
    out.targetSpeed = 50.0f;
    sendEvent(out);
}

} // namespace