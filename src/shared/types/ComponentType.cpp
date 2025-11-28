#include "ComponentType.h"

namespace Garbox {

const char* ComponentTypeToString(ComponentType type){
    switch(type){

        // component types
        case ComponentType::Runtime: return "Runtime";
        case ComponentType::Replay: return "Replay";
        case ComponentType::Controller: return "Controller";
        case ComponentType::Behaviour: return "Behaviour";
        case ComponentType::Screen: return "Screen";

        // special types
        case ComponentType::Null: return "Null";
        case ComponentType::Count: return "Count";
    }
    return "Invalid";
}

} // namespace