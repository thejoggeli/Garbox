// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "ScreenId.h"

namespace Garbox {

const char* ScreenIdToString(ScreenId id){
    switch(id){
        case ScreenId::Null: return "Null";
        case ScreenId::Simple: return "Simple";
        case ScreenId::Main: return "Main";
        case ScreenId::Debug: return "Debug";
        case ScreenId::EventLog: return "EventLog";
        case ScreenId::StateLog: return "StateLog";
        case ScreenId::Count: return "Count";
    }
    return "Invalid";
}

} // namespace Garbox