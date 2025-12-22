// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DisplayControllerAbs.h"

namespace Garbox {

DisplayControllerAbs::DisplayControllerAbs() : ControllerAbs(ComponentId::DisplayController, ControllerId::Display){
    // nothing to do
}

DisplayControllerStates& DisplayControllerAbs::states(){
    return mStates.value();
}

} // namespace Garbox