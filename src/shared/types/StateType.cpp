// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "StateType.h"

namespace Garbox {

const char* StateTypeToString(StateType type){
    switch(type){

        case StateType::Null: return "Null";

        case StateType::DisplayStatus: return "DisplayStatus";
        case StateType::DisplayDiagnostics: return "DisplayDiagnostics";
        case StateType::FanStatus: return "FanStatus";
        case StateType::FanSample: return "FanSample";
        case StateType::FermentationStatus: return "FermentationStatus";
        case StateType::HeatpadStatus: return "HeatpadStatus";
        case StateType::HeatpadSample: return "HeatpadSample";
        case StateType::HeatpadProgress: return "HeatpadProgress";
        case StateType::TemperatureStatus: return "TemperatureStatus";
        case StateType::TemperatureSample: return "TemperatureSample";
        case StateType::ActiveBehaviour: return "ActiveBehaviour";
        case StateType::ActiveScreen: return "ActiveScreen";

        case StateType::Count: return "Count";
    }

    return "Invalid";
}

} // namespace Garbox