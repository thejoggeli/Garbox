#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"

namespace Garbox {

class DebugScreenAbs : public ScreenAbs {
public:

    DebugScreenAbs();

    void receiveTick(TickPhase phase) final {};
    void receiveEvent(const EventHeader* event) final {};

    // receive event handlers
    virtual void onHeartbeat(const HeartbeatEvent& event) = 0;

protected:

    // make typed events
    DisplayCommandEvent makeDisplayCommandEvent();

    // send typed events
    void sendEvent(const DisplayCommandEvent& event);

private:

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

};

} // namespace Garbox