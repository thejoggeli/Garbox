#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class DisplayControllerAbs : public ControllerAbs {
public:

    DisplayControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onRenderTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onDisplayCommand(const DisplayCommandEvent& event) = 0;

protected:

    // make typed events 
    DisplayStatusEvent makeDisplayStatusEvent();

    // send typed events
    void sendEvent(const DisplayStatusEvent& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::publishEvent;

};

} // namespace Garbox