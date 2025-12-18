#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class FanControllerAbs : public ControllerAbs {
public:

    // component constructor
    FanControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onInputTick() = 0;
    virtual void onOutputTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onFanCommand(const FanCommandEvent& event) = 0;

protected:

    // make typed events 
    FanStatusEvent makeFanStatusEvent();
    FanSampleEvent makeFanSampleEvent();

    // send typed events
    void sendEvent(const FanStatusEvent& event);
    void sendEvent(const FanSampleEvent& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

};

} // namespace Garbox