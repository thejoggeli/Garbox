// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DisplayControllerAbs.h"

namespace Garbox {

DisplayControllerAbs::DisplayControllerAbs() : ControllerAbs(ComponentId::DisplayController, ControllerId::Display){
    // nothing to do
}

DisplayControllerAbs::States& DisplayControllerAbs::states(){
    return mStates.value();
}

void DisplayControllerAbs::bindStates(
    DisplayStatusState& displayStatus,
    DisplayDiagnosticsState& displayDiagnostics
){
    mStates.emplace(
        displayStatus,
        displayDiagnostics
    );
}

} // namespace Garbox