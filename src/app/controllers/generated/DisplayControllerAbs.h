// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

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

    // receive event handlers
    virtual void onTemperatureStatus(const EventRead<EventPayload::TemperatureStatus>& event) = 0;
    virtual void onTemperatureSample(const EventRead<EventPayload::TemperatureSample>& event) = 0;
    virtual void onBacklightCommand(const EventRead<EventPayload::BacklightCommand>& event) = 0;

protected:

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEvent;

};

} // namespace Garbox