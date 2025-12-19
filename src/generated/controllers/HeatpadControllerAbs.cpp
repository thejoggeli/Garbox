// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HeatpadControllerAbs.h"

namespace Garbox {

HeatpadControllerAbs::HeatpadControllerAbs() : ControllerAbs(ComponentId::HeatpadController, ControllerId::Heatpad){
    // nothing to do
}

HeatpadControllerAbs::States& HeatpadControllerAbs::states(){
    return mStates.value();
}

void HeatpadControllerAbs::bindStates(
    HeatpadStatusState& heatpadStatus,
    HeatpadSampleState& heatpadSample,
    HeatpadProgressState& heatpadProgress
){
    mStates.emplace(
        heatpadStatus,
        heatpadSample,
        heatpadProgress
    );
}

} // namespace Garbox