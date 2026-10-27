#include "FermentationState.h"

namespace Garbox {

const char* FermentationStateToString(FermentationState id){
    switch(id){
        case FermentationState::Null: return "Null";
        case FermentationState::Reset: return "Reset";
        case FermentationState::Ready: return "Ready";
        case FermentationState::Regulating: return "Regulating";
        case FermentationState::InvalidInput: return "InvalidInput";
        case FermentationState::OverTemperature: return "OverTemperature";
        case FermentationState::Cooldown: return "Cooldown";
        case FermentationState::Count: return "Count";
    }
    return "Invalid";
}

const char* SafetyFaultToString(SafetyFault fault){
    switch(fault){
        case SafetyFault::None: return "None";
        case SafetyFault::SensorUnavailable: return "SensorUnavailable";
        case SafetyFault::SensorStale: return "SensorStale";
        case SafetyFault::SensorImplausible: return "SensorImplausible";
        case SafetyFault::AbsoluteOverTemperature: return "AbsoluteOverTemperature";
        case SafetyFault::UnexpectedHeaterOutput: return "UnexpectedHeaterOutput";
        case SafetyFault::InvalidHeaterCommand: return "InvalidHeaterCommand";
        case SafetyFault::Count: return "Count";
    }
    return "Invalid";
}

} // namespace Garbox
