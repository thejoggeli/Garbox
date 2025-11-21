// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "EventType.h"



namespace Garbox {

const char* EventTypeToString(EventType type){
    switch(type){

        case EventType::Null: return "Null";

        case EventType::Heartbeat: return "Heartbeat";
        case EventType::FanStatus: return "FanStatus";
        case EventType::FanSample: return "FanSample";
        case EventType::FanCommand: return "FanCommand";
        case EventType::HeatpadStatus: return "HeatpadStatus";
        case EventType::HeatpadCommand: return "HeatpadCommand";
        case EventType::TemperatureStatus: return "TemperatureStatus";
        case EventType::TemperatureSample: return "TemperatureSample";
        case EventType::Button: return "Button";
        case EventType::ButtonRepeat: return "ButtonRepeat";
        case EventType::EncoderStep: return "EncoderStep";

        case EventType::Count: return "Count";
    }

    return "Invalid";
}

} // namespace Garbox