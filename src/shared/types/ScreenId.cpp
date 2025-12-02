// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "ScreenId.h"

namespace Garbox {

const char* ScreenIdToString(ScreenId id){
    switch(id){
        case ScreenId::Null: return "Null";
        case ScreenId::Main: return "Main";
        case ScreenId::EventLog: return "EventLog";
        case ScreenId::Debug: return "Debug";
        case ScreenId::Count: return "Count";
    }
    return "Invalid";
}

} // namespace Garbox