#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class HeatpadControllerAbs : public ControllerAbs {
public:
    
    // controller constructor
    HeatpadControllerAbs(ComponentId id, const RuntimeContext& context) : ControllerAbs(id, context) {}

    // tick handlers
    virtual void onInputTick() = 0;
    virtual void onOutputTick() = 0;

    // receive event handlers
    virtual void onHeatpadCommand(const EventRead<EventPayload::HeatpadCommand>& event) = 0;

};

} // namespace Garbox