#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class HeartbeatControllerAbs : public ControllerAbs {
public:
    
    // controller constructor
    HeartbeatControllerAbs(ComponentId id, const RuntimeContext& context) : ControllerAbs(id, context) {}

    // tick handlers
    virtual void onHeartbeatTick() = 0;

};

} // namespace Garbox