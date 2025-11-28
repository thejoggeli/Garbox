// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "ComponentId.h"

namespace Garbox {

const char* ComponentIdToString(ComponentId id){
    switch(id){
        case ComponentId::Null: return "Null";
        case ComponentId::Runtime: return "Runtime";
        case ComponentId::Replay: return "Replay";
        case ComponentId::DisplayController: return "DisplayController";
        case ComponentId::DevtoolsController: return "DevtoolsController";
        case ComponentId::FanController: return "FanController";
        case ComponentId::HeartbeatController: return "HeartbeatController";
        case ComponentId::HeatpadController: return "HeatpadController";
        case ComponentId::InputController: return "InputController";
        case ComponentId::I2cPartsController: return "I2cPartsController";
        case ComponentId::CalibrationBehaviour: return "CalibrationBehaviour";
        case ComponentId::FermentationBehaviour: return "FermentationBehaviour";
        case ComponentId::MainScreen: return "MainScreen";
        case ComponentId::DebugScreen: return "DebugScreen";
        case ComponentId::Count: return "Count";
    }
    return "Invalid";
}

} // namespace Garbox