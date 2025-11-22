// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "ComponentId.h"

namespace Garbox {

const char* ComponentIdToString(ComponentId id){
    switch(id){
        case ComponentId::Null: return "Null";
        case ComponentId::Runtime: return "Runtime";
        case ComponentId::CalibrationBehaviour: return "CalibrationBehaviour";
        case ComponentId::DisplayController: return "DisplayController";
        case ComponentId::FanController: return "FanController";
        case ComponentId::FermentationBehaviour: return "FermentationBehaviour";
        case ComponentId::HeartbeatController: return "HeartbeatController";
        case ComponentId::HeatpadController: return "HeatpadController";
        case ComponentId::I2cPartsController: return "I2cPartsController";
        case ComponentId::InputController: return "InputController";
        case ComponentId::Count: return "Count";
    }
    return "Invalid";
}

} // namespace Garbox