#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"
#include "core/util/helpers/DirtyDispatcher.h"
#include "shared/types/EventType.h"

namespace Garbox {

class DebugScreenAbs : public ScreenAbs {
protected:

public:

    // receive event handlers (to be implmeneted by user)
    virtual void onHeartbeat(const HeartbeatEvent& event) = 0;

    DebugScreenAbs();

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