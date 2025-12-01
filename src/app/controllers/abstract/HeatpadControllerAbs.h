#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class HeatpadControllerAbs : public ControllerAbs {
public:

    HeatpadControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onInputTick() = 0;
    virtual void onOutputTick() = 0;

    // receive event handlers (to be implmeneted by user)
    virtual void onHeatpadCommand(const HeatpadCommandEvent& event) = 0;

protected:

    // make typed events 
    HeatpadStatusEvent makeHeatpadStatusEvent();
    HeatpadSampleEvent makeHeatpadSampleEvent();

    // send typed events
    void sendEvent(const HeatpadStatusEvent& event);
    void sendEvent(const HeatpadSampleEvent& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::publishEvent;

};

} // namespace Garbox