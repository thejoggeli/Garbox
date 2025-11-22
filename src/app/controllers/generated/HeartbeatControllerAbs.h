// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class HeartbeatControllerAbs : public ControllerAbs {
public:
    
    HeartbeatControllerAbs();

    // tick handlers
    virtual void onHeartbeatTick() = 0;

protected:

    // shorthand types for write events 
    using HeartbeatWrite = EventWrite<EventPayload::Heartbeat>;

    // make typed events
    HeartbeatWrite makeHeartbeatEvent();

    // send typed events
    void sendEvent(const HeartbeatWrite& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEvent;

};

} // namespace Garbox