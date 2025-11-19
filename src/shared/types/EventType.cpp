#include "EventType.h"

namespace Garbox {

const char* EventTypeToString(EventType type){
    switch(type){
        case EventType::Null: return "Null";

        // status events
        case EventType::Heartbeat: return "Heartbeat";
        case EventType::FanStatus: return "FanStatus";
        case EventType::HeatpadStatus: return "HeatpadStatus";
        case EventType::TemperatureStatus: return "TemperatureStatus";

        // command events
        case EventType::FanCommand: return "FanCommand";
        case EventType::HeatpadCommand: return "HeatpadCommand";

        // special type
        case EventType::Count: return "Count";
    }
    return "Invalid";
}

} // namespace