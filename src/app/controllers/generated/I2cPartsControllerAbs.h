#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class I2cPartsControllerAbs : public ControllerAbs {
public:
    
    // controller constructor
    I2cPartsControllerAbs(ComponentId id, const RuntimeContext& context) : ControllerAbs(id, context) {}

    // tick handlers
    virtual void onInputTick() = 0;

};

} // namespace Garbox