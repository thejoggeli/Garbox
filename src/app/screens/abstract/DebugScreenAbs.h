#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class DebugScreenAbs : public ScreenAbs {

public:

    // event handlers (to be implmeneted by user)
    virtual void onHeartbeat(const HeartbeatEvent& event) = 0;

    // constructor requires lvgl context
    DebugScreenAbs(LvglContext& lvgl);

    // init override from component
    void init(ComponentHostIfc& host) final;

protected:

    // make typed events 
    DisplayCommandEvent makeDisplayCommandEvent();

    // send typed events
    void sendEvent(const DisplayCommandEvent& event);

private:

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

    // hide dangerous raw access
    using ScreenAbs::registerUpdateHandler;
    using ScreenAbs::markDirtyRaw;
    using ScreenAbs::isMarkedDirtyRaw;

};

} // namespace Garbox