#include "EventType.h"

#include <array>

namespace Garbox {

const char* EventTypeToString(EventType type){
    switch(type){
        case EventType::Null:       return "Null";
        case EventType::Fan:        return "Fan";
        case EventType::Button:     return "Button";
        case EventType::Heartbeat:  return "Heartbeat";
        case EventType::Count:      return "Count";
    }
    return "Invalid";
}

} // namespace