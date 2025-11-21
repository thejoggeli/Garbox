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
    FanControllerAbs(const RuntimeContext& context);

    // tick handlers
    virtual void onInputTick() = 0;
    virtual void onOutputTick() = 0;

    // receive event handlers
    virtual void onFanCommand(const EventRead<EventPayload::FanCommand>& event) = 0;

protected:

    // make typed events
    EventWrite<EventPayload::FanStatus> makeFanStatusEvent();
    EventWrite<EventPayload::FanSample> makeFanSampleEvent();

    // send typed events
    void sendEvent(const EventWrite<EventPayload::FanStatus>& event);
    void sendEvent(const EventWrite<EventPayload::FanSample>& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEvent;

};

} // namespace Garbox