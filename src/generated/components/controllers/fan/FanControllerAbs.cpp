// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "FanControllerAbs.h"

namespace Garbox {

FanControllerAbs::FanControllerAbs() : ControllerAbs(ComponentId::FanController, ControllerId::Fan){
    // nothing to do
}

FanControllerStates& FanControllerAbs::states(){
    return mStates.value();
}

} // namespace Garbox