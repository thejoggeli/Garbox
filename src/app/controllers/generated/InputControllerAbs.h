// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class InputControllerAbs : public ControllerAbs {
public:
    
    // controller constructor
    InputControllerAbs(const RuntimeContext& context);

    // tick handlers
    virtual void onInputTick() = 0;

protected:

    // make typed events
    EventWrite<EventPayload::ButtonStateChanged> makeButtonStateChangedEvent();
    EventWrite<EventPayload::ButtonRepeat> makeButtonRepeatEvent();
    EventWrite<EventPayload::EncoderStep> makeEncoderStepEvent();

    // send typed events
    void sendEvent(const EventWrite<EventPayload::ButtonStateChanged>& event);
    void sendEvent(const EventWrite<EventPayload::ButtonRepeat>& event);
    void sendEvent(const EventWrite<EventPayload::EncoderStep>& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEvent;

};

} // namespace Garbox