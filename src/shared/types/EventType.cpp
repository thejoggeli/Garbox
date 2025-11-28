// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "EventType.h"

namespace Garbox {

const char* EventTypeToString(EventType type){
    switch(type){

        case EventType::Null: return "Null";

        case EventType::Heartbeat: return "Heartbeat";
        case EventType::FermentationStatus: return "FermentationStatus";
        case EventType::BacklightCommand: return "BacklightCommand";
        case EventType::FanStatus: return "FanStatus";
        case EventType::FanSample: return "FanSample";
        case EventType::FanCommand: return "FanCommand";
        case EventType::HeatpadStatus: return "HeatpadStatus";
        case EventType::HeatpadSample: return "HeatpadSample";
        case EventType::HeatpadCommand: return "HeatpadCommand";
        case EventType::TemperatureStatus: return "TemperatureStatus";
        case EventType::TemperatureSample: return "TemperatureSample";
        case EventType::ButtonStateChanged: return "ButtonStateChanged";
        case EventType::ButtonRepeat: return "ButtonRepeat";
        case EventType::EncoderStep: return "EncoderStep";
        case EventType::ActiveBehaviourChanged: return "ActiveBehaviourChanged";
        case EventType::RequestChangeBehaviour: return "RequestChangeBehaviour";
        case EventType::ActiveScreenChanged: return "ActiveScreenChanged";
        case EventType::RequestChangeScreen: return "RequestChangeScreen";

        case EventType::Count: return "Count";
    }

    return "Invalid";
}

} // namespace Garbox