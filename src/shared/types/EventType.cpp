#include "EventType.h"

namespace Garbox {

const char* EventTypeToString(EventType type){
    switch(type){
        case EventType::Null: return "Null";
        case EventType::Heartbeat: return "Heartbeat";
        case EventType::FanStatus: return "FanStatus";
        case EventType::FanCommand: return "FanCommand";
        case EventType::HeatpadStatus: return "HeatpadStatus";
        case EventType::HeatpadCommand: return "HeatpadCommand";
        case EventType::Count: return "Count";
    }
    return "Invalid";
}

} // namespace