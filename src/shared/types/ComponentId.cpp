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
        case ComponentId::GarboxController: return "GarboxController";
        case ComponentId::HeatpadController: return "HeatpadController";
        case ComponentId::DisplayController: return "DisplayController";

        // special types
        case ComponentId::Null: return "Null";
        case ComponentId::Count: return "Count";
    }
    return "Invalid";
}

} // namespace