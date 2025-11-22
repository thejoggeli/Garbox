// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "DisplayControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

DisplayControllerAbs::DisplayControllerAbs(): 
    // init members
    ControllerAbs(ComponentId::DisplayController){
    // nothing to do
}

ControllerId DisplayControllerAbs::getControllerId() const {
    return ControllerId::Display;
}

} // namespace Garbox