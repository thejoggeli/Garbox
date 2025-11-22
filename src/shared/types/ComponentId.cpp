// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "ComponentId.h"

namespace Garbox {

const char* ComponentIdToString(ComponentId id){
    switch(id){
        case ComponentId::Null: return "Null";
        case ComponentId::FanController: return "FanController";
        case ComponentId::InputController: return "InputController";
        case ComponentId::FermentationBehaviour: return "FermentationBehaviour";
        case ComponentId::HeatpadController: return "HeatpadController";
        case ComponentId::CalibrationBehaviour: return "CalibrationBehaviour";
        case ComponentId::I2cPartsController: return "I2cPartsController";
        case ComponentId::HeartbeatController: return "HeartbeatController";
        case ComponentId::DisplayController: return "DisplayController";
        case ComponentId::Count: return "Count";
    }
    return "Invalid";
}

} // namespace Garbox