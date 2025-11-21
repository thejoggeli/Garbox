#include "DisplayControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

DisplayControllerAbs::DisplayControllerAbs(const RuntimeContext& context): 
    // init members
    ControllerAbs(ComponentId::DisplayController, context){
    // nothing to do
}

} // namespace Garbox