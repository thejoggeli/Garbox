// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class I2cPartsControllerAbs : public ControllerAbs {
public:
    
    // controller constructor
    I2cPartsControllerAbs(const RuntimeContext& context);

    // tick handlers
    virtual void onInputTick() = 0;

    // receive event handlers
    virtual void onButtonStateChanged(const EventRead<EventPayload::ButtonStateChanged>& event) = 0;

};

} // namespace Garbox