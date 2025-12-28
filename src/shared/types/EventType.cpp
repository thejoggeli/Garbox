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
        case EventType::Button: return "Button";
        case EventType::ButtonRepeat: return "ButtonRepeat";
        case EventType::EncoderStep: return "EncoderStep";
        case EventType::RequestFanMode: return "RequestFanMode";
        case EventType::RequestFermentationMode: return "RequestFermentationMode";
        case EventType::RequestTargetTemperature: return "RequestTargetTemperature";
        case EventType::ActiveBehaviourChanged: return "ActiveBehaviourChanged";
        case EventType::ActiveScreenChanged: return "ActiveScreenChanged";

        case EventType::Count: return "Count";
    }

    return "Invalid";
}

} // namespace Garbox