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
    DisplayControllerAbs();

    // tick handlers
    virtual void onRenderTick() = 0;

    // shorthand types for read events 
    using TemperatureStatus = EventRead<EventPayload::TemperatureStatus>;
    using TemperatureSample = EventRead<EventPayload::TemperatureSample>;
    using BacklightCommand = EventRead<EventPayload::BacklightCommand>;

    // receive event handlers
    virtual void onTemperatureStatus(const TemperatureStatus& event) = 0;
    virtual void onTemperatureSample(const TemperatureSample& event) = 0;
    virtual void onBacklightCommand(const BacklightCommand& event) = 0;

protected:

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEvent;

};

} // namespace Garbox