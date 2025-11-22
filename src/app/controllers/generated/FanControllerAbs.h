// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class FanControllerAbs : public ControllerAbs {
public:
    
    // controller constructor
    FanControllerAbs();

    // tick handlers
    virtual void onInputTick() = 0;
    virtual void onOutputTick() = 0;

    // shorthand types for read events 
    using FanCommand = EventRead<EventPayload::FanCommand>;

    // receive event handlers
    virtual void onFanCommand(const FanCommand& event) = 0;

protected:

    // shorthand types for write events 
    using FanStatusWrite = EventWrite<EventPayload::FanStatus>;
    using FanSampleWrite = EventWrite<EventPayload::FanSample>;

    // make typed events
    FanStatusWrite makeFanStatusEvent();
    FanSampleWrite makeFanSampleEvent();

    // send typed events
    void sendEvent(const FanStatusWrite& event);
    void sendEvent(const FanSampleWrite& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEvent;

};

} // namespace Garbox