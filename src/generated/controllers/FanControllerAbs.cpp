// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "FanControllerAbs.h"

namespace Garbox {

FanControllerAbs::FanControllerAbs() : ControllerAbs(ComponentId::FanController, ControllerId::Fan){
    // nothing to do
}

FanControllerAbs::States& FanControllerAbs::states(){
    return mStates.value();
}

void FanControllerAbs::bindStates(
    FanStatusState& fanStatus,
    FanSampleState& fanSample
){
    mStates.emplace(
        fanStatus,
        fanSample
    );
}

} // namespace Garbox