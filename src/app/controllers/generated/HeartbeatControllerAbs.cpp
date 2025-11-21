#include "HeartbeatControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

HeartbeatControllerAbs::HeartbeatControllerAbs(const RuntimeContext& context): 
    // init members
    ControllerAbs(ComponentId::HeartbeatController, context){
    // nothing to do
}

} // namespace Garbox