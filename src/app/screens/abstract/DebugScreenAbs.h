#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"

namespace Garbox {

class DebugScreenAbs : public ScreenAbs {
public:

    DebugScreenAbs();

    // receive event handlers
    virtual void onHeartbeat(const HeartbeatEvent& event) = 0;

    // update handlers
    void updateFanStatus(const FanStatusPayload& payload);

protected:

    // make typed events
    BacklightCommandEvent makeBacklightCommandEvent();

    // send typed events
    void sendEvent(const BacklightCommandEvent& event);

private:

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

};

} // namespace Garbox