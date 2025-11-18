#include "FanState.h"

namespace Garbox {

const char* FanStateToString(FanState state){
    switch(state){
    case FanState::Disabled: return "Disabled";
    case FanState::Enabled:  return "Enabled";
    case FanState::Stalled:  return "Stalled";
    case FanState::Count:    return "Count";
    }
    return "Unknown";
}

} // namespace
