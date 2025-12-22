// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HeatpadControllerAbs.h"

namespace Garbox {

HeatpadControllerAbs::HeatpadControllerAbs() : ControllerAbs(ComponentId::HeatpadController, ControllerId::Heatpad){
    // nothing to do
}

HeatpadControllerStates& HeatpadControllerAbs::states(){
    return mStates.value();
}

} // namespace Garbox