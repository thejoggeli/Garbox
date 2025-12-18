// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DisplayControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

DisplayControllerAbs::DisplayControllerAbs() : ControllerAbs(ComponentId::DisplayController, ControllerId::Display){
    // nothing to do
}

DisplayStatusEvent DisplayControllerAbs::makeDisplayStatusEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayStatus>();
}

void DisplayControllerAbs::sendEvent(const DisplayStatusEvent& event){
    sendEventToHost(event.header());
}

DisplayStatusState& DisplayControllerAbs::stateDisplayStatus(){
    return *mDisplayStatusState;
}

DisplayDiagnosticsState& DisplayControllerAbs::stateDisplayDiagnostics(){
    return *mDisplayDiagnosticsState;
}

void DisplayControllerAbs::injectDisplayStatusState(DisplayStatusState* state){
    mDisplayStatusState = state;
}

void DisplayControllerAbs::injectDisplayDiagnosticsState(DisplayDiagnosticsState* state){
    mDisplayDiagnosticsState = state;
}

} // namespace Garbox