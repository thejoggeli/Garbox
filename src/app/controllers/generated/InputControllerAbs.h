#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class InputControllerAbs : public ControllerAbs {
public:
    
    // controller constructor
    InputControllerAbs(ComponentId id, const RuntimeContext& context) : ControllerAbs(id, context) {}

    // tick handlers
    virtual void onInputTick() = 0;

};

} // namespace Garbox