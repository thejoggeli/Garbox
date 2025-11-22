// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class HeatpadControllerAbs : public ControllerAbs {
public:
    
    HeatpadControllerAbs();
    
    ControllerId getControllerId() const final;

    // tick handlers
    virtual void onInputTick() = 0;
    virtual void onOutputTick() = 0;

    // shorthand types for read events 
    using HeatpadCommand = EventRead<EventPayload::HeatpadCommand>;

    // receive event handlers
    virtual void onHeatpadCommand(const HeatpadCommand& event) = 0;

protected:

    // shorthand types for write events 
    using HeatpadStatusWrite = EventWrite<EventPayload::HeatpadStatus>;

    // make typed events
    HeatpadStatusWrite makeHeatpadStatusEvent();

    // send typed events
    void sendEvent(const HeatpadStatusWrite& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEvent;

};

} // namespace Garbox