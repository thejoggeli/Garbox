// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "BehaviourId.h"

namespace Garbox {

const char* BehaviourIdToString(BehaviourId id){
    switch(id){
        case BehaviourId::Null: return "Null";
        case BehaviourId::Calibration: return "Calibration";
        case BehaviourId::Fermentation: return "Fermentation";
        case BehaviourId::Count: return "Count";
    }
    return "Invalid";
}

} // namespace Garbox