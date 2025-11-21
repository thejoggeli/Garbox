#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class DisplayControllerAbs : public ControllerAbs {
public:
    
    // controller constructor
    DisplayControllerAbs(const RuntimeContext& context);

    // tick handlers
    virtual void onRenderTick() = 0;

};

} // namespace Garbox