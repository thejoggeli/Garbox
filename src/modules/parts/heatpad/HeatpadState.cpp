#include "HeatpadState.h"

namespace Garbox {

const char* HeatpadStateToString(HeatpadState state){
    switch(state){
    case HeatpadState::Disabled: return "Disabled";
    case HeatpadState::Heating:  return "Heating";
    case HeatpadState::Idle:     return "Idle";
    case HeatpadState::Count:    return "Count";
    }
    return "Unknown";
}

} // namespace
