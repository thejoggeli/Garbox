// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "EventType.h"

namespace Garbox {

const char* EventTypeToString(EventType type){
    switch(type){

        case EventType::Null: return "Null";

        case EventType::Heartbeat: return "Heartbeat";
        case EventType::DisplayCommand: return "DisplayCommand";
        case EventType::FanCommand: return "FanCommand";
        case EventType::HeatpadCommand: return "HeatpadCommand";
        case EventType::ButtonStateChanged: return "ButtonStateChanged";
        case EventType::ButtonRepeat: return "ButtonRepeat";
        case EventType::EncoderStep: return "EncoderStep";
        case EventType::FermentationStatus: return "FermentationStatus";
        case EventType::DisplayStatus: return "DisplayStatus";
        case EventType::FanStatus: return "FanStatus";
        case EventType::FanSample: return "FanSample";
        case EventType::HeatpadStatus: return "HeatpadStatus";
        case EventType::HeatpadSample: return "HeatpadSample";
        case EventType::TemperatureStatus: return "TemperatureStatus";
        case EventType::TemperatureSample: return "TemperatureSample";
        case EventType::ActiveBehaviourChanged: return "ActiveBehaviourChanged";
        case EventType::ActiveScreenChanged: return "ActiveScreenChanged";

        case EventType::Count: return "Count";
    }

    return "Invalid";
}

} // namespace Garbox