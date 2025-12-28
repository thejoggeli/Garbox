// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "TickPhase.h"

namespace Garbox {

const char* TickPhaseToString(TickPhase phase){
    switch(phase){
        case TickPhase::Null: return "Null";
        case TickPhase::Heartbeat: return "Heartbeat";
        case TickPhase::SensorRead: return "SensorRead";
        case TickPhase::UserInput: return "UserInput";
        case TickPhase::Logic: return "Logic";
        case TickPhase::ActorWrite: return "ActorWrite";
        case TickPhase::Logging: return "Logging";
        case TickPhase::Render: return "Render";
        case TickPhase::Count: return "Count";
    }
    return "Invalid";
}

} // namespace Garbox