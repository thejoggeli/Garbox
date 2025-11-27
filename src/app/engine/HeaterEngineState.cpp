#include "HeaterEngineState.h"

namespace Garbox {

const char* HeaterEngineStateToString(HeaterEngineState id){
    switch(id){
        case HeaterEngineState::Null: return "Null";
        case HeaterEngineState::Reset: return "Reset";
        case HeaterEngineState::Ready: return "Ready";
        case HeaterEngineState::Regulating: return "Regulating";
        case HeaterEngineState::InvalidInput: return "InvalidInput";
        case HeaterEngineState::OverTemperature: return "OverTemperature";
        case HeaterEngineState::Cooldown: return "Cooldown";
        case HeaterEngineState::Count: return "Count";
    }
    return "Invalid";
}

} // namespace Garbox