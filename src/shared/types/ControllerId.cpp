// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "ControllerId.h"

namespace Garbox {

const char* ControllerIdToString(ControllerId id){
    switch(id){
        case ControllerId::Null: return "Null";
        case ControllerId::Display: return "Display";
        case ControllerId::Fan: return "Fan";
        case ControllerId::Heartbeat: return "Heartbeat";
        case ControllerId::Heatpad: return "Heatpad";
        case ControllerId::Input: return "Input";
        case ControllerId::I2cParts: return "I2cParts";
        case ControllerId::Count: return "Count";
    }
    return "Invalid";
}

} // namespace Garbox