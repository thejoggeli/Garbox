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
    InputControllerAbs();

    // tick handlers
    virtual void onInputTick() = 0;

protected:

    // shorthand types for write events 
    using ButtonStateChangedWrite = EventWrite<EventPayload::ButtonStateChanged>;
    using ButtonRepeatWrite = EventWrite<EventPayload::ButtonRepeat>;
    using EncoderStepWrite = EventWrite<EventPayload::EncoderStep>;

    // make typed events
    ButtonStateChangedWrite makeButtonStateChangedEvent();
    ButtonRepeatWrite makeButtonRepeatEvent();
    EncoderStepWrite makeEncoderStepEvent();

    // send typed events
    void sendEvent(const ButtonStateChangedWrite& event);
    void sendEvent(const ButtonRepeatWrite& event);
    void sendEvent(const EncoderStepWrite& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEvent;

};

} // namespace Garbox