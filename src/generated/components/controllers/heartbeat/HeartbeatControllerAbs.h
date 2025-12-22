#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
 

#include "core/application/controller/ControllerAbs.h"

namespace Garbox {

class Runtime;

class HeartbeatControllerAbs : public ControllerAbs {
public:

    // component constructor
    HeartbeatControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onHeartbeatTick() = 0;

protected:

    // make typed events 
    HeartbeatEvent makeHeartbeatEvent();

    // send typed events
    void sendEvent(const HeartbeatEvent& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox