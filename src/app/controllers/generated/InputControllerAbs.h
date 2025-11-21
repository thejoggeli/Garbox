#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class InputControllerAbs : public ControllerAbs {
public:
    
    // controller constructor
    InputControllerAbs(const RuntimeContext& context);

    // tick handlers
    virtual void onInputTick() = 0;

};

} // namespace Garbox