#include "ComponentId.h"

namespace Garbox {

const char* ComponentIdToString(ComponentId id){
    switch(id){

        // behaviours
        case ComponentId::CalibrationBehaviour: return "CalibrationBehaviour";
        case ComponentId::FermentationBehaviour: return "FermentationBehaviour";

        // controllers
        case ComponentId::FanController: return "FanController";
        case ComponentId::HeartbeatController: return "HeartbeatController";
        case ComponentId::InputController: return "InputController";
        case ComponentId::HeatpadController: return "HeatpadController";
        case ComponentId::DisplayController: return "DisplayController";
        case ComponentId::I2cPartsController: return "I2cPartsController";

        // special types
        case ComponentId::Null: return "Null";
        case ComponentId::Count: return "Count";
    }
    return "Invalid";
}

} // namespace