#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class EventLogScreenAbs : public ScreenAbs {

public:

    // event handler (to be implmeneted by user)
    virtual void onEvent(const EventHeader* header) = 0;

    // component constructor
    EventLogScreenAbs();

    // init override from component
    void init(ComponentHostIfc& host) final;

protected:

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