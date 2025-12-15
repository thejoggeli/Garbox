#include "TempsenseState.h"

namespace Garbox {

const char* TempsenseStateToString(TempsenseState state){
    switch(state){
        case TempsenseState::Initial: return "Initial";
        case TempsenseState::Setup:   return "Setup";
        case TempsenseState::Running: return "Running";
        case TempsenseState::Reset:   return "Reset";
        case TempsenseState::Error:   return "Error";
        case TempsenseState::Count:   return "Count";
    }
    return "Unknown";
}

} // namespace
